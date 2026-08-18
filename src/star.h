#pragma once
#include <string>

///
/// Hot and spherical object in space.
///
class Star {
public:
    /// By default, create Earth's sun
    Star();

    /// Create a custom star given its wavelength
    /// @param name star name
    /// @param x,y,z position relative to Earth's sun.
    /// @param wavelength maximum emission wavelength in nm
    Star(const std::string& name, double x, double y, double z, double wavelength);

    /// Create a custom star given its type
    /// @param name star name
    /// @param x,y,z position relative to Earth's sun.
    /// @param type one of the possible valid types (see set_type)
    Star(const std::string& name, double x, double y, double z,
         const std::string& type);

    /// @param other the other star to get the distance to
    /// @return the distance between this and the other star, in light years
    double distance(const Star& other) const;

    /// @return the type name based on the wavelength, or "other"
    std::string get_type() const;
    /// Set the star type. The maximum wavelength is automatically derived from
    /// it.
    /// @param type must be one of "blue", "yellow", "red", "brown".
    ///   Otherwise, it is ignored.
    void set_type(const std::string& type);

    /// @return the current star name
    std::string get_name() const { return m_name; }
    /// @param name new star name. If it is shorter than a minimum,
    /// "0" is appended until it isn't.
    void set_name(const std::string& name);

    /// @param x,y,z variables where the current star's position is saved.
    void get_position(double& x, double& y, double& z) const;
    /// @param x,y,z the position relative to the Earth's sun, in light years
    void set_position(double x, double y, double z);

    /// @return the star wavelength
    double get_wavelength() const { return m_wavelength; }

protected:
    /// Minimum name length.
    const unsigned MIN_NAME_LENGTH = 5;

    /// Scientific or familiar name. Can include numbers (at least 5 characters).
    std::string m_name;
    /// Position relative to Earth's sun (facing front), in light years.
    double m_x, m_y, m_z;
    /// Maximum emission wavelength in nm.
    double m_wavelength;
};
