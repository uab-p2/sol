#pragma once

/// A simple elevator serving floors from 1 to a fixed top floor.
class Elevator {
public:
    /// Create an elevator serving floors 1 to top_floor.
    /// @param top_floor highest floor served (must be >= 1).
    Elevator(int top_floor);

    /// Request the elevator to go to the given floor. Ignored if
    /// floor is not between 1 and the top floor.
    /// @param floor destination floor.
    void push_button(int floor);

    /// @return the floor where the elevator currently is.
    int get_current_floor() const;

private:
    /// Move the elevator box straight to the given floor, without
    /// any validation. Only push_button() may call this, after
    /// checking that the floor is valid.
    /// @param floor destination floor.
    void move_box(int floor);

    /// Current floor. Starts at the ground floor (1).
    int m_current_floor = 1;

    /// Highest floor served by this elevator.
    int m_top_floor;
};
