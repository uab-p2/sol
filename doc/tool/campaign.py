#!/usr/bin/env python3
"""Automatic generation of campaign documentation pages for all
tags with indices defined in the quest directory.

Invoked automatically from the guide Makefile."""

from __future__ import annotations

import os
import re
import textwrap

from quest import Tag, Quest, GUIDE_AUTO_SECTION_DIR, GITHUB_QUEST_URL


def generate_campaign_sections():
    for tag in Tag.list():
        with open(GUIDE_AUTO_SECTION_DIR / f"campaign_{tag.name}.md", "w") as f:
            f.write(textwrap.dedent(f"""\
            # {{{{ tag_title("{tag.name}") }}}}
            
            {{{{ tag_description("{tag.name}") }}}}
            
            ## Quests
            
            A continuación tienes los quests de la campaña `{{{{ tag_title("{tag.name}") }}}}`:
            
            {{{{ tag_quest_sections("{tag.name}") }}}}
            
            """))


def generate_quest_sections():
    for quest in Quest.list():
        description_fixed_links = re.sub(
            r"\]\(\.\./\.\./doc/img/?",
            "](../asset/img/import/",
            quest.description,
        )

        with open(GUIDE_AUTO_SECTION_DIR / f"quest_{os.path.basename(quest.module_path)}.md", "w") as f:
            f.write(textwrap.dedent(f"""\
# {quest.title}

{description_fixed_links}

---
- [Código]({GITHUB_QUEST_URL}/{os.path.basename(quest.module_path)})
- [&leftarrow;&nbsp;{quest.tags[0].title}](campaign_{quest.tags[0].name}.md)
            """))


def main():
    generate_campaign_sections()
    generate_quest_sections()


if __name__ == '__main__':
    raise SystemExit(main())
