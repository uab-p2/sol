#include "plot.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>

/// Generic, project-agnostic SVG string-building: elements,
/// escaping, number formatting, file output. Knows nothing about
/// this project's own colors. See PlotStyle for those.
class SvgBuilder {
public:
    /// Escape the characters that are special inside SVG text content.
    static std::string escape_xml(const std::string& text);

    /// Format a number for display: as an integer when it is one
    /// (within rounding error), as a 2-decimal value otherwise, or
    /// in scientific notation when it is too small for 2 decimals to
    /// show anything but "0.00" (e.g. a tolerance like 1e-6), or too
    /// big for `float` to represent every integer near it exactly.
    static std::string format_number(float value);

    /// @return a rough estimate, in pixels, of how wide `text` will
    ///   render at `size`. No real font metrics are available here,
    ///   so this just assumes an average glyph width, good enough to
    ///   lay out a legend without it running off the canvas.
    static float text_width(const std::string& text, float size);

    /// @return a `<text>` element.
    static std::string text(float x, float y, const std::string& content,
                            const std::string& anchor, float size,
                            const std::string& color, float rotate_deg,
                            bool bold = false);

    /// @return a `<rect>` element, e.g. a box framing the legend.
    static std::string rect(float x, float y, float width, float height,
                            float corner_radius, const std::string& fill,
                            const std::string& stroke, float stroke_width);

    /// @return a `<line>` element.
    static std::string line(float x1, float y1, float x2, float y2,
                            const std::string& color, float width);

    /// @return the opening `<svg>...<rect>` boilerplate common to
    ///   every chart: a `width`x`height` canvas filled with
    ///   `background`.
    static std::string header(float width, float height, const std::string& background);

    /// Write `content` to `filename`, overwriting it if it exists.
    static void write_file(const std::string& filename, const std::string& content);
};

std::string SvgBuilder::escape_xml(const std::string& text) {
    std::string result;
    for (size_t i = 0; i < text.size(); i++) {
        char c = text[i];
        if (c == '&') {
            result += "&amp;";
        } else if (c == '<') {
            result += "&lt;";
        } else if (c == '>') {
            result += "&gt;";
        } else if (c == '"') {
            result += "&quot;";
        } else {
            result += c;
        }
    }
    return result;
}

std::string SvgBuilder::format_number(float value) {
    std::ostringstream stream;
    float magnitude = std::abs(value);
    float rounded = std::floor(value + 0.5f);
    if (value != 0 && (magnitude < 0.005f || magnitude >= 100000.0f)) {
        stream << std::scientific << std::setprecision(2) << value;
    } else if (std::abs(value - rounded) < 1e-4f) {
        stream << static_cast<long long>(rounded);
    } else {
        stream << std::fixed << std::setprecision(2) << value;
    }
    return stream.str();
}

float SvgBuilder::text_width(const std::string& text, float size) {
    return static_cast<float>(text.size()) * size * 0.56f;
}

std::string SvgBuilder::text(float x, float y, const std::string& content,
                             const std::string& anchor, float size,
                             const std::string& color, float rotate_deg,
                             bool bold) {
    std::string transform_attr;
    if (rotate_deg != 0) {
        transform_attr = " transform=\"rotate(" + format_number(rotate_deg) +
            " " + format_number(x) + " " + format_number(y) + ")\"";
    }
    std::string weight_attr = bold ? " font-weight=\"bold\"" : "";
    return "<text x=\"" + format_number(x) + "\" y=\"" + format_number(y) +
        "\" text-anchor=\"" + anchor + "\" font-size=\"" +
        format_number(size) + "\" fill=\"" + color + "\"" + weight_attr + transform_attr +
        ">" + escape_xml(content) + "</text>";
}

