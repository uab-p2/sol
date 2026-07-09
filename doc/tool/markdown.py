"""Small shared helpers for parsing project markdown metadata files."""

from __future__ import annotations

import re
from pathlib import Path

H1_RE = re.compile(r"^#\s+(.+\S)\s*$")


def read_markdown_lines(path: str | Path) -> list[str]:
    """Read a markdown file as UTF-8 and return its lines without newlines."""

    return Path(path).read_text(encoding="utf-8").splitlines()


def find_first_h1(lines: list[str]) -> tuple[int, str] | None:
    """Return `(line_index, title)` for the first level-1 heading in `lines`."""

    for index, line in enumerate(lines):
        match = H1_RE.match(line)
        if match:
            return index, match.group(1).strip()
    return None


def require_first_h1(lines: list[str], source_path: str | Path) -> tuple[int, str]:
    """Like `find_first_h1` but raises a useful error when missing."""

    heading = find_first_h1(lines)
    if heading is None:
        raise ValueError(f"No title found in {Path(source_path)}")
    return heading


def find_last_h1_named(lines: list[str], heading_name: str) -> int | None:
    """Return the last `# <heading_name>` index, case-insensitive."""

    needle = heading_name.strip().lower()
    indexes = [
        index
        for index, line in enumerate(lines)
        if (match := H1_RE.match(line)) and match.group(1).strip().lower() == needle
    ]
    return indexes[-1] if indexes else None


def join_section(lines: list[str], start: int, end: int | None = None) -> str:
    """Join a line slice into markdown text without leading/trailing blank lines."""

    return "\n".join(lines[start:end]).strip()


def sanitize_markdown_description(description: str) -> str:
    """Remove common noisy HTML wrappers from markdown descriptions."""

    text = re.sub(r"<img[^>]*>", "", description, flags=re.IGNORECASE)
    text = re.sub(r"</?p[^>]*>", "", text, flags=re.IGNORECASE)
    return text.strip()


def first_paragraph(description: str) -> str:
    """Return first non-empty paragraph from a markdown description."""

    cleaned = sanitize_markdown_description(description)
    for paragraph in re.split(r"\n\s*\n", cleaned):
        normalized = " ".join(paragraph.split())
        if normalized:
            return normalized
    return ""
