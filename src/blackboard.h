#pragma once
#include <string>

/// A simple text blackboard.
class Blackboard {
public:
    /// Create a blackboard with the given text.
    /// @param text the initial text.
    Blackboard(const std::string& text);

    /// @return the current text.
    std::string get_text() const;

    /// @param text the new text.
    void set_text(const std::string& text);

    /// @return the number of chalks available.
    unsigned get_chalks() const;

    /// @param chalks the new number of chalks available.
    ///   Must be 100 or less, or it is reduced to 100.
    void set_chalks(unsigned chalks);

private:
    /// Text written on the blackboard.
    std::string m_text;

    /// Number of chalks available.
    unsigned m_chalks;
};
