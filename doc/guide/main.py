from __future__ import annotations

import os
import posixpath
import subprocess
import sys
import textwrap
from io import StringIO
from difflib import unified_diff

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from tool.quest import (
    PROJECT_ROOT, DEFAULT_QUEST_DIR, GITHUB_ROOT_URL, GITHUB_SOLUTION_URL,
    GITHUB_QUEST_URL, GUIDE_SECTION_DIR, GUIDE_AUTO_SECTION_DIR, Quest, Tag)
from tool.code import Snippet, SnippetType


def define_env(env):
    @env.macro
    def codex_link(name: str) -> str:
        """Get the link to the codex entry with the given name."""
        from tool.codex import Codex
        try:
            codex: Codex = next(c for c in Codex.list() if c.name == name)
            return f"<a class='codex' href='{env.conf.site_url}codex/{codex.name}'>{codex.title}</a>"
        except StopIteration:
            return ""

    @env.macro
    def codex_list() -> str:
        """Get an index of codices sorted by title."""
        from tool.codex import Codex
        codices = sorted(Codex.list(), key=lambda c: c.title.lower())
        return "\n".join(f"- {codex_link(codex.name)}" for codex in codices)

    @env.macro
    def img(img_name: str):
        return f"""<img src="{env.conf['site_url']}asset/img/{img_name}">"""

    @env.macro
    def img_badge(img_name: str, framed: bool = True, div_class="badge") -> str:
        """Display a centered badge image within the given div class"""
        return textwrap.dedent(f"""\
                <div class="{div_class}{' frame' if framed else ''}">
                <img src="{env.conf['site_url']}asset/img/{img_name}">
                </div>
                """)

    @env.macro
    def tag_title(tag_name):
        try:
            tag: Tag = next(t for t in Tag.list() if t.name == tag_name)
            return tag.title
        except StopIteration:
            return ""

    @env.macro
    def tag_description(tag_name):
        try:
            tag: Tag = next(t for t in Tag.list() if t.name == tag_name)
            return env.render(tag.description)
        except StopIteration:
            return ""

    @env.macro
    def tag_quests(tag_name):
        """List the quests of a given tag (campaign) with links to GITHUB."""
        quests = sorted([quest for quest in Quest.list() if any(tag.name == tag_name for tag in quest.tags)],
                        key=lambda q: next(t for t in q.tags if t.name == tag_name).index)
        lines = []
        for quest in quests:
            lines.append(f"### {quest_link(quest.name)}")
            lines.append(quest.description.strip().split("\n\n")[0])
            lines.append("")
        return env.render("\n".join(lines))

    @env.macro
    def tag_quest_sections(tag_name):
        """List the quests of a given tag (campaign) with links to the quest sections."""
        quests = sorted([quest for quest in Quest.list() if any(tag.name == tag_name for tag in quest.tags)],
                        key=lambda q: next(t for t in q.tags if t.name == tag_name).index or 0)
        lines = []
        for quest in quests:
            lines.append(
                f"### {quest_link(quest.name)}")  # [{quest.title}](quest_{os.path.basename(quest.module_path.resolve().as_posix())}.md)")
            lines.append(quest.description.strip().split("\n\n")[0])
        return env.render("\n".join(lines))

    @env.macro
    def tag_quest_links(tag_name):
        """List the links to the quests of a given tag (campaign)."""
        try:
            quests = [quest for quest in Quest.list() if any(tag.name == tag_name for tag in quest.tags)]
            lines = []
            for quest in quests:
                lines.append(quest_link(quest))
            return env.render("\n".join(lines))
        except ValueError:
            return f"(no hay quests para `{tag_name}`)"

    @env.macro
    def quest_link(quest_name):
        """Get the link to the quest with the given name."""
        try:
            quest: Quest = next(quest for quest in Quest.list() if quest.name.lower() == quest_name.lower())
            return f"<a class='quest' href='{env.conf.site_url}auto/quest_{quest.name}'>{quest.title}</a>"
        except StopIteration:
            return f"(missing quest `{quest_name}`)\n\n{s}"

    @env.macro
    def quest_title(quest_name):
        """Get the title of the quest with the given name."""
        try:
            quest: Quest = next(quest for quest in Quest.list() if quest.name.lower() == quest_name.lower())
            return quest.title
        except StopIteration:
            return f"(missing quest `{quest_name}`)\n\n{s}"

    @env.macro
    def snippet(name: str,
                arg_types: list[str] | None = None,
                include_declarations: bool = False,
                include_definitions: bool = True,
                snippet: Snippet | None = None) -> str:
        """Display a snippet of code in its ```cpp``` block.
        :return: the rendered snippet(s) for the element with the given name.
          If more than one snippet matches, all are rendered.
        :param name: the name of the snippet, e.g., "Cat", "Cat::meow", "free_func".
        :param arg_types: optional list of argument type names, e.g., ["int", "std::string"].
          If present, only those snippets matching the given argument types
          (and name) are rendered."""
        sio = StringIO()
        if snippet is None:
            snippets = find_snippets(name, arg_types, include_declarations, include_definitions)
        else:
            snippets = [snippet]
        sio.write("\n\n".join(str(s) for s in snippets))
        if not snippets:
            sio.write(f"```cpp\n"
                      f"// Missing snippet for {name!r}\n"
                      f"```")
        return env.render(sio.getvalue())

    @env.macro
    def snippet_ref(name: str,
                    arg_types: list[str] | None = None,
                    include_declarations: bool = False,
                    include_definitions: bool = True,
                    snippet: Snippet | None = None) -> str:
        """Get the pretty-printed reference to a snippet,
        including types if available."""
        if snippet is None:
            url = env.page.url
            if url[-1] == "/":
                url = url[:-1]
            priority_dir = os.path.basename(url)
            snippet = find_snippet(name, arg_types, include_declarations, include_definitions, priority_dir)
        if snippet is not None:
            return snippet.label
        return f"`Warning: missing snippet for {name!r}`"

    @env.macro
    def snippet_src(name: str,
                    arg_types: list[str] | None = None,
                    include_declarations: bool = False,
                    include_definitions: bool = True,
                    snippet: Snippet | None = None) -> str:
        """Get the link to the source code of a snippet,
        with its link to the code repository."""
        if snippet is None:
            url = env.page.url
            if url[-1] == "/":
                url = url[:-1]
            priority_dir = os.path.basename(url)
            snippet = find_snippet(name, arg_types, include_declarations, include_definitions, priority_dir)
        if snippet is not None:
            return (f"[{snippet.relative_file_path}"
                    f":{snippet.line_start}-{snippet.line_end}]"
                    f"({GITHUB_ROOT_URL}"
                    f"/{snippet.relative_file_path.as_posix()}"
                    f"#L{snippet.line_start}-L{snippet.line_end})")
        return f"`Warning: missing snippet for {name!r}`"

    @env.macro
    def snippet_tag(name: str,
                    arg_types: list[str] | None = None,
                    include_declarations: bool = False,
                    include_definitions: bool = True,
                    snippet: Snippet | None = None) -> str:
        """Get the pretty-printed reference to a snippet, name @ source"""
        return (f"{snippet_ref(name, arg_types, include_declarations, include_definitions, snippet)} "
                f"@ {snippet_src(name, arg_types, include_declarations, include_definitions, snippet)}")

    @env.macro
    def snippet_box(name: str,
                    arg_types: list[str] | None = None,
                    include_declarations: bool = False,
                    include_definitions: bool = True,
                    default_open: bool = False,
                    snippet: Snippet | None = None) -> str:
        """Show a box with title equal to the return of snippet_tag,
        with the snippet contents rendered inside.
        If `search_quest_only` is True, only snippets in the quests are considered.
        """
        if snippet is None:
            url = env.page.url
            if url[-1] == "/":
                url = url[:-1]
            priority_dir = os.path.basename(url)
            snippet = find_snippet(name, arg_types, include_declarations, include_definitions, priority_dir)
        if snippet is not None:
            return env.render(f"""\
???{'+' if default_open else ''} tip "{snippet_tag(name, arg_types, include_declarations, include_definitions, snippet)}"
{textwrap.indent(str(snippet), "    ")}\n""")
        return f"`Warning: missing snippet for {name!r}`"

    @env.macro
    def snippet_diff_box(name: str, default_open: bool = True):
        """Within a quest solution, show the code differences between the
        original and solution for a given snippet."""
        url = env.page.url
        if url[-1] == "/":
            url = url[:-1]
        solution_dir = os.path.basename(url)
        if not "solution_" in solution_dir:
            return ("Cannot show diff box for `{name}`. Ensure `solution_` "
                    "is in the current directory name.")
        original_dir = solution_dir.replace("solution_", "")

        original_snippet = find_snippet(name, priority_dir=original_dir)
        solution_snippet = find_snippet(name, priority_dir=solution_dir)

        diff = "\n".join(unified_diff(original_snippet.code.splitlines(keepends=True),
                                      solution_snippet.code.splitlines(keepends=True)))

        return textwrap.dedent(f"""
??? diff "Cambios en {snippet_tag(name=name, snippet=solution_snippet)}"
    ```diff
    {textwrap.indent(diff, "    ")}
    ```""")


