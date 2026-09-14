/// Utility to render very simple charts (line plots and
/// probability histograms) as SVG files, from plain std::vector
/// data, with no external tool or library involved. This is
/// instructor-facing scaffolding, not quest material: the
/// string-building it does to assemble an SVG document goes a bit
/// beyond what the course itself teaches, so treat its
/// implementation as a black box rather than as example code for
/// students.
///
/// Every chart is drawn at a fixed size with fixed margins, so a
/// sequence of plots (e.g. one per loop iteration) lines up frame
/// to frame instead of jittering around.
#pragma once
#include <string>
#include <vector>

/// Draws a line plot, one curve at a time, to an SVG file.
///
/// Example:
/// @code
/// LinePlot plot("complexity.svg", "Complexity comparison",
///                "input size", "steps", true, false);
/// plot.plot(sizes, linear_steps, "linear");
/// plot.plot(sizes, log_steps, "logarithmic");
/// plot.save();
/// @endcode
class LinePlot {
public:
    /// Start a new line plot. Call plot() at least once, then
    /// save() (or just let the object be destroyed) to write it.
    /// @param filename SVG file to write.
    /// @param title plot title, shown above the plot; omitted if empty.
    /// @param xlabel horizontal axis label.
    /// @param ylabel vertical axis label.
    /// @param logx draw the horizontal axis in logarithmic scale.
    /// @param logy draw the vertical axis in logarithmic scale.
    /// @param xticks x-axis tick values to label; leave empty for
    ///   evenly spaced automatic ticks.
    /// @param yticks y-axis tick values to label; leave empty for
    ///   evenly spaced automatic ticks.
    LinePlot(const std::string& filename,
            const std::string& title = "",
            const std::string& xlabel = "",
            const std::string& ylabel = "",
            bool logx = false,
            bool logy = false,
            const std::vector<float>& xticks = {},
            const std::vector<float>& yticks = {});

    /// Writes the file if save() was never called.
    ~LinePlot();

    /// Add one curve to the plot.
    /// @param x horizontal-axis values, in ascending order; must be
    ///   strictly positive if the plot uses a logarithmic x-axis.
    /// @param y one value per entry of `x`; must be strictly
    ///   positive if the plot uses a logarithmic y-axis.
    /// @param label legend label for this curve, shown in a row
    ///   above the plot; leave empty to omit this curve from the
    ///   legend. Drawing more than 4 curves repeats colors.
    void plot(const std::vector<float>& x,
             const std::vector<float>& y,
             const std::string& label = "");

    /// Add one curve to the plot, using 0, 1, 2, ... as its x values.
    /// @param y one value per point; must be strictly positive if
    ///   the plot uses a logarithmic y-axis.
    /// @param label legend label for this curve, shown in a row
    ///   above the plot; leave empty to omit this curve from the
    ///   legend. Drawing more than 4 curves repeats colors.
    void plot(const std::vector<float>& y, const std::string& label = "");

    /// Write the SVG file now, from every curve added so far. Safe
    /// to call more than once, or not at all (the destructor then
    /// does it instead). At least one curve must have been added.
    void save();

private:
    // Fixed canvas/margins, so a sequence of plots lines up frame
    // to frame instead of jittering around (defined in plot.cpp).
    static const float WIDTH;
    static const float HEIGHT;
    static const float MARGIN_LEFT;
    static const float MARGIN_RIGHT;
    static const float MARGIN_TOP;
    static const float MARGIN_BOTTOM;
    static const int TICK_COUNT;
    // Every piece of text is drawn at this size (the title is bold
    // instead of larger).
    static const float FONT_SIZE;

    /// SVG file to write, set at construction.
    std::string m_filename;
    /// Plot title; empty to omit it.
    std::string m_title;
    /// Horizontal axis label.
    std::string m_xlabel;
    /// Vertical axis label.
    std::string m_ylabel;
    /// True to draw the horizontal axis in logarithmic scale.
    bool m_logx;
    /// True to draw the vertical axis in logarithmic scale.
    bool m_logy;
    /// True once save() has written the file.
    bool m_saved;
    /// x-axis tick values to label; empty for automatic ticks.
    std::vector<float> m_xticks;
    /// y-axis tick values to label; empty for automatic ticks.
    std::vector<float> m_yticks;

    /// x values of each curve added so far, one entry per plot() call.
    std::vector<std::vector<float>> m_x;
    /// y values of each curve added so far, aligned with m_x.
    std::vector<std::vector<float>> m_y;
    /// Legend label of each curve added so far, aligned with m_x
    /// (empty for a curve left out of the legend).
    std::vector<std::string> m_labels;
};

/// Draws a probability distribution as a vertical-bar SVG chart:
/// the graphical counterpart of Histogram::print (see
/// histogram.h), with the same complaint instead of a plot when
/// the frequencies don't add up to 1.
class HistogramPlot {
public:
    /// Write an SVG file with one bar per entry of `frequencies`,
    /// or, if they don't add up to 1 (within `tolerance`), an SVG
    /// showing that error instead of bars.
    /// @param frequencies relative frequencies to plot; must add
    ///   up to 1, since each value is drawn as a fraction of the
    ///   tallest bar.
    /// @param filename SVG file to write.
    /// @param title plot title, shown above the bars; omitted if
    ///   empty.
    /// @param tolerance largest allowed absolute difference
    ///   between the sum of `frequencies` and 1 before it's
    ///   treated as an error.
    ///
    /// Example:
    /// @code
    /// std::vector<float> distribution = {0.1f, 0.2f, 0.4f, 0.3f};
    /// HistogramPlot::draw(distribution, "distribution.svg");
    /// @endcode
    static void draw(const std::vector<float>& frequencies,
                     const std::string& filename,
                     const std::string& title = "",
                     float tolerance = 1e-6f);

private:
    // Fixed canvas/margins, including the error state, so
    // consecutive frames (e.g. a shrinking population) don't
    // jitter either (defined in plot.cpp).
    static const float WIDTH;
    static const float HEIGHT;
    static const float MARGIN_LEFT;
    static const float MARGIN_RIGHT;
    static const float MARGIN_TOP;
    static const float MARGIN_BOTTOM;
    static const int TICK_COUNT;
    // Every piece of text is drawn at this size (the title is bold
    // instead of larger).
    static const float FONT_SIZE;
};
