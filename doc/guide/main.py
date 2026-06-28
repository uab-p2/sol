from __future__ import annotations

# macro commands invoked when building the guide
import subprocess

import os
import sys
from pathlib import Path
from io import StringIO

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from tool.quest import PROJECT_ROOT, DEFAULT_QUEST_DIR, Quest, Tag


GITHUB_QUEST_URL = "https://github.com/uab-p2/sol/tree/main/quest"


def define_env(env):
    @env.macro
    def cmd(command):
        """Show the output of any shell command"""
        return subprocess.check_output(command, shell=True, text=True).strip()

    @env.macro
    def tag_title(tag_name):
        try:
            tag = [t for t in Tag.list() if t.name == tag_name][0]
            return tag.title
        except IndexError:
            return ""

    @env.macro
    def tag_description(tag_name):
        try:
            tag = [t for t in Tag.list() if t.name == tag_name][0]
            return tag.description
        except IndexError:
            return ""

    @env.macro
    def tag_quests(tag_name):
        """List the quests of a given tag (campaign)."""
        quests = [quest for quest in Quest.list() if any(tag.name == tag_name for tag in quest.tags)]
        lines = []
        for quest in quests:
            lines.append("### [" + quest.title + "](" + GITHUB_QUEST_URL + "/" + os.path.basename(
                quest.module_path.resolve().as_posix()) + ")")
            lines.append(quest.description.strip().split("\n\n")[0])
            lines.append("")
        return "\n".join(lines)

    @env.macro
    def snippet(name: str, arg_types: list[str] | None = None):
        """
        :return: the rendered snippet(s) for the element with the given name.
          If more than one snippet matches, all are rendered.
        :param name: the name of the snippet, e.g., "Cat", "Cat::meow", "free_func".
        :param arg_types: optional list of argument type names, e.g., ["int", "std::string"].
          If present, only those snippets matching the given argument types
          (and name) are rendered."""
        # Lazy import because otherwise github pages cannot find clang
        from tool.code import Snippet

        sio = StringIO()
        found: bool = False

        snippets = Snippet.list()
        for snippet in snippets:
            if snippet.name == name:
                if arg_types is not None and snippet.arg_types != arg_types:
                    continue
                if found:
                    sio.write("\n")
                sio.write(str(snippet))
                found = True

        if not found:
            sio.write(f"```cpp\n\n"
                      f"// Missing snippet for {name!r}\n"
                      f"```")

        return sio.getvalue()
