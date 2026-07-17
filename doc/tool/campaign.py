#!/usr/bin/env python3
"""Automatic generation of campaign documentation pages for all
tags with indices defined in the quest directory.

Invoked automatically from the guide Makefile."""

from __future__ import annotations

import os
import re
import textwrap

from quest import (
    Tag, Quest, GUIDE_AUTO_SECTION_DIR, GITHUB_QUEST_URL, GITHUB_SOLUTION_URL)


def generate_campaign_sections():
    for tag in Tag.list():
        with open(GUIDE_AUTO_SECTION_DIR / f"campaign_{tag.name}.md", "w") as f:
            f.write(textwrap.dedent(f"""\
            # {{{{ tag_title("{tag.name}") }}}}
            
            {{{{ tag_description("{tag.name}") }}}}
            
            ## Quests
            
            A continuación tienes un resumen de los quests de la campaña `{{{{ tag_title("{tag.name}") }}}}`:
            
            {{{{ tag_quest_sections("{tag.name}") }}}}
            
            """))


def generate_quest_sections():
    solutions = Quest.solutions()

    for quest in Quest.list():
        solution = next((s for s in solutions
                         if os.path.basename(s.module_path) == os.path.basename(quest.module_path)), None)
        solution_link = ""
        if solution is not None:
            solution_link = f"[&rightarrow; Solución](solution_{os.path.basename(quest.module_path)}.md)<br/>"

        with open(GUIDE_AUTO_SECTION_DIR / f"quest_{os.path.basename(quest.module_path)}.md", "w") as f:
            f.write(textwrap.dedent(f"""\
# {quest.title}

{quest.description}

<br/><br/>

---

{solution_link}
[&rightarrow; Repositorio]({GITHUB_QUEST_URL}/{os.path.basename(quest.module_path)})
            """))


def generate_solution_sections():
    for quest in Quest.solutions():
        with open(GUIDE_AUTO_SECTION_DIR / f"solution_{os.path.basename(quest.module_path)}.md", "w") as f:
            f.write(textwrap.dedent(f"""\
# {quest.title}

{quest.description}

<br/><br/>

---

[&rightarrow; Repositorio]({GITHUB_SOLUTION_URL}/{os.path.basename(quest.module_path)})
            """))


def main():
    generate_campaign_sections()
    generate_quest_sections()
    generate_solution_sections()


if __name__ == '__main__':
    raise SystemExit(main())
