/**
 * Generic single-instance popup: a centered panel over a backdrop, shown
 * with arbitrary HTML via Popup.open(html) and dismissed via Popup.close(),
 * the close button, a backdrop click, or Escape. Knows nothing about what
 * content it shows — features (see definitions.js) resolve their own
 * content and call into this.
 */
window.Popup = (function () {
    let backdrop = null;

    function ensureBackdrop() {
        if (backdrop) return backdrop;
        backdrop = document.createElement("div");
        backdrop.id = "popup-backdrop";
        backdrop.hidden = true;
        backdrop.innerHTML =
            '<div id="popup" class="md-typeset" role="dialog" aria-modal="true">' +
            '<button type="button" class="popup-close" aria-label="Cerrar">&times;</button>' +
            '<div class="popup-body"></div>' +
            "</div>";
        document.body.appendChild(backdrop);
        return backdrop;
    }

    function isOpen() {
        return !!backdrop && !backdrop.hidden;
    }

    function open(html) {
        const el = ensureBackdrop();
        const body = el.querySelector(".popup-body");
        body.innerHTML = html;
        el.hidden = false;
        document.body.classList.add("popup-open");
        el.querySelector(".popup-close").focus();
        renderMermaidDiagrams(body);
    }

    let mermaidCounter = 0;
    let mermaidInitialized = false;

    // Same themeCSS Material's own bundle passes to mermaid.initialize()
    // (bridges mermaid's generated classes to Material's --md-mermaid-*
    // custom properties, so colors follow the site's palette and light/dark
    // scheme) — copied here since Material never runs this itself on a page
    // whose only mermaid content lives in a popup (nothing for its own
    // `pre.mermaid` scan to find), and mermaid falls back to its own
    // default (mismatched) theme otherwise.
    const MERMAID_THEME_CSS =
        ".node circle,.node ellipse,.node path,.node polygon,.node rect{fill:var(--md-mermaid-node-bg-color);stroke:var(--md-mermaid-node-fg-color)}marker{fill:var(--md-mermaid-edge-color)!important}.edgeLabel .label rect{fill:#0000}.flowchartTitleText{fill:var(--md-mermaid-label-fg-color)}.label{color:var(--md-mermaid-label-fg-color);font-family:var(--md-mermaid-font-family)}.label foreignObject{line-height:normal;overflow:visible}.label div .edgeLabel{color:var(--md-mermaid-label-fg-color)}.edgeLabel,.edgeLabel p,.label div .edgeLabel{background-color:var(--md-mermaid-label-bg-color)}.edgeLabel,.edgeLabel p{fill:var(--md-mermaid-label-bg-color);color:var(--md-mermaid-edge-color)}.edgePath .path,.flowchart-link{stroke:var(--md-mermaid-edge-color)}.edgePath .arrowheadPath{fill:var(--md-mermaid-edge-color);stroke:none}.cluster rect{fill:var(--md-default-fg-color--lightest);stroke:var(--md-default-fg-color--lighter)}.cluster span{color:var(--md-mermaid-label-fg-color);font-family:var(--md-mermaid-font-family)}g #flowchart-circleEnd,g #flowchart-circleStart,g #flowchart-crossEnd,g #flowchart-crossStart,g #flowchart-pointEnd,g #flowchart-pointStart{stroke:none}.classDiagramTitleText{fill:var(--md-mermaid-label-fg-color)}g.classGroup line,g.classGroup rect{fill:var(--md-mermaid-node-bg-color);stroke:var(--md-mermaid-node-fg-color)}g.classGroup text{fill:var(--md-mermaid-label-fg-color);font-family:var(--md-mermaid-font-family)}.classLabel .box{fill:var(--md-mermaid-label-bg-color);background-color:var(--md-mermaid-label-bg-color);opacity:1}.classLabel .label{fill:var(--md-mermaid-label-fg-color);font-family:var(--md-mermaid-font-family)}.node .divider{stroke:var(--md-mermaid-node-fg-color)}.relation{stroke:var(--md-mermaid-edge-color)}.cardinality{fill:var(--md-mermaid-label-fg-color);font-family:var(--md-mermaid-font-family)}.cardinality text{fill:inherit!important}defs marker.marker.composition.class path,defs marker.marker.dependency.class path,defs marker.marker.extension.class path{fill:var(--md-mermaid-edge-color)!important;stroke:var(--md-mermaid-edge-color)!important}defs marker.marker.aggregation.class path{fill:var(--md-mermaid-label-bg-color)!important;stroke:var(--md-mermaid-edge-color)!important}.statediagramTitleText{fill:var(--md-mermaid-label-fg-color)}g.stateGroup rect{fill:var(--md-mermaid-node-bg-color);stroke:var(--md-mermaid-node-fg-color)}g.stateGroup .state-title{fill:var(--md-mermaid-label-fg-color)!important;font-family:var(--md-mermaid-font-family)}g.stateGroup .composit{fill:var(--md-mermaid-label-bg-color)}.nodeLabel,.nodeLabel p{color:var(--md-mermaid-label-fg-color);font-family:var(--md-mermaid-font-family)}a .nodeLabel{text-decoration:underline}.node circle.state-end,.node circle.state-start,.start-state{fill:var(--md-mermaid-edge-color);stroke:none}.end-state-inner,.end-state-outer{fill:var(--md-mermaid-edge-color)}.end-state-inner,.node circle.state-end{stroke:var(--md-mermaid-label-bg-color)}.transition{stroke:var(--md-mermaid-edge-color)}[id^=state-fork] rect,[id^=state-join] rect{fill:var(--md-mermaid-edge-color)!important;stroke:none!important}.statediagram-cluster.statediagram-cluster .inner{fill:var(--md-default-bg-color)}.statediagram-cluster rect{fill:var(--md-mermaid-node-bg-color);stroke:var(--md-mermaid-node-fg-color)}.statediagram-state rect.divider{fill:var(--md-default-fg-color--lightest);stroke:var(--md-default-fg-color--lighter)}defs #statediagram-barbEnd{stroke:var(--md-mermaid-edge-color)}[id^=entity] path,[id^=entity] rect{fill:var(--md-default-bg-color)}.relationshipLine{stroke:var(--md-mermaid-edge-color)}defs .marker.oneOrMore.er *,defs .marker.onlyOne.er *,defs .marker.zeroOrMore.er *,defs .marker.zeroOrOne.er *{stroke:var(--md-mermaid-edge-color)!important}text:not([class]):last-child{fill:var(--md-mermaid-label-fg-color)}.actor{fill:var(--md-mermaid-sequence-actor-bg-color);stroke:var(--md-mermaid-sequence-actor-border-color)}text.actor>tspan{fill:var(--md-mermaid-sequence-actor-fg-color);font-family:var(--md-mermaid-font-family)}line{stroke:var(--md-mermaid-sequence-actor-line-color)}.actor-man circle,.actor-man line{fill:var(--md-mermaid-sequence-actorman-bg-color);stroke:var(--md-mermaid-sequence-actorman-line-color)}.messageLine0,.messageLine1{stroke:var(--md-mermaid-sequence-message-line-color)}.note{fill:var(--md-mermaid-sequence-note-bg-color);stroke:var(--md-mermaid-sequence-note-border-color)}.loopText,.loopText>tspan,.messageText,.noteText>tspan{stroke:none;font-family:var(--md-mermaid-font-family)!important}.messageText{fill:var(--md-mermaid-sequence-message-fg-color)}.loopText,.loopText>tspan{fill:var(--md-mermaid-sequence-loop-fg-color)}.noteText>tspan{fill:var(--md-mermaid-sequence-note-fg-color)}#arrowhead path{fill:var(--md-mermaid-sequence-message-line-color);stroke:none}.loopLine{fill:var(--md-mermaid-sequence-loop-bg-color);stroke:var(--md-mermaid-sequence-loop-border-color)}.labelBox{fill:var(--md-mermaid-sequence-label-bg-color);stroke:none}.labelText,.labelText>span{fill:var(--md-mermaid-sequence-label-fg-color);font-family:var(--md-mermaid-font-family)}.sequenceNumber{fill:var(--md-mermaid-sequence-number-fg-color)}rect.rect{fill:var(--md-mermaid-sequence-box-bg-color);stroke:none}rect.rect+text.text{fill:var(--md-mermaid-sequence-box-fg-color)}defs #sequencenumber{fill:var(--md-mermaid-sequence-number-bg-color)!important}";

    function ensureMermaidInitialized() {
        if (mermaidInitialized) return;
        mermaidInitialized = true;
        mermaid.initialize({
            startOnLoad: false,
            themeCSS: MERMAID_THEME_CSS,
            sequence: {actorFontSize: "16px", messageFontSize: "16px", noteFontSize: "16px"}
        });
    }

    // Popup source divs keep mermaid blocks under a `mermaid-popup` class
    // instead of `mermaid` (see DefinitionMacros._namespace_popup_html) so
    // Material's own page-load scan (which finds `pre.mermaid` regardless
    // of visibility) never touches them: it replaces each one with a
    // closed-shadow-DOM render whose content is lost the moment this div's
    // innerHTML gets copied here. Render them ourselves instead — via
    // mermaid.render(id, text) directly (as Material itself does), not
    // mermaid.run(), which reads an element's innerHTML as the diagram
    // source and would choke on the <code> wrapper (or an injected
    // copy-button <nav>) markup that comes along with it.
    function renderMermaidDiagrams(root) {
        if (!window.mermaid) return;
        const nodes = root.querySelectorAll(".mermaid-popup");
        if (!nodes.length) return;
        ensureMermaidInitialized();
        for (const node of nodes) {
            const source = node.textContent;
            mermaid.render(`popup-mermaid-${mermaidCounter++}`, source).then(
                function (result) {
                    node.innerHTML = result.svg;
                    if (result.bindFunctions) result.bindFunctions(node);
                },
                function (error) {
                    console.error("mermaid render failed:", error);
                });
        }
    }

    function close() {
        if (!isOpen()) return;
        backdrop.hidden = true;
        document.body.classList.remove("popup-open");
    }

    document.addEventListener("click", function (event) {
        if (event.target.closest(".popup-close")) {
            close();
        } else if (backdrop && event.target === backdrop) {
            close();
        }
    });

    document.addEventListener("keydown", function (event) {
        if (event.key === "Escape") close();
    });

    return {open: open, close: close, isOpen: isOpen};
})();
