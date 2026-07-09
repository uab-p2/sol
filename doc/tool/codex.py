#!/usr/bin/env python3
"""Parse codex section documents into dataclasses and allow listing
them all."""

from __future__ import annotations

import glob
from dataclasses import dataclass
from pathlib import Path



try:
    from markdown import join_section, read_markdown_lines, require_first_h1
    from quest import PROJECT_ROOT
except ImportError:  # pragma: no cover - import path when used as package `tool.codex`
    from tool.markdown import join_section, read_markdown_lines, require_first_h1
    from tool.quest import PROJECT_ROOT

DEFAULT_CODEX_DIR = PROJECT_ROOT / "doc" / "guide" / "sections" / "codex"


@dataclass
class Codex:
    """A codex entry."""
    name: str
    title: str
    description: str

    @classmethod
    def list(cls) -> list[Codex]:
        codex_paths = glob.glob((DEFAULT_CODEX_DIR / "*.md").resolve().as_posix(), recursive=False)
        return sorted([Codex.from_file(p) for p in codex_paths], key=lambda codex: codex.name)

    @classmethod
    def from_file(cls, path: str | Path) -> Codex:
        """Read codex metadata from a markdown file.
        - `name`: markdown filename stem
        - `title`: first level-1 heading
        - `description`: markdown text after title heading
        """

        path = Path(path)
        lines = read_markdown_lines(path)
        title_index, title = require_first_h1(lines, path)
        description = join_section(lines, title_index + 1)
        return cls(name=path.stem, title=title, description=description)
