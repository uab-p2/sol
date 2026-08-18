#pragma once

/// Abstract ship for simulations, base of all implemented models.
class Ship {
public:
    /// Needed because virtual methods are defined.
    virtual ~Ship();

    /// Get the current position of the ship, expressed in meters.
    /// @param x,y,z variables where the ship's position is saved.
    void get_position(double& x, double& y, double& z) const;

    /// @return true if and only if the health and the battery
    ///   are strictly above zero.
    virtual bool is_working() const;

    /// Attempt to move the ship to the destination (x,y,z).
    /// The base Ship class simply changes position without
    /// affecting the battery charge. Subclasses may modify this behavior.
    /// @param x,y,z destination
    /// @return true if the ship moved to the destination
    ///   (potentially using battery charge), or false
    ///   if the ship could not move to the destination
    ///   (e.g., no fuel, no health, it's a fixed station...).
    virtual bool move(double x, double y, double z);

protected:
    /// Create a ship with the specified battery charge and position.
    Ship(double battery_charge, double x=0, double y=0, double z=0);

    /// @return the distance from the current position to (x,y,z), in distance units.
    double distance_to(double x, double y, double z) const;

    /// Ship position with respect to some reference point.
    double m_x, m_y, m_z;

    /// Current battery charge.
    double m_battery;
};
