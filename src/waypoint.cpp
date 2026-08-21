#include "waypoint.h"

Waypoint::Waypoint(double x, double y, double z)
    : m_x(x), m_y(y), m_z(z) {
}

void Waypoint::set_position(double x, double y, double z) {
    m_x = x;
    m_y = y;
    m_z = z;
}

double Waypoint::get_x() const {
    return m_x;
}

double Waypoint::get_y() const {
    return m_y;
}

double Waypoint::get_z() const {
    return m_z;
}
