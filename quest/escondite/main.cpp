#include <iostream>
#include <vector>
#include "clock.h"
#include "hunt.h"
#include "int_vector_skills.h"
using namespace std;

/// Build a row of 20M nanobots.
vector<int> get_nanobot_row() {
    const int NANOBOT_COUNT = 20'000'000;
    vector<int> ids(NANOBOT_COUNT);
    for (int i = 0; i < NANOBOT_COUNT; i++) {
        ids[i] = i;
    }
    return ids;
}

int main() {
    const int SEARCH_COUNT = 10;

    // First case
    {
        vector<int> ids = get_nanobot_row();
        WallClock clock;
        clock.start();
        for (int i = 0; i < SEARCH_COUNT; i++) {
            hunt_unordered(ids, ids.size()); // does not exist
        }
        cout << "Time case 1: " << fixed << clock.get_seconds() << " s" << endl;         
    }

    // Second case
    {
        vector<int> ids = get_nanobot_row();
        WallClock clock;
        clock.start();
        for (int i = 0; i < SEARCH_COUNT; i++) {
            hunt_ascending(ids, ids.size()); // does not exist
        }
        cout << "Time case 2: " << fixed << clock.get_seconds() << " s" << endl;         
    }

    return 0;
}