std::string SvgBuilder::rect(float x, float y, float width, float height,
                             float corner_radius, const std::string& fill,
                             const std::string& stroke, float stroke_width) {
    std::string stroke_attr = stroke.empty() ? "" :
        " stroke=\"" + stroke + "\" stroke-width=\"" + format_number(stroke_width) + "\"";
    return "<rect x=\"" + format_number(x) + "\" y=\"" + format_number(y) +
        "\" width=\"" + format_number(width) + "\" height=\"" + format_number(height) +
        "\" rx=\"" + format_number(corner_radius) + "\" fill=\"" + fill + "\"" + stroke_attr + "/>";
}

std::string SvgBuilder::line(float x1, float y1, float x2, float y2,
                             const std::string& color, float width) {
    return "<line x1=\"" + format_number(x1) + "\" y1=\"" +
        format_number(y1) + "\" x2=\"" + format_number(x2) + "\" y2=\"" +
        format_number(y2) + "\" stroke=\"" + color + "\" stroke-width=\"" +
        format_number(width) + "\"/>";
}

std::string SvgBuilder::header(float width, float height, const std::string& background) {
    return "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 " +
        format_number(width) + " " + format_number(height) + "\" width=\"" +
        format_number(width) + "\" height=\"" + format_number(height) +
        "\" font-family=\"Roboto, -apple-system, BlinkMacSystemFont, Helvetica, "
        "Arial, sans-serif\"><rect width=\"" +
        format_number(width) + "\" height=\"" + format_number(height) +
        "\" fill=\"" + background + "\"/>";
}

void SvgBuilder::write_file(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    file << content;
}

/// @return the text-anchor to use for a tick label centered at `px`,
///   so a label at the very edge of the canvas doesn't run past it:
///   "middle" ordinarily, "start"/"end" for a label whose centered
///   bounding box would clip the left/right edge, so it instead
///   grows inward from `px` on that side.
static std::string tick_label_anchor(float px, const std::string& label,
                                     float font_size, float canvas_width) {
    float half_width = SvgBuilder::text_width(label, font_size) / 2;
    std::string anchor = "middle";
    if (px - half_width < 2) {
        anchor = "start";
    } else if (px + half_width > canvas_width - 2) {
        anchor = "end";
    }
    return anchor;
}

/// This project's own visual identity, applied over plain SvgBuilder
/// elements: the color palette and the "glow" effect.
class PlotStyle {
public:
    /// @return a `<filter>` producing a soft colored halo behind
    ///   whatever it is applied to, e.g. `filter="url(#glow0)"` on
    ///   a `<polyline>` or a `<g>` wrapping several bars. Built from
    ///   feGaussianBlur/feFlood/feComposite/feMerge rather than the
    ///   (functionally equivalent) feDropShadow, since some SVG
    ///   renderers used to preview/export these charts (e.g.
    ///   Inkscape's librsvg) don't implement feDropShadow yet.
    static std::string glow_filter(const std::string& id, const std::string& color);

    /// The site's main accent orange first, then the purple used by
    /// the !!! example admonition, then the rest of the campaign
    /// palette (see macro.py's CalendarMacros._CAMPAIGN_COLORS) for a
    /// 3rd/4th curve.
    static const char* const PALETTE[4];
    static const char* const BACKGROUND;
    static const char* const AXIS_COLOR;
    static const char* const GRID_COLOR;
    static const char* const TEXT_COLOR;
    static const char* const ERROR_COLOR;
    static const char* const ERROR_BACKGROUND;
    static const char* const BOX_BACKGROUND;
    static const char* const BOX_BORDER;
};

std::string PlotStyle::glow_filter(const std::string& id, const std::string& color) {
    std::string source = "SourceGraphic";
    return "<filter id=\"" + id + "\" x=\"-30%\" y=\"-30%\" width=\"160%\" height=\"160%\">"
        "<feFlood flood-color=\"" + color + "\" flood-opacity=\"0.55\" result=\"glowColor\"/>"
        "<feComposite in=\"glowColor\" in2=\"" + source + "\" operator=\"in\" result=\"coloredSource\"/>"
        "<feGaussianBlur in=\"coloredSource\" stdDeviation=\"3\" result=\"blurred\"/>"
        "<feMerge><feMergeNode in=\"blurred\"/><feMergeNode in=\"" + source + "\"/></feMerge>"
        "</filter>";
}

