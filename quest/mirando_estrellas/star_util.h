#include "star.h"

/// @return the total length of the 3 sides of the triangle
///   defined by stars s1, s2 and s3.
double triangle_length(const Star& s1, const Star& s2, const Star& s3);

/// Smashes two stars together.
/// @param s1 first star. Must be moved to (0,0,0) after collision.
/// @param s2 second star. Must be moved to (0,0,0) after collision.
/// @return the new star, resulting from the collision
Star smash(Star& s1, Star& s2);