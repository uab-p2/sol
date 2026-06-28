"""
Parse C++ code to list defined classes and functions
and allow easy snippet display.
"""

from __future__ import annotations

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from clang.cindex import Config
from clang.cindex import Index, CursorKind, TranslationUnitLoadError
from dataclasses import dataclass
from enum import Enum, auto
from pathlib import Path
from quest import PROJECT_ROOT
import os
import sys
import textwrap


class SnippetType(Enum):
    CLASS_DEFINITION = "class"
    FUNCTION_OR_METHOD = "method"
    WHOLE_FILE = "file"


@dataclass
class Snippet:
    """An individual snippet of code."""
    name: str
    code: str
    relative_file_path: Path
    line_start: int
    line_end: int
    type: SnippetType

    @classmethod
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

            for d in tu.diagnostics:
                print(f"Parsing warning: {d}")

            for node in tu.cursor.walk_preorder():
                if node.kind == CursorKind.CLASS_DECL and node.is_definition():
                    try:
                        node_relative_path = Path(os.path.abspath(
                            node.location.file.name)).relative_to(PROJECT_ROOT)
                        if node_relative_path != relative_path:
                            continue
                    except ValueError:
                        continue

                    with open(PROJECT_ROOT / node_relative_path, "r") as f:
                        lines = f.read().splitlines()
                        comment_start = node.extent.start.line - 1
                        while comment_start > 0 and lines[comment_start - 1].lstrip().startswith("//"):
                            comment_start -= 1
                        snippets.append(Snippet(
                            name=node.spelling,
                            code="\n".join(lines[comment_start:node.extent.end.line]),
                            relative_file_path=node_relative_path,
                            line_start=node.extent.start.line,
                            line_end=node.extent.end.line,
                            type=SnippetType.CLASS_DEFINITION,
                        ))

        return snippets

    def __repr__(self) -> str:
        return (f"Snippet({self.type.value} {self.name}"
                f"@{self.relative_file_path.as_posix()}"
                f":{self.line_start}-{self.line_end})")

    def __str__(self) -> str:
        """The snippet as a markdown code block."""
        return textwrap.dedent(f"```cpp\n{self.code}\n```")
