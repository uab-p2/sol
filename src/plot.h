/// Utility to render very simple charts (line plots and
/// probability histograms) as SVG files.
#pragma once
#include <string>
#include <vector>

/// Shared base for every chart: the canvas size and margins it is
/// drawn with, and the SVG boilerplate common to all of them (the
/// header, the title, the plot area's axis lines, and writing the
/// finished file). LinePlot and HistogramPlot each add their own
/// data and drawing on top of this. Not meant to be used on its
/// own: its constructor and every method are protected.
class Chart {
protected:
    /// @param filename SVG file to write.
    /// @param title chart title, shown above the plot; omitted if empty.
    /// @param width canvas width, in pixels.
    /// @param height canvas height, in pixels.
    /// @param margin_left space reserved left of the plot area, for
    ///   its y-axis tick labels and axis label.
    /// @param margin_right space reserved right of the plot area.
    /// @param margin_top space reserved above the plot area, for the
    ///   title and (for LinePlot) the legend.
    /// @param margin_bottom space reserved below the plot area, for
    ///   its x-axis tick labels and axis label.
    /// @param font_size size every piece of text is drawn at (the
    ///   title is bold instead of larger).
    Chart(const std::string& filename, const std::string& title,
         float width, float height,
         float margin_left, float margin_right, float margin_top, float margin_bottom,
         float font_size);

    /// @return the plot area's left edge, in canvas pixels.
    float plot_left() const;
    /// @return the plot area's top edge, in canvas pixels.
    float plot_top() const;
    /// @return the plot area's width, in pixels.
    float plot_width() const;
    /// @return the plot area's height, in pixels.
    float plot_height() const;

    /// @return an SVG document containing just the canvas, its
    ///   white background, and the bold centered title (if any).
    ///   The caller appends its own `<defs>` and drawing, then
    ///   closes the document with write_svg().
    std::string draw_header() const;

    /// Append the plot area's left and bottom axis lines to `svg`.
    void draw_axes(std::string& svg) const;

    /// Close `svg`'s `</svg>` tag and write it to `filename` (see
    /// the constructor).
    void write_svg(std::string svg) const;

    std::string m_filename;
    std::string m_title;
    float m_width;
    float m_height;
    float m_margin_left;
    float m_margin_right;
    float m_margin_top;
    float m_margin_bottom;
    float m_font_size;
};

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
class LinePlot : private Chart {
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
    static const float FONT_SIZE;

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

/// Draws a probability distribution as a vertical-bar SVG chart,
/// complaining instead of drawing a plot when the frequencies don't
/// add up to 1.
///
/// Example:
/// @code
/// std::vector<float> distribution = {0.1f, 0.2f, 0.4f, 0.3f};
/// HistogramPlot plot("distribution.svg", "Distribution");
/// plot.draw(distribution);
/// plot.save();
/// @endcode
class HistogramPlot : private Chart {
public:
    /// Start a new histogram. Call draw() once, then save() (or
    /// just let the object be destroyed) to write it.
    /// @param filename SVG file to write.
    /// @param title plot title, shown above the bars; omitted if empty.
    /// @param tolerance largest allowed absolute difference between
    ///   the sum of draw()'s `frequencies` and 1 before it's treated
    ///   as an error.
    HistogramPlot(const std::string& filename,
                  const std::string& title = "",
                  float tolerance = 1e-6f);

    /// Writes the file if save() was never called.
    ~HistogramPlot();

    /// Set the bars to draw: one per entry of `frequencies`, or, if
    /// they don't add up to 1 (within the constructor's tolerance),
    /// an error shown instead of bars.
    /// @param frequencies relative frequencies to plot; must add up
    ///   to 1, since each value is drawn as a fraction of the
    ///   tallest bar.
    void draw(const std::vector<float>& frequencies);

    /// Write the SVG file now. Safe to call more than once, or not
    /// at all (the destructor then does it instead). draw() must
    /// have been called first.
    void save();

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
    static const float FONT_SIZE;

    /// Largest allowed absolute difference between the sum of
    /// m_frequencies and 1 before it's treated as an error.
    float m_tolerance;
    /// True once save() has written the file.
    bool m_saved;
    /// Relative frequencies to plot, set by draw().
    std::vector<float> m_frequencies;
};