const char* const PlotStyle::PALETTE[4] = {"#ff6e42", "#7c4dff", "#00b8d4", "#43a047"};
const char* const PlotStyle::BACKGROUND = "none";
const char* const PlotStyle::AXIS_COLOR = "#808080";
const char* const PlotStyle::GRID_COLOR = "#808080";
const char* const PlotStyle::TEXT_COLOR = "#808080";
const char* const PlotStyle::ERROR_COLOR = "#c0392b";
const char* const PlotStyle::ERROR_BACKGROUND = "#fdecea";
const char* const PlotStyle::BOX_BACKGROUND = "#f7f7f9";
const char* const PlotStyle::BOX_BORDER = "#dcdce0";

/// Vertical space draw_header() gives the title when there is one.
/// Subtracted back from the top margin when there isn't, so an
/// untitled chart doesn't carry a blank gap above its content.
static const float TITLE_HEIGHT = 36;

PlotBase::PlotBase(const std::string& filename, const std::string& title,
            float width, float height,
            float margin_left, float margin_right, float margin_top, float margin_bottom,
            float font_size)
    : m_filename(filename), m_title(title), m_width(width), m_height(height),
      m_margin_left(margin_left), m_margin_right(margin_right),
      m_margin_top(title.empty() ? margin_top - TITLE_HEIGHT : margin_top),
      m_margin_bottom(margin_bottom), m_font_size(font_size) {
}

float PlotBase::plot_left() const {
    return m_margin_left;
}

float PlotBase::plot_top() const {
    return m_margin_top;
}

float PlotBase::plot_width() const {
    return m_width - m_margin_left - m_margin_right;
}

float PlotBase::plot_height() const {
    return m_height - m_margin_top - m_margin_bottom;
}

std::string PlotBase::draw_header() const {
    std::string svg = SvgBuilder::header(m_width, m_height, PlotStyle::BACKGROUND);
    if (!m_title.empty()) {
        svg += SvgBuilder::text(m_width / 2, 30, m_title, "middle", m_font_size,
            PlotStyle::TEXT_COLOR, 0, true);
    }
    return svg;
}

void PlotBase::draw_axes(std::string& svg) const {
    float left = plot_left();
    float top = plot_top();
    float width = plot_width();
    float height = plot_height();
    svg += SvgBuilder::line(left, top, left, top + height, PlotStyle::AXIS_COLOR, 2);
    svg += SvgBuilder::line(left, top + height, left + width, top + height, PlotStyle::AXIS_COLOR, 2);
}

void PlotBase::write_svg(std::string svg) const {
    svg += "</svg>";
    SvgBuilder::write_file(m_filename, svg);
}

const float LinePlot::WIDTH = 640;
const float LinePlot::HEIGHT = 480;
const float LinePlot::MARGIN_LEFT = 140;
// tick_label_anchor() already keeps the rightmost x-tick label from
// clipping by switching its anchor near the edge, so this only needs
// to clear the y-axis label's own margin.
const float LinePlot::MARGIN_RIGHT = 32;
// With a title and no legend (both adjust this further: the
// constructor subtracts TITLE_HEIGHT back out when there's no title,
// and save() grows it per legend row).
const float LinePlot::MARGIN_TOP = 70;
const float LinePlot::MARGIN_BOTTOM = 76;
const int LinePlot::TICK_COUNT = 5;
const float LinePlot::FONT_SIZE = 19;

LinePlot::LinePlot(const std::string& filename, const std::string& title,
                   const std::string& xlabel, const std::string& ylabel,
                   const std::vector<float>& xticks, const std::vector<float>& yticks)
    : PlotBase(filename, title, WIDTH, HEIGHT, MARGIN_LEFT, MARGIN_RIGHT, MARGIN_TOP, MARGIN_BOTTOM, FONT_SIZE),
      m_xlabel(xlabel), m_ylabel(ylabel),
      m_saved(false), m_xticks(xticks), m_yticks(yticks) {
}

