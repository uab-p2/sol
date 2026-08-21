#pragma once

/// A point in 3D space, used to mark a spot along a route.
class Waypoint {
public:
    /// Create a waypoint in space at position (x,y,z).
    /// @param x,y,z: the position where the waypoint is placed.
    Waypoint(double x=0, double y=0, double z=0);

    /// Move the waypoint to new coordinates.
    void set_position(double x, double y, double z);

    /// @return the x coordinate.
    double get_x() const;

    /// @return the y coordinate.
    double get_y() const;

    /// @return the z coordinate.
    double get_z() const;

private:
    /// X coordinate.
    double m_x;
    /// Y coordinate.
    double m_y;
    /// Z coordinate.
    double m_z;
};
