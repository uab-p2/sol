#include "shop.h"

Shop::Shop() : m_door_locked(true), m_lights_on(false) {
}

void Shop::open() {
    m_door_locked = false;
    m_lights_on = true;
}

void Shop::close() {
    m_door_locked = true;
    m_lights_on = false;
}
