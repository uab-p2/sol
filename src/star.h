#pragma once
#include <string>

/**
 * Hot and spherical object in space.
 */
class Star {
public:
    /// Create a custom star given its wavelength
    /// @param name star name
    /// @param x,y,z position relative to Earth's sun.
    /// @param wavelength maximum emission wavelength in nm
    Star(const std::string& name, float x, float y, float z, float wavelength);

    /// Create a custom star given its type
    /// @param name star name
    /// @param x,y,z position relative to Earth's sun.
    /// @param type one of the possible valid type (see set_type)
    Star(const std::string& name, float x, float y, float z,
         const std::string& type);

    /// By default, create Earth's sun
    Star() : Star("sol", 0, 0, 0, "yellow") {
    }

    /// @return the type name based on the wavelength, or "other"
    std::string get_type() const;
    /// Set the star type. The maximum wavelength is automatically derived from
    /// it.
    /// @param type must be one of "blue", "yellow", "red", "brown".
    ///   Otherwise, it is ignored.
    void set_type(const std::string& type);

    /// @return the current star name
    std::string get_name() const { return m_name; }
    /// @param name new star name. If it is shorter than 5 characters,
    ///   a suffix of "0"s is automatically added
    void set_name(const std::string& name);

    /// @param x,y,z variables where the current star's position is saved.
    void get_position(float& x, float& y, float& z) const;
    /// @param x,y,z the position relative to the Earth's sun, in light years
    void set_position(float x, float y, float z);

    /// @return the star wavelength
    float get_wavelength() const { return m_wavelength; }

protected:
    /// Scientific or familiar name. Can include numbers (at least 5 characters).
    std::string m_name;
    /// Position relative to Earth's sun (facing front), in light years.
    float m_x, m_y, m_z;
    /// Maximum emission wavelength in nm.
    float m_wavelength;
};
