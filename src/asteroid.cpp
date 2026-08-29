#include "asteroid.h"
#include "random.h"
#include <algorithm>

Asteroid::Asteroid(unsigned amount, float price_per_unit)
    : m_amount(amount),
      m_price_per_unit(std::max(price_per_unit, 0.0f)) {
}

Asteroid::~Asteroid() {
}

unsigned Asteroid::mine() {
    const unsigned extracted = std::min(get_yield_per_mine(), m_amount);
    m_amount -= extracted;
    return extracted;
}

unsigned Asteroid::get_amount() const {
    return m_amount;
}

float Asteroid::get_price_per_unit() const {
    return m_price_per_unit;
}

IceAsteroid::IceAsteroid(unsigned amount, float price_per_unit)
    : Asteroid(amount, price_per_unit) {
}

unsigned IceAsteroid::mine() {
    unsigned extracted = 0;
    if (Random::get_bool(EXPLOSION_PROBABILITY)) {
        m_amount = 0;
    }
    else {
        extracted = Asteroid::mine();
    }
    return extracted;
}

unsigned IceAsteroid::get_yield_per_mine() const {
    return YIELD_PER_MINE;
}

RockAsteroid::RockAsteroid(unsigned amount, float price_per_unit)
    : Asteroid(amount, price_per_unit) {
}

unsigned RockAsteroid::get_yield_per_mine() const {
    return YIELD_PER_MINE;
}

MetalAsteroid::MetalAsteroid(unsigned amount, float price_per_unit)
    : Asteroid(amount, price_per_unit) {
}

unsigned MetalAsteroid::get_yield_per_mine() const {
    return YIELD_PER_MINE;
}
