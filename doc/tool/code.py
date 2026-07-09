#!/usr/bin/env python3
"""
Parse C++ code to list defined classes and functions
and allow easy snippet display.
"""

from __future__ import annotations

import sys
import subprocess
import ctypes.util
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from clang.cindex import Config
from clang.cindex import Index, CursorKind, TranslationUnitLoadError, LibclangError
from dataclasses import dataclass
from enum import Enum
from functools import lru_cache
from pathlib import Path
from quest import PROJECT_ROOT
import os
import textwrap


def _configure_libclang() -> None:
    """Configure libclang location for python clang bindings."""
    if os.getenv("LIBCLANG_FILE"):
        Config.set_library_file(os.environ["LIBCLANG_FILE"])
        return
    if os.getenv("LIBCLANG_PATH"):
        Config.set_library_path(os.environ["LIBCLANG_PATH"])
        return

    # Best effort fallback for CI runners that have llvm-config available.
    try:
        libdir = subprocess.check_output(["llvm-config", "--libdir"], text=True).strip()
        if libdir:
            Config.set_library_path(libdir)
            return
    except (subprocess.SubprocessError, FileNotFoundError):
        pass

    library_name = ctypes.util.find_library("clang")
    if library_name:
        Config.set_library_file(library_name)


_configure_libclang()


class SnippetType(Enum):
    WHOLE_FILE = "file"
    CLASS = "class"
    # These include constructors (I know, I know...)
    DECLARATION = "meth-dcl"
    DEFINITION = "meth-def"

    @classmethod
    def from_node(cls, node: CursorKind) -> SnippetType:
        if node.kind == CursorKind.CLASS_DECL:
            assert node.is_definition(), "Class declarations must be definitions"
            return cls.CLASS
        elif node.kind in (CursorKind.CXX_METHOD,
                           CursorKind.CONSTRUCTOR,
                           CursorKind.DESTRUCTOR,
                           CursorKind.FUNCTION_DECL,
                           CursorKind.CONVERSION_FUNCTION):
            if node.is_definition():
                return cls.DEFINITION
            else:
                return cls.DECLARATION
        else:
            raise ValueError(f"Unsupported node kind: {node.kind}")


