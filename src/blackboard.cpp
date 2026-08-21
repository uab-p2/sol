#include "blackboard.h"

Blackboard::Blackboard(const std::string& text)
    : m_text(text), m_chalks(1) {
}

std::string Blackboard::get_text() const {
    return m_text;
}

void Blackboard::set_text(const std::string& text) {
    m_text = text;
}

unsigned Blackboard::get_chalks() const {
    return m_chalks;
}

void Blackboard::set_chalks(unsigned chalks) {
    if (chalks <= 100) {
        m_chalks = chalks;
    }
}
