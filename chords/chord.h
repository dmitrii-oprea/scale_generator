#ifndef CHORD_H
#define CHORD_H

#include <list>
#include <optional>
#include <vector>
#include <string>
#include <set>
#include "notes/note.h"
#include <algorithm>
#include <optional>

enum class ChordType
{
    _5,
    Major,
    Minor,
    Dim,
    Dim7,
    Major6,
    Minor6,
    Major7,
    Minor7,
    Major9,
    Minor9,
    MajorMaj7,
    MinorMaj7,
    MajorAdd9,
    MinorAdd9,
    MajorAdd11,
    MinorAdd11,
    Sus2,
    Sus4,
    Aug,
    MajorAddb6,
    MinorAddb6,
    MajorAddb9,
    MinorAddb9,
    MajorAdds11,
    _5Adds11
};

std::string ChordTypeToString(ChordType type);
std::string ChordTypeToSuffixString(ChordType type);
const std::vector<ChordType>& AllChordsTypes();

class Chord
{
    std::optional<Note> m_baseNote;
    std::list<Note> m_notes;
    std::string m_name;

public:
    Chord();
    Chord(const Chord& obj);
    const std::list<Note>& GetNotes() const;
    void AddNote(const Note& note);
    void SetName(const std::string& name);
    const std::string& GetName() const;
    void SetBaseNote(Note baseNote);
    const std::optional<Note> GetBaseNote() const;

    std::set<NoteType> ToNoteTypeSet() const;
    bool operator == (const Chord& right) const;
    bool operator <= (const Chord& right) const;
};

#endif // CHORD_H