@dataclass
class Snippet:
    """An individual snippet of code."""
    name: str
    code: str
    relative_file_path: Path
    line_start: int
    line_end: int
    type: SnippetType
    # Only for method definitions.
    return_type: str | None = None
    arg_types: list[str] | None = None

    def __post_init__(self):
        if self.return_type is not None:
            self.return_type = self.return_type.replace(" &", "&")
            self.return_type = self.return_type.replace(" *", "*")
        if self.arg_types is not None:
            self.arg_types = [arg_type.replace(" &", "&") for arg_type in self.arg_types]
            self.arg_types = [arg_type.replace(" *", "*") for arg_type in self.arg_types]

    @classmethod
    @lru_cache
    def list(cls, path: str | Path | None = None) -> list[Snippet]:
        """List all snippets in a file, or in the whole project if no path is given."""

        snippets: list[Snippet] = []

        paths = []
        if path is None:
            paths = ([p for p in Path(PROJECT_ROOT / "src").rglob("*.cpp")]
                     + [p for p in Path(PROJECT_ROOT).rglob("*.h")])
        else:
            paths = [path]

        for path in paths:
            path = Path(path) if path is not None else None
            try:
                relative_path = path.relative_to(PROJECT_ROOT)
            except ValueError as ex:
                print(f"Path must be under project root {PROJECT_ROOT}, but got {path}")
                raise ex

            with open(path, "r") as f:
                lines = f.read().splitlines()

            snippets.append(Snippet(
                name=path.name,
                code="\n".join(lines),
                relative_file_path=relative_path,
                line_start=1,
                line_end=len(lines),
                type=SnippetType.WHOLE_FILE,
            ))

            try:
                tu = Index.create().parse(
                    path,
                    args=[
                        "-x", "c++",
                        "-std=c++23",
                        "-I/usr/include",
                        "-I/usr/include/x86_64-linux-gnu",
                        f"-I{os.path.abspath(PROJECT_ROOT)}/src",
                    ],
                )
            except LibclangError as ex:
                raise RuntimeError(
                    "Unable to load libclang. Set LIBCLANG_FILE to the full libclang.so path "
                    "or LIBCLANG_PATH to the directory containing libclang.so."
                ) from ex

            for d in tu.diagnostics:
                print(f"Parsing warning: {d}")

            for node in tu.cursor.walk_preorder():
                try:
                    node_kind = node.kind
                except ValueError:
                    # Some cursor kinds emitted by libclang (e.g. for newer
                    # C++ constructs like concepts/deduction guides) aren't
                    # registered in the `clang` python bindings' CursorKind
                    # table, and accessing `.kind` raises ValueError instead
                    # of returning an unrecognized value. Just skip them.
                    continue
                if node_kind not in (
                        CursorKind.CLASS_DECL,
                        CursorKind.CXX_METHOD,
                        CursorKind.CONSTRUCTOR,
                        CursorKind.DESTRUCTOR,
                        CursorKind.FUNCTION_DECL,
                        CursorKind.CONVERSION_FUNCTION):
                    continue
                try:
                    node_relative_path = Path(os.path.abspath(
                        node.location.file.name)).relative_to(PROJECT_ROOT)
                    if node_relative_path != relative_path:
                        continue
                except ValueError:
                    continue
                if node_kind == CursorKind.CLASS_DECL and not node.is_definition():
                    continue

                arg_types = None
                return_type = None
                snippet_name = node.spelling
                if node.kind in (
                        CursorKind.CXX_METHOD,
                        CursorKind.CONSTRUCTOR,
                        CursorKind.DESTRUCTOR,
                        CursorKind.FUNCTION_DECL,
                        CursorKind.CONVERSION_FUNCTION):
                    arg_types = [arg.type.spelling for arg in node.get_arguments()]
                if node.kind in (
                        CursorKind.CONSTRUCTOR,
                        CursorKind.DESTRUCTOR,
                        CursorKind.CXX_METHOD,
                        CursorKind.CONVERSION_FUNCTION):
                    return_type = node.result_type.spelling
                    snippet_name = f"{node.semantic_parent.spelling}::{node.spelling}"

                comment_start = node.extent.start.line - 1
                while comment_start > 0 and lines[comment_start - 1].lstrip().startswith("//"):
                    comment_start -= 1
                snippets.append(Snippet(
                    name=snippet_name,
                    code="\n".join(lines[comment_start:node.extent.end.line]),
                    relative_file_path=node_relative_path,
                    line_start=node.extent.start.line,
                    line_end=node.extent.end.line,
                    type=SnippetType.from_node(node),
                    arg_types=arg_types,
                    return_type=return_type,
                ))

        return snippets

    @property
    def label(self) -> str:
        if self.type in (SnippetType.WHOLE_FILE, SnippetType.CLASS):
            return self.name

        ret_str = f"{self.return_type} " if self.return_type else ""
        arg_str = "(" + ", ".join(self.arg_types) + ")" if self.arg_types is not None else ""
        return f"{ret_str}{self.name}{arg_str}"

    def __repr__(self) -> str:
        return (f"Snippet({self.type.value} {self.label}"
                f" @ {self.relative_file_path.as_posix()}"
                f":{self.line_start}-{self.line_end})[{self.name}]")

    def __str__(self) -> str:
        """The snippet as a markdown code block."""
        return f"```cpp linenums=\"{self.line_start}\"\n{self.code}\n```"


if __name__ == '__main__':
    snippets = Snippet.list()
    for snippet in snippets:
        print(
            f"{snippet!r}"
            # f"\n{snippet!s}"
        )
