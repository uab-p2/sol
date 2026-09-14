#include "plot.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>

/// Generic, project-agnostic SVG string-building: elements,
/// escaping, number formatting, file output. Knows nothing about
/// this project's own colors -- see ChartStyle for those.
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

    /// Format an axis tick's real (non-transformed) value for
    /// display. A log-scale axis always uses scientific notation,
    /// since its ticks routinely span many orders of magnitude:
    /// that keeps every label about the same width, rather than
    /// clipping against the margin on whichever tick happens to be
    /// huge.
    static std::string format_axis_label(float value, bool log_scale);

    /// @return `value` in the coordinate space actually plotted:
    ///   itself, or its base-10 logarithm when `log_scale` is set.
    static float axis_value(float value, bool log_scale);

    /// @return a rough estimate, in pixels, of how wide `text` will
    ///   render at `size`. No real font metrics are available here,
    ///   so this just assumes an average glyph width -- good enough
    ///   to lay out a legend without it running off the canvas.
    static float text_width(const std::string& text, float size);

    /// @return a `<text>` element.
    static std::string text(float x, float y, const std::string& content,
                            const std::string& anchor, float size,
                            const std::string& color, float rotate_deg,
                            bool bold = false);

    /// @return a `<rect>` element, e.g. a box framing the legend.
    static std::string rect(float x, float y, float width, float height,
                            float corner_radius, const std::string& fill,
                            const std::string& stroke);

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

std::string SvgBuilder::format_axis_label(float value, bool log_scale) {
    std::string label;
    if (log_scale) {
        std::ostringstream stream;
        stream << std::scientific << std::setprecision(1) << value;
        label = stream.str();
    } else {
        label = format_number(value);
    }
    return label;
}

