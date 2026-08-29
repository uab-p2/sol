/**
 * Fixes classDiagram relation arrowheads (extension/composition/dependency/
 * aggregation) staying mermaid's default near-black (#333), unreadable
 * against the site's dark scheme, while every other mermaid color (nodes,
 * edges, sequence diagrams...) already follows --md-mermaid-*.
 *
 * Material's own themeCSS (duplicated in popup.js as MERMAID_THEME_CSS)
 * tries to recolor these via `marker.marker.extension.class path` etc.,
 * but the vendored mermaid.min.js in this repo doesn't put those classes
 * on the marker <path> — only a "...-extensionStart"/"...-extensionEnd" id
 * on the <marker> itself — so that rule silently never matches. Both
 * mermaid.initialize() call sites (Material's own bundle, for on-page
 * diagrams, and popup.js, for popup ones) end up with the same broken
 * themeCSS, so patch the one function they both funnel through instead of
 * fixing each call site: whatever themeCSS is passed gets these id-based
 * rules appended, landing after (and so, tie-broken in favor of) mermaid's
 * own injected marker styles in the generated SVG's <style>.
 */
(function () {
    const MARKER_FIX_CSS =
        'marker[id$="-extensionStart"] path,marker[id$="-extensionEnd"] path,' +
        'marker[id$="-compositionStart"] path,marker[id$="-compositionEnd"] path,' +
        'marker[id$="-dependencyStart"] path,marker[id$="-dependencyEnd"] path,' +
        'marker[id$="-aggregationStart"] path,marker[id$="-aggregationEnd"] path' +
        '{stroke:var(--md-mermaid-edge-color)!important}' +
        'marker[id$="-extensionStart"] path,marker[id$="-extensionEnd"] path,' +
        'marker[id$="-compositionStart"] path,marker[id$="-compositionEnd"] path,' +
        'marker[id$="-dependencyStart"] path,marker[id$="-dependencyEnd"] path' +
        '{fill:var(--md-mermaid-edge-color)!important}' +
        'marker[id$="-aggregationStart"] path,marker[id$="-aggregationEnd"] path' +
        '{fill:var(--md-mermaid-label-bg-color)!important}';

    if (!window.mermaid) return;
    const originalInitialize = mermaid.initialize.bind(mermaid);
    mermaid.initialize = function (config) {
        config = Object.assign({}, config, {
            themeCSS: (config && config.themeCSS ? config.themeCSS : "") + MARKER_FIX_CSS
        });
        return originalInitialize(config);
    };
})();
