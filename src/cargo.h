#pragma once
#include "ship.h"

/// Cargo transportation ship.
class Cargo : public Ship {
public:
    /// Create a cargo ship carrying `units` cargo units, estimated
    /// to be worth `estimated_cost` (in some currency).
    /// @param units: the initial number of cargo units in the ship.
    /// @param estimated_cost: estimated total value of the cargo onboard.
    /// @param x,y,z: position where the cargo ship is created.
    Cargo(unsigned units, float estimated_cost = 0,
          double x = 0, double y = 0, double z = 0);

    /// @return the current number of cargo units onboard.
    unsigned get_cargo() const;

    /// @return the maximum number of cargo units
    ///   this ship can carry.
    virtual unsigned get_maximum_cargo() const;

    /// @return the estimated value of the cargo currently onboard.
    float get_estimated_cost() const;

    /// Set the estimated value of the cargo currently onboard.
    /// @param estimated_cost the new estimated value. Negative
    ///   values are clamped to 0.
    void set_estimated_cost(float estimated_cost);

    /// @return the value of the cargo per unit onboard
    ///   (`get_estimated_cost() / get_cargo_count()`), or 0
    ///   if the ship is not carrying any cargo.
    float get_money_density() const;

    /// Attempt to load more cargo onto the ship, but only
    /// if it all fits.
    /// @param units the number of cargo units that are
    ///   (or aren't) loaded onto the ship.
    /// @return true if the cargo was loaded, false if there
    ///   was not enough room, and nothing was loaded.
    virtual bool load_cargo(unsigned units);

    /// Attempt to unload the requested number of cargo units,
    /// but only if enough cargo is onboard. Otherwise,
    /// the cargo ship contents are not changed.
    /// @param units the number of cargo units to unload.
    virtual bool unload_cargo(unsigned units);

    /// Attempt to move the Cargo ship to (x,y,z).
    /// There must be enough battery (at least the cost reported
    /// by `move_cost`) or the move is not performed at all
    /// (and no battery is spent).
    /// @return true if the ship moved and consumed the battery. False if
    ///   the ship did not move nor spend any battery.
    bool move(double x, double y, double z) override;

protected:
    /// @return the amount of battery needed to go from the current
    ///   position to (x,y,z). The base cost is
    ///   10 + distance * units * cost_per_unit_per_distance, then
    ///   increased by an insurance surcharge proportional to
    ///   `get_money_density()`: valuable cargo is handled with
    ///   more care, and thus costs more battery to transport.
    virtual double move_cost(
        double x, double y, double z,
        double cost_per_unit_per_distance) const;

    /// Number of cargo units onboard the ship.
    unsigned m_cargo;

    /// Estimated value of the cargo currently onboard.
    float m_estimated_cost;

private:
    /// Maximum cargo loadable in this cargo ship.
    const unsigned MAXIMUM_CARGO = 50000;
};
