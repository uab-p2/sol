#pragma once
#include <string>
#include <iostream>

/// Append-only document with a sequence number.
class Document {
public:
    /// Create a document with sequence number zero.
    /// @param content text content
    /// @param id document id
    Document(const std::string& content, const std::string& id);

    /// Create a document that's an exact copy of another,
    /// but has the next sequence number.
    Document(const Document& other);

    /// @return the current document's content
    std::string get_content() const;
    /// Add some text to the current content and increase the sequence_number.
    /// @param content text to add to the document.
    void add_content(const std::string& content);

    /// @param id that is confirmed (or not)
    /// @return true if `id` is identical to the document's id, false otherwise.
    bool verify_id(const std::string& id) const;

    /// Get the current sequence number.
    unsigned get_sequence_number() const;

    /// Copy the original document into this one, keeping the sequence
    /// number one greater than the original's.
    Document& operator=(const Document& original);

protected:
    /// Sequence number is increased with each copy and each change.
    unsigned m_sequence_number;

    /// Content (no format enforced)
    std::string m_content;

    /// Name (immutable)
    std::string m_id;
};

/// Human readable representation of the document.
std::ostream& operator<<(std::ostream& out, const Document& document);