float SvgBuilder::axis_value(float value, bool log_scale) {
    return log_scale ? std::log10(value) : value;
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
                             const std::string& stroke) {
    std::string stroke_attr = stroke.empty() ? "" : " stroke=\"" + stroke + "\" stroke-width=\"2\"";
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

/// This project's own visual identity, applied over plain SvgBuilder
/// elements: the color palette and the "glow" effect.
class ChartStyle {
public:
    /// @return a `<filter>` producing a soft colored halo behind
    ///   whatever it is applied to, e.g. `filter="url(#glow0)"` on
    ///   a `<polyline>` or a `<g>` wrapping several bars. Built from
    ///   feGaussianBlur/feFlood/feComposite/feMerge rather than the
    ///   (functionally equivalent) feDropShadow, since some SVG
    ///   renderers used to preview/export these charts (e.g.
    ///   Inkscape's librsvg) don't implement feDropShadow yet.
    static std::string glow_filter(const std::string& id, const std::string& color);

    /// Campaign cyan/orange/purple (see macro.py's CalendarMacros.
    /// _CAMPAIGN_COLORS) plus a green for a 4th curve.
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

std::string ChartStyle::glow_filter(const std::string& id, const std::string& color) {
    return "<filter id=\"" + id + "\" x=\"-30%\" y=\"-30%\" width=\"160%\" height=\"160%\">"
        "<feFlood flood-color=\"" + color + "\" flood-opacity=\"0.55\" result=\"glowColor\"/>"
        "<feComposite in=\"glowColor\" in2=\"SourceGraphic\" operator=\"in\" result=\"coloredSource\"/>"
        "<feGaussianBlur in=\"coloredSource\" stdDeviation=\"3\" result=\"blurred\"/>"
        "<feMerge><feMergeNode in=\"blurred\"/><feMergeNode in=\"SourceGraphic\"/></feMerge>"
        "</filter>";
}

const char* const ChartStyle::PALETTE[4] = {"#00b8d4", "#ff6e42", "#7c4dff", "#43a047"};
const char* const ChartStyle::BACKGROUND = "#ffffff";
const char* const ChartStyle::AXIS_COLOR = "#555555";
const char* const ChartStyle::GRID_COLOR = "#e4e4e4";
const char* const ChartStyle::TEXT_COLOR = "#333333";
const char* const ChartStyle::ERROR_COLOR = "#c0392b";
const char* const ChartStyle::ERROR_BACKGROUND = "#fdecea";
const char* const ChartStyle::BOX_BACKGROUND = "#f7f7f9";
const char* const ChartStyle::BOX_BORDER = "#dcdce0";

const float LinePlot::WIDTH = 640;
const float LinePlot::HEIGHT = 400;
const float LinePlot::MARGIN_LEFT = 108;
// Wide enough that the rightmost x-tick label (middle-anchored right
// on the plot's edge, so half its width sits in this margin) doesn't
// clip, even at a wide label like "1.00e+12".
const float LinePlot::MARGIN_RIGHT = 48;
// Tall enough for a legend wrapped onto 2 rows (see the "rows" wrap
// in save()), the common case at this font size with more than 2
// curves -- a legend needing a 3rd row will overlap the plot itself.
const float LinePlot::MARGIN_TOP = 132;
const float LinePlot::MARGIN_BOTTOM = 76;
const int LinePlot::TICK_COUNT = 5;
const float LinePlot::FONT_SIZE = 19;

LinePlot::LinePlot(const std::string& filename, const std::string& title,
                   const std::string& xlabel, const std::string& ylabel,
                   bool logx, bool logy,
                   const std::vector<float>& xticks, const std::vector<float>& yticks)
    : m_filename(filename), m_title(title), m_xlabel(xlabel), m_ylabel(ylabel),
      m_logx(logx), m_logy(logy), m_saved(false), m_xticks(xticks), m_yticks(yticks) {
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
    float plot_left = MARGIN_LEFT;
    float plot_top = MARGIN_TOP;
    float plot_width = WIDTH - MARGIN_LEFT - MARGIN_RIGHT;
    float plot_height = HEIGHT - MARGIN_TOP - MARGIN_BOTTOM;

    float x_min = SvgBuilder::axis_value(m_x[0][0], m_logx);
    float x_max = x_min;
    float y_min = SvgBuilder::axis_value(m_y[0][0], m_logy);
    float y_max = y_min;
    for (size_t curve = 0; curve < m_x.size(); curve++) {
        for (size_t i = 0; i < m_x[curve].size(); i++) {
            float value = SvgBuilder::axis_value(m_x[curve][i], m_logx);
            x_min = std::min(x_min, value);
            x_max = std::max(x_max, value);
        }
        for (size_t i = 0; i < m_y[curve].size(); i++) {
            float value = SvgBuilder::axis_value(m_y[curve][i], m_logy);
            y_min = std::min(y_min, value);
            y_max = std::max(y_max, value);
        }
    }
    float x_range = x_max != x_min ? x_max - x_min : 1;
    float y_range = y_max != y_min ? y_max - y_min : 1;

    std::string svg = SvgBuilder::header(WIDTH, HEIGHT, ChartStyle::BACKGROUND);

    svg += "<defs>";
    for (int i = 0; i < 4; i++) {
        svg += ChartStyle::glow_filter("glow" + SvgBuilder::format_number(i), ChartStyle::PALETTE[i]);
    }
    svg += "</defs>";

    if (!m_title.empty()) {
        svg += SvgBuilder::text(WIDTH / 2, 30, m_title, "middle", FONT_SIZE, ChartStyle::TEXT_COLOR, 0, true);
    }

    // Legend: one entry per labeled curve, in a boxed row right-
    // aligned above the plot -- each item reads left to right as
    // [label][colored line], the row itself flush against the
    // plot's right edge. A curve with an empty label is drawn but
    // left out of the legend.
    std::vector<size_t> legend_curves;
    for (size_t curve = 0; curve < m_labels.size(); curve++) {
        if (!m_labels[curve].empty()) {
            legend_curves.push_back(curve);
        }
    }
    if (!legend_curves.empty()) {
        float swatch_width = 22;
        float swatch_gap = 8;
        float item_gap = 26;
        float box_pad_x = 14;
        float box_pad_y = 10;
        float box_top = 46;
        float row_height = FONT_SIZE + 8;

        // Each item's own estimated width (not equal slots): an
        // equal split would let a long label in a narrow slot run
        // past the canvas edge. Items are then greedily wrapped into
        // rows that fit the plot width, so a long label or many
        // curves grow the legend downward instead of sideways past
        // the canvas.
        std::vector<float> item_widths(legend_curves.size());
        for (size_t i = 0; i < legend_curves.size(); i++) {
            item_widths[i] = SvgBuilder::text_width(m_labels[legend_curves[i]], FONT_SIZE) +
                swatch_gap + swatch_width;
        }

        std::vector<std::vector<size_t>> rows;
        std::vector<float> row_widths;
        float row_width = 0;
        for (size_t i = 0; i < legend_curves.size(); i++) {
            float needed = rows.empty() || rows.back().empty()
                ? item_widths[i] : row_width + item_gap + item_widths[i];
            if (!rows.empty() && !rows.back().empty() && needed > plot_width) {
                row_widths.push_back(row_width);
                rows.push_back({});
                row_width = item_widths[i];
            } else {
                if (rows.empty()) {
                    rows.push_back({});
                }
                row_width = needed;
            }
            rows.back().push_back(i);
        }
        row_widths.push_back(row_width);

        float box_width = 0;
        for (float width : row_widths) {
            box_width = std::max(box_width, width);
        }
        box_width += box_pad_x * 2;
        float box_height = row_height * static_cast<float>(rows.size()) + box_pad_y * 2;
        float box_right = plot_left + plot_width;
        float box_left = box_right - box_width;

        svg += SvgBuilder::rect(box_left, box_top, box_width, box_height, 8,
            ChartStyle::BOX_BACKGROUND, ChartStyle::BOX_BORDER);

        for (size_t r = 0; r < rows.size(); r++) {
            float legend_y = box_top + box_pad_y + row_height * (static_cast<float>(r) + 0.5f) +
                FONT_SIZE * 0.35f;
            float swatch_y = legend_y - FONT_SIZE * 0.32f;
            float cursor = box_right - box_pad_x - row_widths[r];
            for (size_t i : rows[r]) {
                size_t curve = legend_curves[i];
                std::string color = ChartStyle::PALETTE[curve % 4];
                float text_w = SvgBuilder::text_width(m_labels[curve], FONT_SIZE);
                svg += SvgBuilder::text(cursor, legend_y, m_labels[curve], "start", FONT_SIZE,
                    ChartStyle::TEXT_COLOR, 0);
                float swatch_x = cursor + text_w + swatch_gap;
                svg += SvgBuilder::line(swatch_x, swatch_y, swatch_x + swatch_width, swatch_y, color, 4);
                cursor += item_widths[i] + item_gap;
            }
        }
    }

    // Horizontal gridlines + y-axis ticks.
    std::vector<float> yticks = m_yticks;
    if (yticks.empty()) {
        for (int t = 0; t < TICK_COUNT; t++) {
            float real_value = y_min + y_range * t / (TICK_COUNT - 1);
            yticks.push_back(m_logy ? std::pow(10, real_value) : real_value);
        }
    }
    for (size_t t = 0; t < yticks.size(); t++) {
        float value = SvgBuilder::axis_value(yticks[t], m_logy);
        float py = plot_top + plot_height - (value - y_min) / y_range * plot_height;
        svg += SvgBuilder::line(plot_left, py, plot_left + plot_width, py, ChartStyle::GRID_COLOR, 1);
        std::string label = SvgBuilder::format_axis_label(yticks[t], m_logy);
        svg += SvgBuilder::text(plot_left - 12, py + 6, label, "end", FONT_SIZE, ChartStyle::TEXT_COLOR, 0);
    }

    // x-axis ticks.
    std::vector<float> xticks = m_xticks;
    if (xticks.empty()) {
        for (int t = 0; t < TICK_COUNT; t++) {
            float real_value = x_min + x_range * t / (TICK_COUNT - 1);
            xticks.push_back(m_logx ? std::pow(10, real_value) : real_value);
        }
    }
    for (size_t t = 0; t < xticks.size(); t++) {
        float value = SvgBuilder::axis_value(xticks[t], m_logx);
        float px = plot_left + (value - x_min) / x_range * plot_width;
        std::string label = SvgBuilder::format_axis_label(xticks[t], m_logx);
        svg += SvgBuilder::text(px, plot_top + plot_height + 28, label, "middle", FONT_SIZE, ChartStyle::TEXT_COLOR, 0);
    }

    // Axes, drawn on top of the gridlines.
    svg += SvgBuilder::line(plot_left, plot_top, plot_left, plot_top + plot_height, ChartStyle::AXIS_COLOR, 2);
    svg += SvgBuilder::line(plot_left, plot_top + plot_height, plot_left + plot_width,
        plot_top + plot_height, ChartStyle::AXIS_COLOR, 2);

    // Curves.
    for (size_t curve = 0; curve < m_x.size(); curve++) {
        std::string points;
        for (size_t i = 0; i < m_x[curve].size(); i++) {
            float px = plot_left +
                (SvgBuilder::axis_value(m_x[curve][i], m_logx) - x_min) / x_range * plot_width;
            float py = plot_top + plot_height -
                (SvgBuilder::axis_value(m_y[curve][i], m_logy) - y_min) / y_range * plot_height;
            points += SvgBuilder::format_number(px) + "," + SvgBuilder::format_number(py) + " ";
        }
        svg += "<polyline points=\"" + points + "\" fill=\"none\" stroke=\"" +
            ChartStyle::PALETTE[curve % 4] + "\" stroke-width=\"3.5\" stroke-linecap=\"round\" " +
            "stroke-linejoin=\"round\" filter=\"url(#glow" + SvgBuilder::format_number(curve % 4) +
            ")\"/>";
    }

    // Axis labels, drawn last so they sit above everything else.
    if (!m_xlabel.empty()) {
        svg += SvgBuilder::text(plot_left + plot_width / 2, HEIGHT - 16, m_xlabel,
            "middle", FONT_SIZE, ChartStyle::TEXT_COLOR, 0);
    }
    if (!m_ylabel.empty()) {
        svg += SvgBuilder::text(24, plot_top + plot_height / 2, m_ylabel, "middle", FONT_SIZE,
            ChartStyle::TEXT_COLOR, -90);
    }

    svg += "</svg>";
    SvgBuilder::write_file(m_filename, svg);
    m_saved = true;
}

const float HistogramPlot::WIDTH = 480;
const float HistogramPlot::HEIGHT = 320;
const float HistogramPlot::MARGIN_LEFT = 76;
const float HistogramPlot::MARGIN_RIGHT = 24;
const float HistogramPlot::MARGIN_TOP = 54;
const float HistogramPlot::MARGIN_BOTTOM = 32;
const int HistogramPlot::TICK_COUNT = 5;
const float HistogramPlot::FONT_SIZE = 19;

void HistogramPlot::draw(const std::vector<float>& frequencies,
                         const std::string& filename,
                         const std::string& title, float tolerance) {
    float sum = 0;
    for (size_t i = 0; i < frequencies.size(); i++) {
        sum += frequencies[i];
    }

    float plot_left = MARGIN_LEFT;
    float plot_top = MARGIN_TOP;
    float plot_width = WIDTH - MARGIN_LEFT - MARGIN_RIGHT;
    float plot_height = HEIGHT - MARGIN_TOP - MARGIN_BOTTOM;

    std::string svg = SvgBuilder::header(WIDTH, HEIGHT, ChartStyle::BACKGROUND);
    svg += "<defs>"
        "<linearGradient id=\"histBar\" x1=\"0\" y1=\"0\" x2=\"0\" y2=\"1\">"
        "<stop offset=\"0%\" stop-color=\"#ffb199\"/>"
        "<stop offset=\"100%\" stop-color=\"" + std::string(ChartStyle::PALETTE[1]) + "\"/></linearGradient>" +
        ChartStyle::glow_filter("histGlow", ChartStyle::PALETTE[1]) + "</defs>";

    if (!title.empty()) {
        svg += SvgBuilder::text(WIDTH / 2, 30, title, "middle", FONT_SIZE, ChartStyle::TEXT_COLOR, 0, true);
    }

    svg += SvgBuilder::line(plot_left, plot_top, plot_left, plot_top + plot_height, ChartStyle::AXIS_COLOR, 2);
    svg += SvgBuilder::line(plot_left, plot_top + plot_height, plot_left + plot_width,
        plot_top + plot_height, ChartStyle::AXIS_COLOR, 2);

    if (std::abs(sum - 1.0f) > tolerance) {
        std::string line1 = "Error: frequencies don't add up to 1.";
        std::string line2 = "(sum = " + SvgBuilder::format_number(sum) + ", tolerance = " +
            SvgBuilder::format_number(tolerance) + ")";
        float content_width = std::max(
            SvgBuilder::text_width(line1, FONT_SIZE), SvgBuilder::text_width(line2, FONT_SIZE));
        float panel_w = std::min(content_width + 40, WIDTH - 32);
        float panel_h = 84;
        float panel_x = (WIDTH - panel_w) / 2;
        float panel_y = plot_top + (plot_height - panel_h) / 2;
        svg += SvgBuilder::rect(panel_x, panel_y, panel_w, panel_h, 8,
            ChartStyle::ERROR_BACKGROUND, ChartStyle::ERROR_COLOR);
        svg += SvgBuilder::text(WIDTH / 2, panel_y + 34, line1, "middle", FONT_SIZE, ChartStyle::ERROR_COLOR, 0);
        svg += SvgBuilder::text(WIDTH / 2, panel_y + 62, line2, "middle", FONT_SIZE, ChartStyle::ERROR_COLOR, 0);
    } else {
        float max_frequency = 0;
        for (size_t i = 0; i < frequencies.size(); i++) {
            max_frequency = std::max(max_frequency, frequencies[i]);
        }
        if (max_frequency <= 0) {
            max_frequency = 1;
        }

        for (int t = 0; t < TICK_COUNT; t++) {
            float value = max_frequency * t / (TICK_COUNT - 1);
            float py = plot_top + plot_height - value / max_frequency * plot_height;
            svg += SvgBuilder::line(plot_left, py, plot_left + plot_width, py, ChartStyle::GRID_COLOR, 1);
            svg += SvgBuilder::text(plot_left - 12, py + 6, SvgBuilder::format_number(value), "end", FONT_SIZE,
                ChartStyle::TEXT_COLOR, 0);
        }

        float count = static_cast<float>(frequencies.size());
        float slot = count > 0 ? plot_width / count : plot_width;
        float bar_width = slot * 0.6f;
        svg += "<g filter=\"url(#histGlow)\">";
        for (size_t i = 0; i < frequencies.size(); i++) {
            float bar_height = frequencies[i] / max_frequency * plot_height;
            float bx = plot_left + slot * static_cast<float>(i) + (slot - bar_width) / 2;
            float by = plot_top + plot_height - bar_height;
            svg += "<rect x=\"" + SvgBuilder::format_number(bx) + "\" y=\"" + SvgBuilder::format_number(by) +
                "\" width=\"" + SvgBuilder::format_number(bar_width) + "\" height=\"" +
                SvgBuilder::format_number(bar_height) + "\" rx=\"3\" fill=\"url(#histBar)\"/>";
        }
        svg += "</g>";
    }

    svg += "</svg>";
    SvgBuilder::write_file(filename, svg);
}
