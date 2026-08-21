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

    function showDefinition(key) {
        const source = document.querySelector(
            '[data-definition-source="' + CSS.escape(key) + '"]');
        if (!source) return;
        currentKey = key;
        Popup.open(source.innerHTML);
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
