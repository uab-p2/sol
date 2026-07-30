#include "star.h"

Star::Star(const std::string& name, float x, float y, float z, float wavelength) :
    m_name(name), m_x(x), m_y(y), m_z(z), m_wavelength(wavelength) {
}

Star::Star(const std::string& name, float x, float y, float z, const std::string& type) :
    m_name(name), m_x(x), m_y(y), m_z(z), m_wavelength(0) {
    set_type(type);
}

std::string Star::get_type() const {
    std::string type = "other";
    if (m_wavelength == 145) {
        type = "blue";
    }
    else if (m_wavelength == 500) {
        type = "yellow";
    }
    else if (m_wavelength == 970) {
        type = "red";
    }
    else if (m_wavelength == 1100) {
        type = "brown";
    }
    return type;
}

void Star::set_type(const std::string& type) {
    if (type == "blue") {
        m_wavelength = 145; // ~20000 K (ultraviolet peak)
    }
    else if (type == "yellow") {
        m_wavelength = 500; // ~5800 K (Sun-like)
    }
    else if (type == "red") {
        m_wavelength = 970; // ~3000 K
    }
    else if (type == "brown") {
        m_wavelength = 1100; // ~2600 K (brown dwarf, mostly infrared)
    }
    // Otherwise, ignored
}

void Star::set_name(const std::string& name) {
    const unsigned MIN_LENGTH = 5;
    m_name = name;
    while (m_name.length() < MIN_LENGTH) {
        m_name = m_name + "0";
    }
}

void Star::get_position(float& x, float& y, float& z) const {
    x = m_x;
    y = m_y;
    z = m_z;
}

void Star::set_position(float x, float y, float z) {
    m_x = x;
    m_y = y;
    m_z = z;
}
