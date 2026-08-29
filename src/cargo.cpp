#include "cargo.h"
#include <algorithm>

Cargo::Cargo(unsigned units, float estimated_cost,
             double x, double y, double z)
    : Ship(100, x, y, z), m_cargo(units), m_estimated_cost(0) {
    m_cargo = std::min(m_cargo, get_maximum_cargo());
    set_estimated_cost(estimated_cost);
}

unsigned Cargo::get_cargo() const {
    return m_cargo;
}

unsigned Cargo::get_maximum_cargo() const {
    return MAXIMUM_CARGO;
}

float Cargo::get_estimated_cost() const {
    return m_estimated_cost;
}

void Cargo::set_estimated_cost(float estimated_cost) {
    m_estimated_cost = std::max(estimated_cost, 0.0f);
}

float Cargo::get_money_density() const {
    return m_cargo == 0 ? 0 : m_estimated_cost / static_cast<float>(m_cargo);
}

bool Cargo::load_cargo(unsigned units) {
    bool success = true;
    if (units > get_maximum_cargo() - m_cargo) {
        success = false;
    }

    if (success) {
        m_cargo += units;
    }

    return success;
}

bool Cargo::unload_cargo(unsigned units) {
    bool success = true;
    if (units > m_cargo) {
        success = false;
    }

    if (success) {
        m_cargo -= units;
    }

    return success;
}

bool Cargo::move(double x, double y, double z) {
    const double cost_per_unit_per_distance = 1 / 500.0;
    const double cost = move_cost(x, y, z, cost_per_unit_per_distance);

    // Move only if we have enough battery and are not broken
    bool success = false;
    if (is_working() && cost <= m_battery) {
        m_battery -= cost;
        Ship::move(x, y, z);
        success = true;
    }
    return success;
}

double Cargo::move_cost(double x, double y, double z,
                         double cost_per_unit_per_distance) const {
    const double fixed_cost = 10;
    const double distance = distance_to(x, y, z);
    const double base_cost =
        fixed_cost + distance * m_cargo * cost_per_unit_per_distance;

    const double insurance_cost_per_money_unit = 1 / 10000.0;
    const double insurance_surcharge =
        base_cost * static_cast<double>(get_money_density()) * insurance_cost_per_money_unit;

    return base_cost + insurance_surcharge;
}
