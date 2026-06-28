#!/bin/env python3
"""Generate a quest index README based on the defined quest modules."""

from __future__ import annotations

import argparse
import re
import sys
import os
from pathlib import Path
from quest import Quest, Tag

LABEL_SECTION_CATEGORIES = "Categorías"
LABEL_SECTION_QUESTS = "Quests"
LABEL_QUEST_INDEX = "Índice de quests"
LABEL_INTRO = """\
Índice de quests disponibles en el proyecto SOL.
"""
CATEGORIES_INTRO = """\
Campañas y categorías de quests.
"""
QUESTS_INTRO = """\
Listado alfabético de quests.
"""

DEFAULT_IGNORED_TAGS = "plantilla,solucion"

from quest import PROJECT_ROOT, DEFAULT_QUEST_DIR, DEFAULT_TAG_DIR


def md_list_quests(
        base_dir: Path = DEFAULT_QUEST_DIR,
        ignored_tags: set[str] | None = None,
        tag_filter: set[str] | None = None,
) -> str:
    """Generate a Markdown-formatted list of quests, optionally filtered by tag.
    :param ignored_tags: names of tags to exclude from the output.
    :param tag_filter: return only quests that contain at least one tag with name equal to `tag_filter`.
    """

    if ignored_tags is None:
        ignored_tags = set()

    quests = Quest.list(base_dir)

    if tag_filter:
        available_tags = set(list_tag_names(base_dir))
        missing = tag_filter - available_tags
        if missing:
            raise ValueError(f"Unknown tags: {', '.join(sorted(missing))}")

    filtered_metadata = [quest for quest in quests
                         if not any(tag.name in ignored_tags for tag in quest.tags)]

    if tag_filter:
        filtered_metadata = [
            metadata
            for metadata in filtered_metadata
            if any(tag.name == tag_filter for tag in metadata.tags)
        ]

    if tag_filter:
        tag_names = sorted(tag_filter)
    else:
        seen_names: set[str] = set()
        tag_items: list[Tag] = []
        for tag in Tag.list(base_dir):
            if tag.name in ignored_tags or tag.name in seen_names:
                continue
            seen_names.add(tag.name)
            tag_items.append(tag)

    lines: list[str] = []

    lines.extend([f"## {LABEL_SECTION_QUESTS}", "", QUESTS_INTRO.strip(), ""])

    for metadata in sorted(filtered_metadata, key=lambda item: item.title.lower()):
        module_name = os.path.basename(metadata.module_path.as_posix())
        anchor = f"quest-{_slug(module_name)}"
        categories = sorted({tag.name for tag in metadata.tags if tag.name not in ignored_tags})

        first_paragraph = _first_paragraph(metadata.description)

        lines.append(f"<a id=\"{anchor}\"></a>")
        lines.append("")
        lines.append(f"* **[{metadata.title}]({module_name})** ({', '.join(categories)})")
        lines.append("")

    return "\n".join(lines).rstrip() + "\n"


def _resolve_base_dir(base_dir: str | Path) -> Path:
    """Resolve a base directory from cwd or, for relative paths, script location."""

    path = Path(base_dir)
    if path.exists():
        return path
    if not path.is_absolute():
        script_relative = Path(__file__).resolve().parent / path
        if script_relative.exists():
            return script_relative
    raise FileNotFoundError(path)


def _display_path(path: str | Path) -> str:
    """Return project-root-relative path text when possible."""

    resolved = Path(path).resolve()
    try:
        return resolved.relative_to(PROJECT_ROOT).as_posix()
    except ValueError:
        return str(resolved)


def _slug(value: str) -> str:
    """Convert a heading name into a stable anchor suffix."""

    return re.sub(r"[^a-z0-9]+", "-", value.lower()).strip("-")


def _sanitize_description(description: str) -> str:
    """Remove noisy HTML tags from README descriptions."""

    text = re.sub(r"<img[^>]*>", "", description, flags=re.IGNORECASE)
    text = re.sub(r"</?p[^>]*>", "", text, flags=re.IGNORECASE)
    return text.strip()


def _first_paragraph(description: str) -> str:
    """Return the first non-empty paragraph from a description."""

    cleaned = _sanitize_description(description)
    for paragraph in re.split(r"\n\s*\n", cleaned):
        normalized = " ".join(paragraph.split())
        if normalized:
            return normalized
    return ""


def build_parser() -> argparse.ArgumentParser:
    """Build CLI arguments for auto-index generation."""

    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )

    parser.add_argument(
        "dir",
        nargs="?",
        default=DEFAULT_QUEST_DIR,
        type=Path,
        help="Base quest directory (contains module subfolders with README.md)",
    )
    parser.add_argument(
        "readme",
        nargs="?",
        default=None,
        type=Path,
        help="Output README path instead of <dir>/README.md",
    )

    parser.add_argument(
        "--ignore",
        type=str,
        default=DEFAULT_IGNORED_TAGS,
        help="Comma-separated list of tag names to exclude quests",
    )

    parser.add_argument(
        "--tag",
        type=str,
        default="",
        help="Comma-separated list of tag names to include, default is to accept all",
    )

    return parser


def main(argv: list[str] | None = None) -> int:
    """Generate the index README file."""

    args = build_parser().parse_args(argv)
    base_dir = _resolve_base_dir(args.dir)
    readme_path = Path(args.readme) if args.readme is not None else base_dir / "README.md"

    ignored_tags = {tag_name.strip() for tag_name in args.ignore.split(",") if tag_name.strip()}
    tag_filter = {tag_name.strip() for tag_name in args.tag.split(",") if tag_name.strip()} or None

    try:
        content = md_list_quests(base_dir, ignored_tags, tag_filter)
        readme_path.write_text(content, encoding="utf-8")
        print(f"Autoindex {_display_path(readme_path)} created from {_display_path(base_dir)}")
        return 0
    except ValueError as error:
        print(f"Error: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
