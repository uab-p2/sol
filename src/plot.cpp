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
    /// in scientific notation when it is too small for 2 decimals
    /// to show anything but "0.00" (e.g. a tolerance like 1e-6).
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
                            const std::string& color, float rotate_deg);

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
    float rounded = std::floor(value + 0.5f);
    if (value != 0 && std::abs(value) < 0.005f) {
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
                             const std::string& color, float rotate_deg) {
    std::string transform_attr;
    if (rotate_deg != 0) {
        transform_attr = " transform=\"rotate(" + format_number(rotate_deg) +
            " " + format_number(x) + " " + format_number(y) + ")\"";
    }
    return "<text x=\"" + format_number(x) + "\" y=\"" + format_number(y) +
        "\" text-anchor=\"" + anchor + "\" font-size=\"" +
        format_number(size) + "\" fill=\"" + color + "\"" + transform_attr +
        ">" + escape_xml(content) + "</text>";
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

const float LinePlot::WIDTH = 640;
const float LinePlot::HEIGHT = 360;
const float LinePlot::MARGIN_LEFT = 84;
const float LinePlot::MARGIN_RIGHT = 24;
const float LinePlot::MARGIN_TOP = 62;
const float LinePlot::MARGIN_BOTTOM = 60;
const int LinePlot::TICK_COUNT = 5;

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
        svg += SvgBuilder::text(WIDTH / 2, 24, m_title, "middle", 19, ChartStyle::TEXT_COLOR, 0);
    }

    // Legend: one entry per labeled curve, evenly spread in a row
    // above the plot. A curve with an empty label is drawn but left
    // out of the legend.
    std::vector<size_t> legend_curves;
    for (size_t curve = 0; curve < m_labels.size(); curve++) {
        if (!m_labels[curve].empty()) {
            legend_curves.push_back(curve);
        }
    }
    if (!legend_curves.empty()) {
        // Laid out with each item's own estimated width (not equal
        // slots): an equal split would let a long label in a narrow
        // slot run past the canvas edge.
        float legend_y = 48;
        float swatch_width = 20;
        float swatch_gap = 6;
        float item_gap = 22;
        float total_width = 0;
        for (size_t i = 0; i < legend_curves.size(); i++) {
            total_width += swatch_width + swatch_gap +
                SvgBuilder::text_width(m_labels[legend_curves[i]], 14);
        }
        total_width += item_gap * static_cast<float>(legend_curves.size() - 1);

        float cursor = plot_left + std::max(0.0f, (plot_width - total_width) / 2);
        for (size_t i = 0; i < legend_curves.size(); i++) {
            size_t curve = legend_curves[i];
            std::string color = ChartStyle::PALETTE[curve % 4];
            svg += SvgBuilder::line(cursor, legend_y - 5, cursor + swatch_width, legend_y - 5, color, 4);
            svg += SvgBuilder::text(cursor + swatch_width + swatch_gap, legend_y, m_labels[curve],
                "start", 14, ChartStyle::TEXT_COLOR, 0);
            cursor += swatch_width + swatch_gap +
                SvgBuilder::text_width(m_labels[curve], 14) + item_gap;
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
        svg += SvgBuilder::text(plot_left - 10, py + 5, label, "end", 13, ChartStyle::TEXT_COLOR, 0);
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
        svg += SvgBuilder::text(px, plot_top + plot_height + 22, label, "middle", 13, ChartStyle::TEXT_COLOR, 0);
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
        svg += SvgBuilder::text(plot_left + plot_width / 2, HEIGHT - 10, m_xlabel,
            "middle", 14, ChartStyle::TEXT_COLOR, 0);
    }
    if (!m_ylabel.empty()) {
        svg += SvgBuilder::text(20, plot_top + plot_height / 2, m_ylabel, "middle", 14,
            ChartStyle::TEXT_COLOR, -90);
    }

    svg += "</svg>";
    SvgBuilder::write_file(m_filename, svg);
    m_saved = true;
}

const float HistogramPlot::WIDTH = 480;
const float HistogramPlot::HEIGHT = 320;
const float HistogramPlot::MARGIN_LEFT = 62;
const float HistogramPlot::MARGIN_RIGHT = 24;
const float HistogramPlot::MARGIN_TOP = 46;
const float HistogramPlot::MARGIN_BOTTOM = 32;
const int HistogramPlot::TICK_COUNT = 5;

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
        svg += SvgBuilder::text(WIDTH / 2, 24, title, "middle", 19, ChartStyle::TEXT_COLOR, 0);
    }

    svg += SvgBuilder::line(plot_left, plot_top, plot_left, plot_top + plot_height, ChartStyle::AXIS_COLOR, 2);
    svg += SvgBuilder::line(plot_left, plot_top + plot_height, plot_left + plot_width,
        plot_top + plot_height, ChartStyle::AXIS_COLOR, 2);

    if (std::abs(sum - 1.0f) > tolerance) {
        float panel_w = plot_width * 0.85f;
        float panel_h = 64;
        float panel_x = plot_left + (plot_width - panel_w) / 2;
        float panel_y = plot_top + (plot_height - panel_h) / 2;
        svg += "<rect x=\"" + SvgBuilder::format_number(panel_x) + "\" y=\"" +
            SvgBuilder::format_number(panel_y) + "\" width=\"" + SvgBuilder::format_number(panel_w) +
            "\" height=\"" + SvgBuilder::format_number(panel_h) + "\" rx=\"6\" fill=\"" +
            ChartStyle::ERROR_BACKGROUND + "\" stroke=\"" + ChartStyle::ERROR_COLOR + "\" stroke-width=\"2\"/>";
        svg += SvgBuilder::text(panel_x + panel_w / 2, panel_y + 28,
            "Error: frequencies don't add up to 1.", "middle", 14, ChartStyle::ERROR_COLOR, 0);
        svg += SvgBuilder::text(panel_x + panel_w / 2, panel_y + 48,
            "(sum = " + SvgBuilder::format_number(sum) + ", tolerance = " +
            SvgBuilder::format_number(tolerance) + ")", "middle", 13, ChartStyle::ERROR_COLOR, 0);
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
            svg += SvgBuilder::text(plot_left - 10, py + 5, SvgBuilder::format_number(value), "end", 13,
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
