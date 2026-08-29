#include "waypoint.h"

Waypoint::Waypoint(double x, double y, double z)
    : m_x(x), m_y(y), m_z(z) {
}

void Waypoint::get_position(double& x, double& y, double& z) const {
    x = m_x;
    y = m_y;
    z = m_z;
}

void Waypoint::set_position(double x, double y, double z) {
    m_x = x;
    m_y = y;
    m_z = z;
}
