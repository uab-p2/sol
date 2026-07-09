from __future__ import annotations

import os
import subprocess
import sys
import textwrap
from io import StringIO

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from tool.quest import PROJECT_ROOT, DEFAULT_QUEST_DIR, GITHUB_ROOT_URL, GITHUB_QUEST_URL, GUIDE_SECTION_DIR, \
    GUIDE_AUTO_SECTION_DIR, Quest, Tag


def define_env(env):
    @env.macro
    def codex_link(name: str) -> str:
        """Get the link to the codex entry with the given name."""
        from tool.codex import Codex
        codex: Codex = next(c for c in Codex.list() if c.name == name)
        site_url = (env.conf.get("site_url") or "").rstrip("/")
        return f"[{codex.title}]({site_url}/codex/{codex.name})"

    @env.macro
    def codex_list() -> str:
        """Get an index of codices sorted by title."""
        from tool.codex import Codex
        codices = sorted(Codex.list(), key=lambda c: c.title.lower())
        return "\n".join(f"- {codex_link(codex.name)}" for codex in codices)

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
        """List the quests of a given tag (campaign) with links to GITHUB."""
        quests = sorted([quest for quest in Quest.list() if any(tag.name == tag_name for tag in quest.tags)],
                        key=lambda q: next(t for t in q.tags if t.name == tag_name).index)

        lines = []
        for quest in quests:
            lines.append("### [" + quest.title + "](" + GITHUB_QUEST_URL + "/" + os.path.basename(
                quest.module_path.resolve().as_posix()) + ")")
            lines.append(quest.description.strip().split("\n\n")[0])
            lines.append("")
        return "\n".join(lines)

    @env.macro
    def tag_quest_sections(tag_name):
        """List the quests of a given tag (campaign) with links to the quest sections."""
        quests = sorted([quest for quest in Quest.list() if any(tag.name == tag_name for tag in quest.tags)],
                        key=lambda q: next(t for t in q.tags if t.name == tag_name).index or 0)
        lines = []
        for quest in quests:
            lines.append(f"### [{quest.title}](quest_{os.path.basename(quest.module_path.resolve().as_posix())}.md)")
            lines.append(quest.description.strip().split("\n\n")[0])
        return "\n".join(lines)

    @env.macro
    def tag_quest_links(tag_name):
        """List the links to the quests of a given tag (campaign)."""
        try:
            quests = [quest for quest in Quest.list() if any(tag.name == tag_name for tag in quest.tags)]
            lines = []
            for quest in quests:
                lines.append(quest_link(quest))
            return "\n".join(lines)
        except ValueError:
            return f"(no hay quests para `{tag_name}`)"

    @env.macro
    def quest_link(quest_name):
        """Get the link to the quest with the given name."""
        try:
            quest = [quest for quest in Quest.list() if quest.name.lower() == quest_name.lower()][0]
            return f"[{quest.title}](quest_{quest.name}.md)"
        except IndexError:
            return f"(missing quest `{quest_name}`)\n\n{s}"

    @env.macro
    def snippet(name: str,
                arg_types: list[str] | None = None,
                include_declarations: bool = True,
                include_definitions: bool = True) -> str:
        """Display a snippet of code in its ```cpp``` block.
        :return: the rendered snippet(s) for the element with the given name.
          If more than one snippet matches, all are rendered.
        :param name: the name of the snippet, e.g., "Cat", "Cat::meow", "free_func".
        :param arg_types: optional list of argument type names, e.g., ["int", "std::string"].
          If present, only those snippets matching the given argument types
          (and name) are rendered."""
        sio = StringIO()
        snippets = find_snippets(name, arg_types, include_declarations, include_definitions)
        sio.write("\n\n".join(str(s) for s in snippets))
        if not snippets:
            sio.write(f"```cpp linenum\n\n"
                      f"// Missing snippet for {name!r}\n"
                      f"```")
        return sio.getvalue()

    @env.macro
    def snippet_ref(name: str,
                    arg_types: list[str] | None = None,
                    include_declarations: bool = True,
                    include_definitions: bool = True) -> str:
        """Get the pretty-printed reference to a snippet,
        including types if available."""
        s = find_snippet(name, arg_types, include_declarations, include_definitions)
        if s is not None:
            return s.label
        return f"`Warning: missing snippet for {name!r}`"

    @env.macro
    def snippet_src(name: str,
                    arg_types: list[str] | None = None,
                    include_declarations: bool = True,
                    include_definitions: bool = True) -> str:
        """Get the link to the source code of a snippet,
        with its link to the code repository."""
        s = find_snippet(name, arg_types, include_declarations, include_definitions)
        if s is not None:
            return (f"[{s.relative_file_path}"
                    f":{s.line_start}-{s.line_end}]"
                    f"({GITHUB_ROOT_URL}"
                    f"/{s.relative_file_path.as_posix()}"
                    f"#L{s.line_start}-L{s.line_end})")
        return f"`Warning: missing snippet for {name!r}`"

    @env.macro
    def snippet_tag(name: str,
                    arg_types: list[str] | None = None,
                    include_declarations: bool = True,
                    include_definitions: bool = True) -> str:
        """Get the pretty-printed reference to a snippet, name @ source"""
        return (f"{snippet_ref(name, arg_types, include_declarations, include_definitions)} "
                f"@ {snippet_src(name, arg_types, include_declarations, include_definitions)}")

    @env.macro
    def snippet_box(name: str,
                    arg_types: list[str] | None = None,
                    include_declarations: bool = True,
                    include_definitions: bool = True,
                    default_open: bool = False) -> str:
        """Show a box with title equal to the return of snippet_tag,
        with the snippet contents rendered inside."""
        s = find_snippet(name, arg_types, include_declarations, include_definitions)
        if s is not None:
            return (f"""\
???{'+' if default_open else ''} tip "{snippet_tag(name, arg_types, include_declarations, include_definitions)}"
{textwrap.indent(snippet(name, arg_types, include_declarations, include_definitions), "    ")}\n""")
        return f"`Warning: missing snippet for {name!r}`"


def find_snippets(name: str,
                  arg_types: list[str] | None = None,
                  include_declarations: bool = True,
                  include_definitions: bool = True) -> list[Snippet]:
    """Find all snippets, optionally:
        - with the given name and argument types
        - excluding callable declarations/definitions.
    """
    from tool.code import Snippet, SnippetType

    snippets = [s for s in Snippet.list()
                if s.name == name
                and (arg_types is None or s.arg_types == arg_types)]

    if not include_declarations:
        snippets = [s for s in snippets if s.type != SnippetType.DECLARATION]
    if not include_definitions:
        snippets = [s for s in snippets if s.type != SnippetType.DEFINITION]

    return snippets


def find_snippet(name: str,
                 arg_types: list[str] | None = None,
                 include_declarations: bool = True,
                 include_definitions: bool = True) -> Snippet | None:
    """Find the first snippet matching the given name and argument types,
    or None if no such snippet exists."""
    snippets = find_snippets(name, arg_types, include_declarations, include_definitions)
    return snippets[0] if snippets else None