LinePlot::~LinePlot() {
    if (!m_saved) {
        save();
    }
}

void LinePlot::plot(const std::vector<float>& x, const std::vector<float>& y,
                    const std::string& label) {
    m_x.push_back(x);
    m_y.push_back(y);
    m_labels.push_back(label);
}

void LinePlot::plot(const std::vector<float>& y, const std::string& label) {
    std::vector<float> x(y.size());
    for (size_t i = 0; i < y.size(); i++) {
        x[i] = static_cast<float>(i);
    }
    plot(x, y, label);
}

void LinePlot::save() {
    // Legend: one row per labeled curve, right-aligned above the
    // plot, unboxed. A curve with an empty label is drawn but left
    // out of the legend. Grows the top margin by one row height per
    // legend entry, on top of whatever draw_header() already
    // reserved for the title (or didn't, if there is none).
    std::vector<size_t> legend_curves;
    for (size_t curve = 0; curve < m_labels.size(); curve++) {
        if (!m_labels[curve].empty()) {
            legend_curves.push_back(curve);
        }
    }
    float legend_row_height = FONT_SIZE + 10;
    float legend_gap = legend_curves.empty() ? 0.0f : 20.0f;
    float legend_top = m_margin_top;
    m_margin_top += legend_row_height * static_cast<float>(legend_curves.size()) + legend_gap;

    float left = plot_left();
    float top = plot_top();
    float area_width = plot_width();
    float area_height = plot_height();

    float x_min = m_x[0][0];
    float x_max = x_min;
    float y_min = m_y[0][0];
    float y_max = y_min;
    for (size_t curve = 0; curve < m_x.size(); curve++) {
        for (size_t i = 0; i < m_x[curve].size(); i++) {
            float value = m_x[curve][i];
            x_min = std::min(x_min, value);
            x_max = std::max(x_max, value);
        }
        for (size_t i = 0; i < m_y[curve].size(); i++) {
            float value = m_y[curve][i];
            y_min = std::min(y_min, value);
            y_max = std::max(y_max, value);
        }
    }
    float x_range = x_max != x_min ? x_max - x_min : 1;
    float y_range = y_max != y_min ? y_max - y_min : 1;

    std::string svg = draw_header();

    svg += "<defs>";
    for (int i = 0; i < 4; i++) {
        svg += PlotStyle::glow_filter("glow" + SvgBuilder::format_number(i), PlotStyle::PALETTE[i]);
    }
    svg += "</defs>";

    if (!legend_curves.empty()) {
        float swatch_width = 20;
        float swatch_gap = 8;
        float legend_right = left + area_width;

        for (size_t i = 0; i < legend_curves.size(); i++) {
            size_t curve = legend_curves[i];
            std::string color = PlotStyle::PALETTE[curve % 4];
            float row_y = legend_top + legend_row_height * (static_cast<float>(i) + 0.5f) +
                FONT_SIZE * 0.35f;
            float swatch_y = row_y - FONT_SIZE * 0.32f;
            float swatch_x = legend_right - swatch_width;
            svg += SvgBuilder::text(swatch_x - swatch_gap, row_y, m_labels[curve], "end", FONT_SIZE,
                PlotStyle::TEXT_COLOR, 0);
            svg += SvgBuilder::line(swatch_x, swatch_y, legend_right, swatch_y, color, 4);
        }
    }

    // Horizontal gridlines + y-axis ticks.
    std::vector<float> yticks = m_yticks;
    if (yticks.empty()) {
        for (int t = 0; t < TICK_COUNT; t++) {
            yticks.push_back(y_min + y_range * t / (TICK_COUNT - 1));
        }
    }
    for (size_t t = 0; t < yticks.size(); t++) {
        float py = top + area_height - (yticks[t] - y_min) / y_range * area_height;
        svg += SvgBuilder::line(left, py, left + area_width, py, PlotStyle::GRID_COLOR, 1);
        std::string label = SvgBuilder::format_number(yticks[t]);
        svg += SvgBuilder::text(left - 12, py + 6, label, "end", FONT_SIZE, PlotStyle::TEXT_COLOR, 0);
    }

    // x-axis ticks.
    std::vector<float> xticks = m_xticks;
    if (xticks.empty()) {
        for (int t = 0; t < TICK_COUNT; t++) {
            xticks.push_back(x_min + x_range * t / (TICK_COUNT - 1));
        }
    }
    for (size_t t = 0; t < xticks.size(); t++) {
        float px = left + (xticks[t] - x_min) / x_range * area_width;
        std::string label = SvgBuilder::format_number(xticks[t]);
        svg += SvgBuilder::text(px, top + area_height + 28, label,
            tick_label_anchor(px, label, FONT_SIZE, WIDTH), FONT_SIZE, PlotStyle::TEXT_COLOR, 0);
    }

    draw_axes(svg);

    // Curves, each a glowing line with a fat marker circle at every
    // data point (line and markers share one filtered <g>, so the
    // glow covers both).
    for (size_t curve = 0; curve < m_x.size(); curve++) {
        std::string color = PlotStyle::PALETTE[curve % 4];
        std::string points;
        std::string markers;
        for (size_t i = 0; i < m_x[curve].size(); i++) {
            float px = left + (m_x[curve][i] - x_min) / x_range * area_width;
            float py = top + area_height - (m_y[curve][i] - y_min) / y_range * area_height;
            points += SvgBuilder::format_number(px) + "," + SvgBuilder::format_number(py) + " ";
            markers += "<circle cx=\"" + SvgBuilder::format_number(px) + "\" cy=\"" +
                SvgBuilder::format_number(py) + "\" r=\"7\" fill=\"" + color + "\"/>";
        }
        svg += "<g filter=\"url(#glow" + SvgBuilder::format_number(curve % 4) + ")\">";
        svg += "<polyline points=\"" + points + "\" fill=\"none\" stroke=\"" + color +
            "\" stroke-width=\"5\" stroke-linecap=\"round\" stroke-linejoin=\"round\"/>";
        svg += markers;
        svg += "</g>";
    }

    // Axis labels, drawn last so they sit above everything else.
    if (!m_xlabel.empty()) {
        svg += SvgBuilder::text(left + area_width / 2, HEIGHT - 12, m_xlabel,
            "middle", FONT_SIZE, PlotStyle::TEXT_COLOR, 0, true);
    }
    if (!m_ylabel.empty()) {
        svg += SvgBuilder::text(20, top + area_height / 2, m_ylabel, "middle", FONT_SIZE,
            PlotStyle::TEXT_COLOR, -90, true);
    }

    write_svg(svg);
    m_saved = true;
}

