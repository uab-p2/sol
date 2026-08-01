#pragma once
#include <string>

/// A singular point of light
class LightPoint {
public:
    /// Create a light point keeping intensity between 0.0 and 1.0
    LightPoint(bool on, const std::string& color, float intensity);

    /// Turn on the light
    void turn_on();
    /// Turn off the light
    void turn_off();

    /// @return light status as a string
    std::string status();

    /// @return true if the light is on, false otherwise
    bool is_on() const;

    /// @return the current emission color
    std::string get_color() const;
    /// @param color the new color of the light
    void set_color(const std::string& color);

    /// @return the intensity of the light
    float get_intensity() const;
    /// Set the light intensity.
    /// @param intensity the new intensity of the light
    void set_intensity(float intensity);

protected:
    /// True of the light is on
    bool m_on;
    /// Emission color
    std::string m_color;
    /// Intensity (0.0-1.0)
    float m_intensity;
};