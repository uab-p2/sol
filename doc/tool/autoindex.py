#!/bin/env python3
"""Generate a quest index README grouped by tags and quest modules."""

from __future__ import annotations

import argparse
import re
import sys
import os
from pathlib import Path

# Add the parent directory to the path so we can import tag
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from tag import Metadata, list_metadata, list_tag_names

LABEL_SECTION_CATEGORIES = "Categories"
LABEL_SECTION_QUESTS = "Quests"
LABEL_INTRO = """\
Índice de quests disponibles en el proyecto SOL.
"""
CATEGORIES_INTRO = """\
Categorías y series de quests.
"""
QUESTS_INTRO = """\
Listado alfabético de quests.
"""

PROJECT_ROOT = Path(__file__).resolve().parents[2]
DEFAULT_QUEST_DIR = PROJECT_ROOT / "quest"
DEFAULT_CATEGORIES_PATH = PROJECT_ROOT / "doc" / "categories.md"


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


def _members_for_tag(metadata_list: list[Metadata], tag_name: str) -> list[Metadata]:
    """Return tag members ordered as regular tags or indexed series."""

    series_matches: list[tuple[int, Metadata]] = []
    regular_matches: list[Metadata] = []

    for metadata in metadata_list:
        matching_tags = [tag for tag in metadata.tags if tag.name == tag_name]
        if not matching_tags:
            continue

        indexed = [tag.index for tag in matching_tags if tag.index is not None]
        if indexed:
            series_matches.append((min(indexed), metadata))
        else:
            regular_matches.append(metadata)

    if series_matches:
        return [
            metadata
            for _, metadata in sorted(
                series_matches,
                key=lambda item: (item[0], item[1].module_path.as_posix()),
            )
        ]

    return sorted(regular_matches, key=lambda metadata: metadata.module_path.as_posix())


def read_category_descriptions(categories_path: Path) -> dict[str, str]:
    """Read markdown category descriptions keyed by top-level `#` headings."""

    if not categories_path.exists():
        return {}

    lines = categories_path.read_text(encoding="utf-8").splitlines()
    descriptions: dict[str, str] = {}
    current_name: str | None = None
    current_lines: list[str] = []

    def flush_current() -> None:
        if current_name is None:
            return
        descriptions[current_name] = "\n".join(current_lines).strip()

    for line in lines:
        match = re.match(r"^#\s+(.+?)\s*$", line)
        if match:
            flush_current()
            current_name = match.group(1).strip()
            current_lines = []
            continue

        if current_name is not None:
            current_lines.append(line)

    flush_current()
    return descriptions


def list_defined_categories(categories_path: Path) -> list[str]:
    """List category names defined in the categories markdown file."""

    return sorted(read_category_descriptions(categories_path).keys())


def build_index_markdown(
        base_dir: Path,
        ignored_tags: set[str] | None = None,
        tag_filter: set[str] | None = None,
        categories_path: Path | None = None,
) -> str:
    """Build markdown content for the generated quest index README.

    Args:
        base_dir: Base quest directory.
        ignored_tags: Set of tag names to exclude quests by (default: none).
        tag_filter: Optional set of tag names to include; if None, all categories shown alphabetically.
        categories_path: Path to categories markdown file (default: doc/categories.md).
    """

    if ignored_tags is None:
        ignored_tags = set()

    if categories_path is None:
        categories_path = DEFAULT_CATEGORIES_PATH

    category_descriptions = read_category_descriptions(categories_path)
    metadata_list = list_metadata(base_dir)

    if tag_filter:
        available_tags = set(list_tag_names(base_dir))
        missing = tag_filter - available_tags
        if missing:
            raise ValueError(f"Unknown tags: {', '.join(sorted(missing))}")

    filtered_metadata = [
        metadata
        for metadata in metadata_list
        if not any(tag.name in ignored_tags for tag in metadata.tags)
    ]

    if tag_filter:
        filtered_metadata = [
            metadata
            for metadata in filtered_metadata
            if any(tag.name in tag_filter for tag in metadata.tags)
        ]

    if tag_filter:
        tag_names = sorted(tag_filter)
    else:
        all_tags = list_tag_names(base_dir)
        tag_names = [tag_name for tag_name in all_tags if tag_name not in ignored_tags]

    lines: list[str] = ["# Quest Index", "", LABEL_INTRO.strip(), "", f"## {LABEL_SECTION_CATEGORIES}", "",
                        CATEGORIES_INTRO.strip(), ""]

    for tag_name in tag_names:
        lines.append(f"### {tag_name}")
        category_description = category_descriptions.get(tag_name, "").strip()
        if category_description:
            lines.append(category_description)
            lines.append("")

        for metadata in _members_for_tag(filtered_metadata, tag_name):
            module_name = metadata.module_path.as_posix()
            lines.append(f"- [{module_name}](#quest-{_slug(module_name)})")
        lines.append("")

    lines.extend([f"## {LABEL_SECTION_QUESTS}", "", QUESTS_INTRO.strip(), ""])

    for metadata in sorted(filtered_metadata, key=lambda item: item.module_path.as_posix()):
        module_name = metadata.module_path.as_posix()
        anchor = f"quest-{_slug(module_name)}"
        categories = sorted({tag.name for tag in metadata.tags if tag.name not in ignored_tags})

        first_paragraph = _first_paragraph(metadata.description)

        lines.append(f"<a id=\"{anchor}\"></a>")
        lines.append(f"### [{metadata.title}]({module_name})")
        lines.append(f"({', '.join(categories)})")
        if first_paragraph:
            lines.append("")
            lines.append(first_paragraph)
        lines.append("")

    return "\n".join(lines).rstrip() + "\n"


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
        help="Output README path (default: <dir>/README.md)",
    )

    parser.add_argument(
        "--ignore",
        type=str,
        default="",
        help="Comma-separated list of tag names to exclude quests",
    )

    parser.add_argument(
        "--tag",
        type=str,
        default="",
        help="Comma-separated list of tag names to include (default: show all categories alphabetically)",
    )

    parser.add_argument(
        "--categories",
        type=Path,
        default=DEFAULT_CATEGORIES_PATH,
        help="Categories markdown file",
    )

    return parser


def main(argv: list[str] | None = None) -> int:
    """Generate the index README file."""

    args = build_parser().parse_args(argv)
    base_dir = _resolve_base_dir(args.dir)
    readme_path = Path(args.readme) if args.readme is not None else base_dir / "README.md"
    categories_path = args.categories

    ignored_tags = {tag_name.strip() for tag_name in args.ignore.split(",") if tag_name.strip()}
    tag_filter = {tag_name.strip() for tag_name in args.tag.split(",") if tag_name.strip()} or None

    try:
        content = build_index_markdown(base_dir, ignored_tags, tag_filter, categories_path)
    except ValueError as error:
        print(f"Error: {error}", file=sys.stderr)
        return 1

    readme_path.write_text(content, encoding="utf-8")
    print(f"Autoindex {_display_path(readme_path)} created from {_display_path(base_dir)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