const float HistogramPlot::WIDTH = 480;
const float HistogramPlot::HEIGHT = 320;
const float HistogramPlot::MARGIN_LEFT = 76;
const float HistogramPlot::MARGIN_RIGHT = 24;
const float HistogramPlot::MARGIN_TOP = 54;
const float HistogramPlot::MARGIN_BOTTOM = 56;
const int HistogramPlot::TICK_COUNT = 5;
const float HistogramPlot::FONT_SIZE = 19;

HistogramPlot::HistogramPlot(const std::string& filename, const std::string& title, float tolerance)
    : PlotBase(filename, title, WIDTH, HEIGHT, MARGIN_LEFT, MARGIN_RIGHT, MARGIN_TOP, MARGIN_BOTTOM, FONT_SIZE),
      m_tolerance(tolerance), m_saved(false) {
}

HistogramPlot::~HistogramPlot() {
    if (!m_saved) {
        save();
    }
}

void HistogramPlot::draw(const std::vector<float>& frequencies, const std::vector<float>& x) {
    m_frequencies = frequencies;
    m_x = x;
}

void HistogramPlot::save() {
    float sum = 0;
    for (size_t i = 0; i < m_frequencies.size(); i++) {
        sum += m_frequencies[i];
    }

    float left = plot_left();
    float top = plot_top();
    float area_width = plot_width();
    float area_height = plot_height();

    std::string svg = draw_header();
    svg += "<defs>" + PlotStyle::glow_filter("histGlow", PlotStyle::PALETTE[0]) + "</defs>";

    draw_axes(svg);

    if (std::abs(sum - 1.0f) > m_tolerance) {
        std::string line1 = "Error: frequencies don't add up to 1.";
        std::string line2 = "(sum = " + SvgBuilder::format_number(sum) + ", tolerance = " +
            SvgBuilder::format_number(m_tolerance) + ")";
        float content_width = std::max(
            SvgBuilder::text_width(line1, FONT_SIZE), SvgBuilder::text_width(line2, FONT_SIZE));
        float panel_w = std::min(content_width + 40, WIDTH - 32);
        float panel_h = 84;
        float panel_x = (WIDTH - panel_w) / 2;
        float panel_y = top + (area_height - panel_h) / 2;
        svg += SvgBuilder::rect(panel_x, panel_y, panel_w, panel_h, 8,
            PlotStyle::ERROR_BACKGROUND, PlotStyle::ERROR_COLOR, 2);
        svg += SvgBuilder::text(WIDTH / 2, panel_y + 34, line1, "middle", FONT_SIZE, PlotStyle::ERROR_COLOR, 0);
        svg += SvgBuilder::text(WIDTH / 2, panel_y + 62, line2, "middle", FONT_SIZE, PlotStyle::ERROR_COLOR, 0);
    } else {
        float max_frequency = 0;
        for (size_t i = 0; i < m_frequencies.size(); i++) {
            max_frequency = std::max(max_frequency, m_frequencies[i]);
        }
        if (max_frequency <= 0) {
            max_frequency = 1;
        }

        for (int t = 0; t < TICK_COUNT; t++) {
            float value = max_frequency * t / (TICK_COUNT - 1);
            float py = top + area_height - value / max_frequency * area_height;
            svg += SvgBuilder::line(left, py, left + area_width, py, PlotStyle::GRID_COLOR, 1);
            svg += SvgBuilder::text(left - 12, py + 6, SvgBuilder::format_number(value), "end", FONT_SIZE,
                PlotStyle::TEXT_COLOR, 0);
        }

        float count = static_cast<float>(m_frequencies.size());
        float slot = count > 0 ? area_width / count : area_width;
        float bar_width = slot * 0.6f;
        svg += "<g filter=\"url(#histGlow)\">";
        for (size_t i = 0; i < m_frequencies.size(); i++) {
            float bar_height = m_frequencies[i] / max_frequency * area_height;
            float bx = left + slot * static_cast<float>(i) + (slot - bar_width) / 2;
            float by = top + area_height - bar_height;
            svg += "<rect x=\"" + SvgBuilder::format_number(bx) + "\" y=\"" + SvgBuilder::format_number(by) +
                "\" width=\"" + SvgBuilder::format_number(bar_width) + "\" height=\"" +
                SvgBuilder::format_number(bar_height) + "\" rx=\"3\" fill=\"" +
                std::string(PlotStyle::PALETTE[0]) + "\"/>";
        }
        svg += "</g>";

        if (m_x.size() == m_frequencies.size()) {
            for (size_t i = 0; i < m_x.size(); i++) {
                float bx = left + slot * (static_cast<float>(i) + 0.5f);
                std::string label = SvgBuilder::format_number(m_x[i]);
                svg += SvgBuilder::text(bx, top + area_height + 28, label,
                    tick_label_anchor(bx, label, FONT_SIZE, WIDTH), FONT_SIZE, PlotStyle::TEXT_COLOR, 0);
            }
        }
    }

    write_svg(svg);
    m_saved = true;
}
