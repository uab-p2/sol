#!/usr/bin/env python3
"""Parse quest README metadata and expose tag query utilities and CLI commands."""

from __future__ import annotations

import argparse
import re
from dataclasses import dataclass
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parents[2]
DEFAULT_QUEST_DIR = PROJECT_ROOT / "quest"
DEFAULT_TAG_DIR = PROJECT_ROOT / "doc" / "tag"
__all__ = [
    "Tag",
    "Metadata",
    "parse_tags",
    "read_readme_metadata",
    "read_tag_metadata",
    "list_metadata",
    "list_tag_metadata",
    "list_tags",
    "list_tag_names",
    "list_metadata_for_tag",
    "list_metadata_for_series",
]


@dataclass(slots=True)
class Tag:
    """A tag reference, optionally enriched with metadata from `doc/tag/<name>.md`."""

    name: str
    index: int | None = None
    title: str = ""
    description: str = ""

    def __lt__(self, other: Tag) -> bool:
        return self.name < other.name


@dataclass(slots=True)
class Metadata:
    """Normalized metadata extracted from one quest `README.md` file."""

    module_path: Path
    title: str
    description: str
    tags: list[Tag]

    def __repr__(self) -> str:
        return f"{self.title} ({' '.join(format_tag(tag) for tag in sorted(self.tags))})"


def format_tag(tag: Tag) -> str:
    """Format a tag for display."""

    return tag.name if tag.index is None else f"{tag.name}:{tag.index}"


def _parse_readme_tags(tag_text: str) -> list[Tag]:
    """Parse a tags section into `Tag` objects from space/comma-separated tokens."""

    tags: list[Tag] = []
    for raw_tag in (tag for tag in re.split(r"[\s,]+", tag_text.strip()) if tag):
        match = re.fullmatch(r"([^:,\s]+)(?::(\d+))?", raw_tag)
        if not match:
            raise ValueError(f"Invalid tag {raw_tag!r}")
        tags.append(Tag(name=match.group(1), index=int(match.group(2)) if match.group(2) is not None else None))
    return tags


def parse_tags(tag_path: str | Path) -> Tag:
    """Parse a tag markdown file into a `Tag` object.

    The tag name comes from the file stem, the title from the first level-1
    markdown heading, and the description from the rest of the file.
    """

    path = Path(tag_path)
    text = path.read_text(encoding="utf-8")
    lines = text.splitlines()

    title_index = next(
        (index for index, line in enumerate(lines) if re.match(r"^#\s+.+\S\s*$", line)),
        None,
    )
    if title_index is None:
        raise ValueError(f"No title found in {path}")

    title = re.sub(r"^#\s+", "", lines[title_index]).strip()
    description = "\n".join(lines[title_index + 1:]).strip()
    return Tag(name=path.stem, title=title, description=description)


def read_tag_metadata(tag_path: Path) -> Tag:
    """Read metadata for one standalone markdown tag file."""

    return parse_tags(tag_path)


def read_readme_metadata(readme_path: Path) -> Metadata:
    """Read the title, description, and tags from a README markdown file.

    The title is taken from the first level-1 heading (`# ...`). The
    description is everything between that heading and the last `# Tags`
    section, case-insensitively detected. The tags section is parsed as
    space- or comma-separated words where each tag may optionally use
    `:<number>` to encode an index.
    """

    text = readme_path.read_text(encoding="utf-8")
    lines = text.splitlines()

    title_index = next(
        (index for index, line in enumerate(lines) if re.match(r"^#\s+.+\S\s*$", line)),
        None,
    )
    if title_index is None:
        raise ValueError(f"No title found in {readme_path}")
    title = re.sub(r"^#\s+", "", lines[title_index]).strip()

    tag_heading_indexes = [
        index
        for index, line in enumerate(lines)
        if re.match(r"^#\s+tags\s*$", line, flags=re.IGNORECASE)
    ]
    if not tag_heading_indexes:
        raise ValueError(f"No tags section found in {readme_path}")
    tags_index = tag_heading_indexes[-1]

    description = "\n".join(lines[title_index + 1: tags_index]).strip()
    tag_text = "\n".join(lines[tags_index + 1:]).strip()
    tags = _parse_readme_tags(tag_text)
    if not tags:
        raise ValueError(f"No tags found in {readme_path}")

    return Metadata(module_path=readme_path.parent, title=title, description=description, tags=tags)


def _resolve_root_dir(root_dir: str | Path) -> Path:
    """Resolve a root path, with a fallback relative to this script location."""

    root = Path(root_dir)
    if root.exists():
        return root
    if not root.is_absolute():
        script_relative = Path(__file__).resolve().parent / root
        if script_relative.exists():
            return script_relative
    raise FileNotFoundError(root)


def list_metadata(root_dir: str | Path = DEFAULT_QUEST_DIR) -> list[Metadata]:
    """Return metadata for every `README.md` discovered under `root_dir`."""

    root = _resolve_root_dir(root_dir)
    readme_paths = sorted(
        (
            child / "README.md"
            for child in root.iterdir()
            if child.is_dir() and (child / "README.md").is_file()
        ),
        key=lambda path: path.parent.name,
    )

    metadata_list: list[Metadata] = []
    for readme_path in readme_paths:
        metadata = read_readme_metadata(readme_path)
        metadata.module_path = readme_path.parent.relative_to(root)
        metadata_list.append(metadata)

    return metadata_list


