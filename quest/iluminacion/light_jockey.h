#pragma once
#include <vector>
#include "light_point.h"

/// Read standard input (with std::cin) and produce a vector of lights.
/// The first input line contains an integer `n` with the number of lights.
/// The following `n` lines have the format `<color><on|off>`, where:
/// <color> are the 3 first letters of the color ("red", "blu", ...)
/// and <on|off> is either "on" or "off". An example input could be
///
/// 4
/// whion
/// bluoff
/// redoff
/// greenon
///
/// which would produce a vector with 4 lights (white on, blue off, red off, green off).
std::vector<LightPoint> receive_lights();

/// Receive a list of lights and then a sequence of student-defined commands.
/// In the end, return the resulting state lights.
std::vector<LightPoint> control_lights();