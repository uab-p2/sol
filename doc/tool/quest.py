"""
Process and display metainformation about quest modules.
"""
from __future__ import annotations

import glob
import os
import re
from dataclasses import dataclass
from pathlib import Path

try:
    from markdown import find_last_h1_named, join_section, read_markdown_lines, require_first_h1
except ImportError:  # pragma: no cover - import path when used as package `tool.quest`
    from tool.markdown import find_last_h1_named, join_section, read_markdown_lines, require_first_h1

__all__ = ["Quest"]

PROJECT_ROOT = Path(__file__).resolve().parents[2]
DEFAULT_QUEST_DIR = PROJECT_ROOT / "quest"
DEFAULT_TAG_DIR = PROJECT_ROOT / "doc" / "tag"
GUIDE_SECTION_DIR = PROJECT_ROOT / "doc" / "guide" / "sections"
GUIDE_AUTO_SECTION_DIR = GUIDE_SECTION_DIR / "auto"
GITHUB_ROOT_URL = "https://github.com/uab-p2/sol/tree/main/"
GITHUB_QUEST_URL = f"{GITHUB_ROOT_URL}/quest"


@dataclass(slots=True)
class Tag:
    """A tag reference, optionally enriched with metadata from `doc/tag/<name>.md`."""

    name: str
    index: int | None = None
    title: str = ""
    description: str = ""

    @classmethod
    def list(cls,
             root_dir: str | Path = DEFAULT_QUEST_DIR,
             tag_dir: str | Path = DEFAULT_TAG_DIR) -> list[Tag]:
        """Return unique tags in quests found directly under `root_dir`.
        Tags are enriched with metadata in `tag_dir` if available.
        Tags are returne sorted alphabetically."""
        root_dir = Path(root_dir)
        tag_dir = Path(tag_dir)

        quest_list = Quest.list(root_dir)
        unique_tags: dict[tuple[str, int | None], Tag] = {}
        for quest in quest_list:
            for tag in quest.tags:
                unique_tags.setdefault((tag.name, tag.index), tag)

        for tag in unique_tags.values():
            tag.populate(tag_dir=tag_dir)

        return sorted(unique_tags.values(),
                      key=lambda tag: (tag.name, -1 if tag.index is None else tag.index))

    def populate(self, tag_dir):
        """Populate the title and description from doc/tag"""
        try:
            lines = read_markdown_lines(Path(tag_dir) / f"{self.name}.md")
            if not lines:
                return
            title_heading = require_first_h1(lines, Path(tag_dir) / f"{self.name}.md")
            self.title = title_heading[1]
            description = join_section(lines, title_heading[0] + 1)
            self.description = description or ""
        except FileNotFoundError:
            pass

    def __lt__(self, other: Tag) -> bool:
        return self.name < other.name

    def __str__(self) -> str:
        return self.name if self.index is None else f"{self.name}:{self.index}"


@dataclass(slots=True)
class Quest:
    """Quest metainformation extracted from its `README.md` file."""

    module_path: Path
    title: str
    description: str
    tags: list[Tag]

    @property
    def name(self) -> str:
        return os.path.basename(self.module_path.resolve().as_posix())

    @classmethod
    def from_readme(cls, readme_path: Path) -> Quest:
        """Read the title, description, and tags from a README markdown file.

        The title is taken from the first level-1 heading (`# ...`). The
        description is everything between that heading and the last `# Tags`
        section, case-insensitively detected. The tags section is parsed as
        space- or comma-separated words where each tag may optionally use
        `:<number>` to encode an index.
        """
        readme_path = Path(readme_path)
        lines = read_markdown_lines(readme_path)

        title_index, title = require_first_h1(lines, readme_path)

        tags_index = find_last_h1_named(lines, "tags")
        if tags_index is None:
            raise ValueError(f"No tags section found in {readme_path}")

        description = join_section(lines, title_index + 1, tags_index)
        tag_text = join_section(lines, tags_index + 1)

        tags = cls._parse_tags(tag_text)
        tags = tags or None


        return cls(module_path=readme_path.parent, title=title, description=description, tags=tags)

    @classmethod
    def list(cls, base_dir: Path = DEFAULT_QUEST_DIR) -> list[Quest]:
        """:return: a list of all quests directly under the given directory"""
        return [Quest.from_readme(p)
                for p in glob.glob((base_dir / "*" / "README.md").resolve().as_posix(), recursive=False)]

        return metadata_list

    @classmethod
    def _parse_tags(cls, tag_text: str) -> list[Tag]:
        """Parse a tags section into `Tag` objects from space/comma-separated tokens."""

        tags: list[Tag] = []
        for raw_tag in (tag for tag in re.split(r"[\s,]+", tag_text.strip()) if tag):
            match = re.fullmatch(r"([^:,\s]+)(?::(\d+))?", raw_tag)
            if not match:
                raise ValueError(f"Invalid tag {raw_tag!r}")
            tags.append(Tag(name=match.group(1), index=int(match.group(2)) if match.group(2) is not None else None))
            tags[-1].populate(tag_dir=DEFAULT_TAG_DIR)
        return tags

    def __repr__(self) -> str:
        return f"{self.title} ({' '.join(str(tag) for tag in sorted(self.tags))})"
