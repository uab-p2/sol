#pragma once
#include <string>

/// Have a little chat with the monster with the red helmet
void talk_to_red();

/// Ask red if he knows a bird with a given name.
/// @return whether red thinks the name refers to a bird
bool is_a_bird(const std::string& name);