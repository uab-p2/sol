#include "star.h"
#include <cmath>

Star::Star()
    : Star("Sol", 0, 0, 0, "yellow") {
}

Star::Star(const std::string& name, double x, double y, double z, double wavelength)
    : m_name(name), m_x(x), m_y(y), m_z(z), m_wavelength(wavelength) {
}

Star::Star(const std::string& name, double x, double y, double z, const std::string& type)
    : m_name(name), m_x(x), m_y(y), m_z(z), m_wavelength(0) {
    set_type(type);
}

double Star::distance(const Star& other) const {
    const double dx = m_x - other.m_x;
    const double dy = m_y - other.m_y;
    const double dz = m_z - other.m_z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

std::string Star::get_type() const {
    std::string type = "other";
    if (m_wavelength > 100 && m_wavelength <= 145) {
        type = "blue";
    }
    else if (m_wavelength <= 500) {
        type = "yellow";
    }
    else if (m_wavelength <= 970) {
        type = "red";
    }
    else if (m_wavelength <= 1100) {
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
    m_name = name;
    while (m_name.length() < MIN_NAME_LENGTH) {
        m_name = m_name + "0";
    }
}

void Star::get_position(double& x, double& y, double& z) const {
    x = m_x;
    y = m_y;
    z = m_z;
}

void Star::set_position(double x, double y, double z) {
    m_x = x;
    m_y = y;
    m_z = z;
}