def find_snippets(name: str,
                  arg_types: list[str] | None = None,
                  include_declarations: bool = False,
                  include_definitions: bool = True,
                  priority_dir: string | None = None) -> list[Snippet]:
    """Find all snippets, optionally:
        - with the given name and argument types.
        - excluding callable declarations/definitions.
        - if more than one snippet matches, only those within the given `priority_dir`.
    """
    from tool.code import Snippet, SnippetType

    snippets = [s for s in Snippet.list()
                if s.name == name
                and (arg_types is None or s.arg_types == arg_types)]

    if not include_declarations:
        snippets = [s for s in snippets if s.type != SnippetType.DECLARATION]
    if not include_definitions and len(snippets):
        snippets = [s for s in snippets if s.type != SnippetType.DEFINITION]
    
    if priority_dir and len(snippets) > 1:
        filtered_snippets = []
        for s in snippets:
            if priority_dir.startswith("quest_"):
                priority_dir = priority_dir[len("quest_"):]
            snippet_dir_name = os.path.basename(os.path.dirname(s.relative_file_path))
            snippet_parent_dir = os.path.basename(os.path.dirname(os.path.dirname(s.relative_file_path)))
            if snippet_dir_name == priority_dir:                
                filtered_snippets.append(s)
            elif (snippet_parent_dir == "solution"
                  and priority_dir == f"solution_{snippet_dir_name}"):
                filtered_snippets.append(s)

        snippets = filtered_snippets
    

    return snippets


def find_snippet(name: str,
                 arg_types: list[str] | None = None,
                 include_declarations: bool = False,
                 include_definitions: bool = True,
                 priority_dir: string | None = None) -> Snippet | None:
    """Find the first snippet matching the given name and argument types,
    or None if no such snippet exists.
    If multiple snippets match, the one within `priority_dir` is returned."""
    snippets = find_snippets(
        name, arg_types, include_declarations, include_definitions, priority_dir) or []
    return snippets[0] if snippets else None
