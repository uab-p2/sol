#include "elevator.h"

Elevator::Elevator(int top_floor) : m_top_floor(top_floor) {
}

void Elevator::push_button(int floor) {
    if (floor >= 1 && floor <= m_top_floor) {
        move_box(floor);
    }
}

int Elevator::get_current_floor() const {
    return m_current_floor;
}

void Elevator::move_box(int floor) {
    m_current_floor = floor;
}
