#include "ship.h"

#include <cmath>

Ship::~Ship() {
}

Ship::Ship(double battery_charge,
           double x, double y, double z)
    : m_x(x), m_y(y), m_z(z), m_battery(battery_charge) {
}

void Ship::get_position(double& x, double& y, double& z) const {
    x = m_x;
    y = m_y;
    z = m_z;
}

bool Ship::is_working() const {
    return m_battery > 0;
}

bool Ship::move(double x, double y, double z) {
    m_x = x;
    m_y = y;
    m_z = z;
    return true;
}

double Ship::distance_to(double x, double y, double z) const {
    double dx = m_x - x;
    double dy = m_y - y;
    double dz = m_z - z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}
