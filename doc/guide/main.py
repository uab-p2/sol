# macro commands invoked when building the guide
import subprocess

import os
import sys
from pathlib import Path

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from tool.tag import PROJECT_ROOT, DEFAULT_QUEST_DIR, list_tags, list_metadata_for_tag

GITHUB_QUEST_URL = "https://github.com/uab-p2/sol/tree/main/quest"


def define_env(env):
    @env.macro
    def cmd(command):
        """Show the output of any shell command"""
        return subprocess.check_output(command, shell=True, text=True).strip()

    @env.macro
    def list_quests(tag_name):
        """List the quests of a given tag (campaign)."""
        lines = []
        for quest_metadata in list_metadata_for_tag(DEFAULT_QUEST_DIR, tag_name):
            lines.append("* [" + quest_metadata.title + "](" + GITHUB_QUEST_URL + "/" + os.path.basename(
                quest_metadata.module_path.resolve().as_posix()) + ")")

        return "\n".join(lines)
