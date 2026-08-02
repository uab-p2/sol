/**
 * Example of how to model a light point WITHOUT using OOP.
 */
#include <iostream>
#include <string>
using namespace std;

/// Turn on a light point
void turn_on(bool& is_on, string& color, float& intensity);
/// Turn off a light point
void turn_off(bool& is_on, string& color, float& intensity);

int main() {
    // This is the light point
    bool is_on;
    string color;
    float intensity;

    turn_on(is_on, color, intensity);
    cout << (is_on ? "On" : "Off") << ":" << color << intensity << endl;
    turn_off(is_on, color, intensity);
    cout << (is_on ? "On" : "Off") << ":" << color << intensity << endl;

    return 0;
}

void turn_on(bool& is_on, string& color, float& intensity) {
    is_on = true;
    color = "white";
    intensity = 0.75;
}

void turn_off(bool& is_on, string& color, float& intensity) {
    is_on = false;
    color = "none";
    intensity = 0;
}