#include "document.h"

Document::Document(const std::string& content, const std::string& id)
    : m_sequence_number(0), m_content(content), m_id(id) {
}

Document::Document(const Document& other)
    : m_sequence_number(other.m_sequence_number + 1), m_content(other.m_content), m_id(other.m_id) {
}

std::string Document::get_content() const {
    return m_content;
}

void Document::append(const std::string& content) {
    m_content += content;
    m_sequence_number++;
}

std::string Document::get_id() const {
    return m_id;
}

unsigned Document::get_sequence_number() const {
    return m_sequence_number;
}

Document& Document::operator=(const Document& original) {
    if (this != &original) {
        m_sequence_number = original.m_sequence_number + 1;
        m_content = original.m_content;
        m_id = original.m_id;
    }

    return *this;
}

std::ostream& operator<<(std::ostream& out, const Document& document) {
    out << "---[Begin #" << document.get_sequence_number() << "]---" << std::endl;
    out << document.get_content() << std::endl;
    out << "---[End   #" << document.get_sequence_number() << "]---";
    return out;
}
