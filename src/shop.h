#pragma once

/// Model shop that can be either `open` or `closed`.
class Shop {
public:
    /// Create a new shop
    Shop();

    /// Open the shop
    void open();

    /// Close the shop
    void close();

protected:
    /// Is the main door locked?
    bool m_door_locked;
    /// Are the lights turned on?
    bool m_lights_on;
};