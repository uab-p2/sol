#pragma once

/// Have a little chat with the monster with the green helmet
void talk_to_green();

/// Ask green about passing grades
/// @return whether the grade is sufficient to pass
bool is_passing_grade(int minimum, float grade);

/// Tell Green you could pay him `payment` euros
/// every day for `days` days.
/// @return the total bribe amount
int propose_bribe(int payment, int days);

