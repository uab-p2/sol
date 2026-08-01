#include "light_point.h"

LightPoint::LightPoint(bool on, const std::string& color, float intensity)
    : m_on(on), m_color(color), m_intensity(-1) {
    set_intensity(intensity);
}

void LightPoint::turn_on() {
    m_on = true;
    m_intensity = 1;
}

void LightPoint::turn_off() {
    m_on = false;
    m_intensity = 0;
}

std::string LightPoint::status() {
    return (m_on ? "On" : "Off") + std::string("-")
        + m_color + std::to_string(m_intensity);
}

bool LightPoint::is_on() const {
    return m_on;
}

std::string LightPoint::get_color() const {
    return m_color;
}

void LightPoint::set_color(const std::string& color) {
    m_color = color;
}

float LightPoint::get_intensity() const {
    return m_intensity;
}

void LightPoint::set_intensity(float intensity) {
    m_intensity = intensity;
}
