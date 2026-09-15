/**
 * Click-to-toggle popups for glossary term references ({{ def(...) }}, see
 * DefinitionMacros.def_ in luna-plugin/luna/macro.py), built on the shared
 * window.Popup (see popup.js). Each reference renders as a small
 * <a class="definition" data-definition="key"> trigger; somewhere else on
 * the page sits a hidden <div data-definition-source="key"> holding that
 * term's full rendered content, emitted once per key per page.
 */
(function () {
    if (window.__definitionsPopupInstalled) return;
    window.__definitionsPopupInstalled = true;

    let currentKey = null;
    let tabbedCounter = 0;

    // Tabbed content (pymdownx.tabbed) is plain radio inputs/labels wired
    // by id/name (`__tabbed_1_1`/`__tabbed_1`, ...), selected via CSS
    // sibling selectors off :checked. Two things break once the source
    // div's markup is copied into the popup:
    //
    // 1. The source div stays in the DOM (hidden), so without renaming,
    //    the popup's copy shares those ids/names with the original:
    //    clicking a tab's <label for="__tabbed_1_2"> resolves to the
    //    *first* matching id in the document (the hidden original, via
    //    getElementById), so the popup's own radios never get checked
    //    and every panel disappears.
    //
    // 2. Material's own JS (content.tabs) runs on every `.tabbed-set` at
    //    page load, hidden ones included, and wraps each label's text in
    //    an `<a href="#id">` for smooth-scroll/tab-linking, bound only to
    //    those original elements. cloneNode copies that anchor along with
    //    it; clicking it then just follows the (stale) link instead of
    //    activating the label, so a tab click does nothing at all. Undo
    //    the wrap so plain label-for activation (native, no JS) applies.
    //
    // 3. That same init adds a `tabbed-labels--linked` class, whose CSS
    //    zeroes out label padding (the "linked" look relies on Material's
    //    JS for the rest of its styling, which the popup never runs) --
    //    strip it too, so tabs keep their normal spacing/underline.
    function unwrapLinkedLabels(root) {
        root.querySelectorAll(".tabbed-labels").forEach(function (labels) {
            labels.classList.remove("tabbed-labels--linked");
        });
        root.querySelectorAll(".tabbed-labels > label > a[href]").forEach(function (anchor) {
            const label = anchor.parentElement;
            while (anchor.firstChild) label.insertBefore(anchor.firstChild, anchor);
            anchor.remove();
        });
    }

    function uniquifyTabs(root) {
        unwrapLinkedLabels(root);
        const suffix = "-popup" + (tabbedCounter++);
        root.querySelectorAll("input[id]").forEach(function (input) {
            const oldId = input.id;
            const newId = oldId + suffix;
            input.id = newId;
            root.querySelectorAll('label[for="' + CSS.escape(oldId) + '"]').forEach(
                function (label) {
                    label.setAttribute("for", newId);
                });
        });
        root.querySelectorAll("input[name]").forEach(function (input) {
            input.name = input.name + suffix;
        });
    }

    function showDefinition(key) {
        const source = document.querySelector(
            '[data-definition-source="' + CSS.escape(key) + '"]');
        if (!source) return;
        currentKey = key;
        const clone = source.cloneNode(true);
        uniquifyTabs(clone);
        Popup.open(clone.innerHTML);
    }

    document.addEventListener("click", function (event) {
        const trigger = event.target.closest("a.definition[data-definition]");
        if (!trigger) return;
        event.preventDefault();
        const key = trigger.dataset.definition;
        if (key === currentKey && Popup.isOpen()) {
            Popup.close();
            currentKey = null;
        } else {
            showDefinition(key);
        }
    });
})();