def list_tag_metadata(tag_dir: str | Path = DEFAULT_TAG_DIR) -> list[Tag]:
    """Return metadata for every standalone tag markdown file in `tag_dir`."""

    root = _resolve_root_dir(tag_dir)
    tag_paths = sorted((child for child in root.iterdir() if child.is_file() and child.suffix == ".md"), key=lambda path: path.stem)
    return [read_tag_metadata(tag_path) for tag_path in tag_paths]


def list_tags(root_dir: str | Path = DEFAULT_QUEST_DIR, tag_dir: str | Path = DEFAULT_TAG_DIR) -> list[Tag]:
    """Return unique tags sorted alphabetically, enriched with tag-file metadata."""

    metadata_list = list_metadata(root_dir)
    unique_tags: dict[tuple[str, int | None], Tag] = {}
    for metadata in metadata_list:
        for tag in metadata.tags:
            unique_tags.setdefault((tag.name, tag.index), tag)

    tag_metadata = {tag.name: tag for tag in list_tag_metadata(tag_dir)}
    for tag in unique_tags.values():
        metadata = tag_metadata.get(tag.name)
        if metadata is None:
            continue
        tag.title = metadata.title
        tag.description = metadata.description

    return sorted(unique_tags.values(), key=lambda tag: (tag.name, -1 if tag.index is None else tag.index))


def list_tag_names(root_dir: str | Path = DEFAULT_QUEST_DIR) -> list[str]:
    """Return unique tag names sorted alphabetically, ignoring tag indices."""

    metadata_list = list_metadata(root_dir)
    return sorted({tag.name for metadata in metadata_list for tag in metadata.tags})


def list_metadata_for_series(root_dir: str | Path, series_name: str) -> list[Metadata]:
    """Return members tagged as `series_name:<index>`, sorted by index then title."""

    metadata_list = list_metadata(root_dir)
    indexed_matching: list[tuple[int, Metadata]] = []

    for metadata in metadata_list:
        indexed_tags = [tag.index for tag in metadata.tags if tag.name == series_name and tag.index is not None]
        if indexed_tags:
            indexed_matching.append((min(indexed_tags), metadata))

    return [metadata for _, metadata in sorted(indexed_matching, key=lambda item: (item[0], item[1].title))]


def list_metadata_for_tag(root_dir: str | Path, tag_name: str) -> list[Metadata]:
    """Return members for one tag, preferring indexed-series matches when present."""

    series_members = list_metadata_for_series(root_dir, tag_name)
    if series_members:
        return series_members

    metadata_list = list_metadata(root_dir)
    regular_matching = [
        metadata
        for metadata in metadata_list
        if any(tag.name == tag_name for tag in metadata.tags)
    ]
    return sorted(regular_matching, key=lambda item: item.title)


def iter_readme_metadata(root_dir: str | Path) -> list[Metadata]:
    """Backward-compatible function alias for `list_metadata`."""

    return list_metadata(root_dir)


def show(folder_dir: str | Path) -> int:
    """Show the metadata for one quest folder."""

    folder = Path(folder_dir)
    metadata = read_readme_metadata(folder / "README.md")
    print(metadata)
    return 0


def tag_name(root_dir: str | Path = DEFAULT_QUEST_DIR, selected_tag_name: str | None = None) -> int:
    """Print all tag names, or members for one tag when a name is provided."""

    if selected_tag_name is None:
        print("\n".join(list_tag_names(root_dir)))
        return 0

    for metadata in list_metadata_for_tag(root_dir, selected_tag_name):
        print(metadata)
    return 0


def build_parser() -> argparse.ArgumentParser:
    """Build the CLI argument parser."""

    parser = argparse.ArgumentParser(
        description=__doc__ or "Inspect README metadata",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    subparsers = parser.add_subparsers(dest="command", required=True)

    show_parser = subparsers.add_parser(
        "show",
        help="Show metadata for a quest folder",
    )
    show_parser.add_argument("folder", help="Folder containing README.md")
    show_parser.set_defaults(func=lambda args: show(args.folder))

    tag_parser = subparsers.add_parser(
        "tag",
        help="List all tags or members in one tag",
    )
    tag_parser.add_argument(
        "--dir",
        default=DEFAULT_QUEST_DIR,
        type=Path,
        help="Quest directory root where the module dirs live",
    )
    tag_parser.add_argument(
        "tag_name",
        nargs="?",
        help="Tag name to filter by",
    )
    tag_parser.set_defaults(func=lambda args: tag_name(args.dir, args.tag_name))

    return parser


def main(argv: list[str] | None = None) -> int:
    """Run the command-line interface."""

    parser = build_parser()
    args = parser.parse_args(argv)
    return args.func(args)


if __name__ == '__main__':
    raise SystemExit(main())
