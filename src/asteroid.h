#pragma once

/// Abstract asteroid that can be mined for resources,
/// base of all implemented asteroid types.
class Asteroid {
public:
    /// Create an asteroid with an initial amount of `amount`
    /// mineable resource units, priced at `price_per_unit`.
    Asteroid(unsigned amount, float price_per_unit);

    /// Needed because virtual methods are defined.
    virtual ~Asteroid();

    /// Extract resources from the asteroid in a single
    /// mining pass. At most `get_yield_per_mine()` units are
    /// extracted, and never more than what is left to mine.
    /// @return the amount of resource units actually
    ///   extracted (it may be less than `get_yield_per_mine()`
    ///   if the asteroid is close to depletion, and 0 if the
    ///   asteroid is already depleted).
    virtual unsigned mine();

    /// @return the amount of resource units currently
    ///   left in the asteroid, available to be mined.
    unsigned get_amount() const;

    /// @return the (constant) price of a single resource unit.
    float get_price_per_unit() const;

protected:
    /// @return the maximum number of resource units that a
    ///   single call to `mine` can extract. Different asteroid
    ///   types yield different amounts.
    virtual unsigned get_yield_per_mine() const = 0;

    /// Amount of resource units left in the asteroid.
    unsigned m_amount;

    /// Price of a single resource unit, set at construction.
    const float m_price_per_unit;
};

/// Asteroid made mostly of ice, easy and fast to mine, but
/// prone to explode while being mined.
class IceAsteroid : public Asteroid {
public:
    /// Create an ice asteroid with an initial amount of
    /// `amount` mineable ice units, priced at `price_per_unit`.
    IceAsteroid(unsigned amount, float price_per_unit);

    /// Extract ice from the asteroid as in `Asteroid::mine`,
    /// but every call has a `EXPLOSION_PROBABILITY` chance of
    /// making the asteroid explode: if it does, the asteroid
    /// becomes fully depleted and no ice is yielded this time.
    unsigned mine() override;

protected:
    /// @return the maximum number of ice units that a
    ///   single call to `mine` can extract.
    unsigned get_yield_per_mine() const override;

    /// Ice units yielded by every successful (non-exploding) mine.
    static const unsigned YIELD_PER_MINE = 50;

    /// Probability that a mine call makes the asteroid explode.
    static constexpr double EXPLOSION_PROBABILITY = 0.05;
};

/// Asteroid made mostly of rock, slow and hard to mine.
class RockAsteroid : public Asteroid {
public:
    /// Create a rock asteroid with an initial amount of
    /// `amount` mineable rock units, priced at `price_per_unit`.
    RockAsteroid(unsigned amount, float price_per_unit);

protected:
    /// @return the maximum number of rock units that a
    ///   single call to `mine` can extract.
    unsigned get_yield_per_mine() const override;

    /// Rock units yielded by every successful mine.
    static const unsigned YIELD_PER_MINE = 20;
};

/// Asteroid made mostly of metal, dense and even harder to
/// mine than rock.
class MetalAsteroid : public Asteroid {
public:
    /// Create a metal asteroid with an initial amount of
    /// `amount` mineable metal units, priced at `price_per_unit`.
    MetalAsteroid(unsigned amount, float price_per_unit);

protected:
    /// @return the maximum number of metal units that a
    ///   single call to `mine` can extract.
    unsigned get_yield_per_mine() const override;

    /// Metal units yielded by every successful mine.
    static const unsigned YIELD_PER_MINE = 10;
};
