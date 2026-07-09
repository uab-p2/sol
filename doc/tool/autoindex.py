#!/bin/env python3
"""Automatic generation of indices by looking at quest README.md files.

- Generate the README.md in `quest/` that lists all quests.
- Update the `mkdocs.yml` file to include quests in the navigation
"""

from __future__ import annotations

import argparse
import os
import re
import sys
from pathlib import Path

import yaml

from quest import PROJECT_ROOT, DEFAULT_QUEST_DIR
from quest import Quest, Tag

try:
    from markdown import first_paragraph as _shared_first_paragraph
    from markdown import sanitize_markdown_description as _shared_sanitize_description
except ImportError:  # pragma: no cover - import path when used as package `tool.autoindex`
    from tool.markdown import first_paragraph as _shared_first_paragraph
    from tool.markdown import sanitize_markdown_description as _shared_sanitize_description

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

CAMPAIGN_PREFIX = "auto/campaign_"
QUEST_PREFIX = "auto/quest_"
DEFAULT_MKDOCS_PATH = PROJECT_ROOT / "doc" / "guide" / "mkdocs.yml"

NAV_ROOT_LABEL = "nav"
QUEST_NAV_KEY = "quest()"
QUEST_NAV_LABEL = "quest():"


def list_tag_names(base_dir: Path = DEFAULT_QUEST_DIR) -> list[str]:
    """Return all available tag names in `base_dir`."""

    return sorted({tag.name for tag in Tag.list(base_dir)})


def md_list_quests(
    base_dir: Path = DEFAULT_QUEST_DIR,
    ignored_tags: set[str] | None = None,
    tag_filter: set[str] | None = None,
) -> str:
    """Generate a Markdown-formatted list of quests, optionally filtered by tag.

    :param ignored_tags: names of tags to exclude from the output.
    :param tag_filter: return only quests that contain at least one allowed tag.
    """

    if ignored_tags is None:
        ignored_tags = set()

    quests = Quest.list(base_dir)

    if tag_filter:
        available_tags = set(list_tag_names(base_dir))
        missing = tag_filter - available_tags
        if missing:
            raise ValueError(f"Unknown tags: {', '.join(sorted(missing))}")

    filtered_metadata = [
        quest for quest in quests if not any(tag.name in ignored_tags for tag in (quest.tags or []))
    ]

    if tag_filter:
        filtered_metadata = [
            metadata
            for metadata in filtered_metadata
            if any(tag.name in tag_filter for tag in (metadata.tags or []))
        ]

    lines: list[str] = []
    lines.extend([f"## {LABEL_SECTION_QUESTS}", "", QUESTS_INTRO.strip(), ""])

    for metadata in sorted(filtered_metadata, key=lambda item: item.title.lower()):
        module_name = os.path.basename(metadata.module_path.as_posix())
        anchor = f"quest-{_slug(module_name)}"
        categories = sorted({tag.name for tag in (metadata.tags or []) if tag.name not in ignored_tags})

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

    return _shared_sanitize_description(description)


def _first_paragraph(description: str) -> str:
    """Return the first non-empty paragraph from a description."""

    return _shared_first_paragraph(description)


def _extract_campaign_tag(path: str) -> str | None:
    """Return campaign tag from `auto/campaign_<tag>.md`, otherwise None."""

    if not isinstance(path, str) or not path.startswith(CAMPAIGN_PREFIX):
        return None

    base = os.path.basename(path)
    if not (base.startswith("campaign_") and base.endswith(".md")):
        return None

    return base[len("campaign_") : -len(".md")]


def _campaign_nav_path(value: object) -> str | None:
    """Return the campaign markdown path from a nav node value, if present."""

    if isinstance(value, str) and value.startswith(CAMPAIGN_PREFIX):
        return value

    if not isinstance(value, list) or not value:
        return None

    first = value[0]
    if isinstance(first, str) and first.startswith(CAMPAIGN_PREFIX):
        return first

    if isinstance(first, dict) and len(first) == 1:
        intro_key, intro_value = next(iter(first.items()))
        if isinstance(intro_key, str) and intro_key.strip().lower() == "intro" and isinstance(intro_value, str):
            if intro_value.startswith(CAMPAIGN_PREFIX):
                return intro_value

    return None


