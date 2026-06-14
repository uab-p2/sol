#include "cat.h"

void Cat::feed() {
    m_hungry = false;
    m_sleepy = true;
}

bool Cat::pet() {
    bool success = false;
    if (!m_hungry) {
        success = true;
        m_sleepy = true;
        m_hungry = true;
    }
    return success;
}

void Cat::sleep() {
    m_sleepy = false;
    m_hungry = true;
}

std::string Cat::meow() const {
    std::string message = "Meow! (\"I am '" + m_name + "'.";
    if (m_sleepy) {
        message += " I am sleepy.";
    }
    if (m_hungry) {
        message += " AND I'M HUNGRY!";
    }
    message += "\")";
    return message;
}
