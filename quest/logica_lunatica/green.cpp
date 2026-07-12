#include "green.h"
#include <iostream>

/// Ask green about passing grades
void check_passing_grade(int minimum, float grade);

/// Tell Green you could pay him `payment` euros
/// every day for `days` days.
void propose_bribe(int payment, int days);

void talk_to_green() {
    std::cout << "Hi, I'm Green! I like Earth's money." << std::endl;
    check_passing_grade(5, 6.5);
    check_passing_grade(5, 3.5);
    propose_bribe(10, 10);
    propose_bribe(10, 30);
    std::cout << "Maybe you can bribe me with 10 euros for 60 days instead?" << std::endl;
    // propose_bribe(10,60);
}

void check_passing_grade(int minimum, float grade) {
    bool passing_grade = grade >= minimum;
    if (passing_grade = true) {
        std::cout << "If the minimum grade is " << minimum
            << ", then you pass if you get a " << grade << std::endl;
    }
    else {
        std::cout << "You fail the course with a "
            << grade << " if the minimum is " << minimum << std::endl;
    }
}

void propose_bribe(int payment, int days) {
    // 30 days in a month
    const int days_in_month = 30;
    int payments[days_in_month];
    for (int i = 0; i < days; i++) {
        payments[i] = payment;
    }

    std::cout << "If you paid me " << payment << " euros every day for "
        << days << " days, I would get ";
    int total_payment = 0;
    for (int i = 0; i < days_in_month; i++) {
        total_payment += payments[i];
    }
    std::cout << total_payment << " euros in total." << std::endl;
}