def _iter_quest_campaign_nodes(nav: object):
    """Yield campaign nav nodes under top-level `quest():` entries."""

    if not isinstance(nav, list):
        return

    for top in nav:
        if not (isinstance(top, dict) and QUEST_NAV_KEY in top and isinstance(top[QUEST_NAV_KEY], list)):
            continue

        for item in top[QUEST_NAV_KEY]:
            if not (isinstance(item, dict) and len(item) == 1):
                continue

            label, value = next(iter(item.items()))
            if _campaign_nav_path(value) is not None:
                yield item, label, value


def _build_campaign_children(tag_name: str) -> list[dict[str, str]]:
    """Build quest subsubsection entries for one campaign tag sorted by tag index."""

    quests = [
        quest
        for quest in Quest.list(DEFAULT_QUEST_DIR)
        if any(tag.name == tag_name for tag in (quest.tags or []))
    ]

    def campaign_index(quest: Quest) -> int | None:
        for tag in (quest.tags or []):
            if tag.name == tag_name:
                return tag.index
        return None

    quests = sorted(
        quests,
        key=lambda q: (
            campaign_index(q) is None,
            campaign_index(q) if campaign_index(q) is not None else sys.maxsize,
            q.title.lower(),
        ),
    )

    children: list[dict[str, str]] = []
    for quest in quests:
        quest_name = quest.module_path.name
        children.append({f"{quest.title}()": f"{QUEST_PREFIX}{quest_name}.md"})

    return children


def _find_quest_block_range(lines: list[str]) -> tuple[int, int] | None:
    """Return `[start, end)` line indices for the `- quest():` block in `nav:`."""

    nav_start: int | None = None
    nav_end = len(lines)

    for idx, line in enumerate(lines):
        if re.match(rf"^{re.escape(NAV_ROOT_LABEL)}:\s*$", line):
            nav_start = idx
            break

    if nav_start is None:
        return None

    for idx in range(nav_start + 1, len(lines)):
        if re.match(r"^[^\s#][^:]*:\s*$", lines[idx]):
            nav_end = idx
            break

    quest_start: int | None = None
    for idx in range(nav_start + 1, nav_end):
        if re.match(r"^  - quest\(\):\s*$", lines[idx]):
            quest_start = idx
            break

    if quest_start is None:
        return None

    quest_end = nav_end
    for idx in range(quest_start + 1, nav_end):
        if re.match(r"^  - ", lines[idx]):
            quest_end = idx
            break

    return quest_start, quest_end


def _format_yaml_scalar(value: str) -> str:
    """Format a scalar conservatively for hand-written YAML snippets."""

    if value == "" or value != value.strip() or ": " in value or " #" in value or value.startswith("#"):
        escaped = value.replace("\\", "\\\\").replace('"', '\\"')
        return f'"{escaped}"'
    return value


def _render_nav_item(key: str, value: object, indent: int) -> list[str]:
    """Render one generic MkDocs nav item as YAML lines at the requested indentation."""

    key_text = _format_yaml_scalar(key)

    if isinstance(value, str):
        return [f"{' ' * indent}- {key_text}: {_format_yaml_scalar(value)}"]

    if isinstance(value, list):
        lines = [f"{' ' * indent}- {key_text}:"]
        for child in value:
            if isinstance(child, str):
                lines.append(f"{' ' * (indent + 2)}- {_format_yaml_scalar(child)}")
            elif isinstance(child, dict) and len(child) == 1:
                child_key, child_value = next(iter(child.items()))
                lines.extend(_render_nav_item(str(child_key), child_value, indent + 2))
        return lines

    return [f"{' ' * indent}- {key_text}: {_format_yaml_scalar(str(value))}"]


