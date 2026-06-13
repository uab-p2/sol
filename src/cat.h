#pragma once
#include <string>

/// A meowing creature
class Cat {
public:
    /// Create a hungry but awake cat
    Cat(const std::string& name) : m_name(name), m_hungry(true), m_sleepy(false) {
    }

    /// Feed the cat, making it not hungry, but sleepy.
    void feed();

    /// Pet the cat. If the cat is not hungry, it will play and become both hungry and sleepy.
    /// If hungry, the cat will refuse to play and won't become sleepy.
    /// @return whether or not you managed to pet the cat.
    bool pet();

    /// Let the cat sleep. It will no longer be sleepy, but will become hungry.
    void sleep();

    /// Ask the cat to tell you something.
    /// @return the thing the cat wants to say.
    std::string meow();

    /// @return the cat's name
    const std::string& name() const { return m_name; }
    /// @return true if the cat is hungry
    bool hungry() const { return m_hungry; }
    /// @return true if the cat is sleepy
    bool sleepy() const { return m_sleepy; }

protected:
    // Cat's name
    std::string m_name;
    // Is the cat hungry?
    bool m_hungry;
    // Is the cat sleepy?
    bool m_sleepy;
};