def _render_quest_nav_block(quest_items: list[object]) -> str:
    """Render the full `- quest():` block.

    Campaign entries render as:
      - <Campaign>:
          - Intro: auto/campaign_<tag>.md
          - <Quest title>(): auto/quest_<name>.md

    Non-campaign entries are rendered as generic nav items.
    """

    lines = ["  - quest():"]

    for entry in quest_items:
        if not (isinstance(entry, dict) and len(entry) == 1):
            continue

        key, value = next(iter(entry.items()))

        if isinstance(value, list) and len(value) >= 1 and isinstance(value[0], str) and value[0].startswith(CAMPAIGN_PREFIX):
            campaign_path = value[0]
            quest_children = value[1:]

            lines.append(f"      - {_format_yaml_scalar(str(key))}:")
            lines.append(f"          - Intro: {_format_yaml_scalar(campaign_path)}")
            for child in quest_children:
                if isinstance(child, dict) and len(child) == 1:
                    child_key, child_value = next(iter(child.items()))
                    if isinstance(child_value, str):
                        lines.append(
                            f"          - {_format_yaml_scalar(str(child_key))}: {_format_yaml_scalar(child_value)}"
                        )
            continue

        lines.extend(_render_nav_item(str(key), value, indent=6))

    return "\n".join(lines) + "\n"


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


def generate_quest_readme_index(argv: list[str] | None = None) -> int:
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


def update_mkdocs_index(argv: list[str] | None = None) -> int:
    """Update only `nav` -> `quest():` campaign subsubsections in `mkdocs.yml`.

    Rules:
    - Consider only entries under top-level `quest():`.
    - From those, only entries with path starting `auto/campaign_*`.
    - Modify only their subsubsection navigation.
    - Build quest subsubsections using `Quest.list()` filtered by campaign tag,
      sorted by tag index.
    - Render campaign intro as `- Intro: auto/campaign_<tag>.md`.

    The function preserves exact file content outside the `- quest():` block by
    replacing only that text span.
    """

    _ = argv  # kept for CLI symmetry

    mkdocs_path = DEFAULT_MKDOCS_PATH
    if not mkdocs_path.exists():
        print(f"Error: mkdocs file not found: {_display_path(mkdocs_path)}", file=sys.stderr)
        return 1

    original_text = mkdocs_path.read_text(encoding="utf-8")
    lines = original_text.splitlines(keepends=True)

    block_range = _find_quest_block_range(lines)
    if block_range is None:
        print(
            f"Error: missing `nav`/`- {QUEST_NAV_LABEL}` block in {_display_path(mkdocs_path)}",
            file=sys.stderr,
        )
        return 1

    try:
        config = yaml.safe_load(original_text) or {}
    except yaml.YAMLError as error:
        print(f"Error parsing YAML in {_display_path(mkdocs_path)}: {error}", file=sys.stderr)
        return 1

    nav = config.get("nav")
    if nav is None:
        print(f"Error: missing 'nav' in {_display_path(mkdocs_path)}", file=sys.stderr)
        return 1

    for item, label, value in _iter_quest_campaign_nodes(nav):
        campaign_path = _campaign_nav_path(value)
        if campaign_path is None:
            continue

        tag_name = _extract_campaign_tag(campaign_path)
        if tag_name is None:
            continue

        children = _build_campaign_children(tag_name)
        item[label] = [campaign_path, *children]

    quest_items: list[object] | None = None
    for top in nav:
        if isinstance(top, dict) and QUEST_NAV_KEY in top and isinstance(top[QUEST_NAV_KEY], list):
            quest_items = top[QUEST_NAV_KEY]
            break

    if quest_items is None:
        print(f"Error: missing `{QUEST_NAV_LABEL}` in {_display_path(mkdocs_path)}", file=sys.stderr)
        return 1

    new_block = _render_quest_nav_block(quest_items)
    start, end = block_range
    updated_text = "".join(lines[:start]) + new_block + "".join(lines[end:])

    mkdocs_path.write_text(updated_text, encoding="utf-8")
    print(f"Autoindex {_display_path(mkdocs_path)} updated")
    return 0


def main(argv: list[str] | None = None) -> int:
    """Generate README index and update MkDocs nav."""

    readme_status = generate_quest_readme_index(argv)
    mkdocs_status = update_mkdocs_index(argv)
    return 1 if (readme_status != 0 or mkdocs_status != 0) else 0


if __name__ == "__main__":
    raise SystemExit(main())
