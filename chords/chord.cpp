#include "chord.h"

const std::vector<ChordType>& AllChordsTypes()
{
    static std::vector<ChordType> allChords =
    {
        ChordType::Major,
        ChordType::Minor,
        ChordType::_5,
        ChordType::_5b5,
        ChordType::Dim,
        ChordType::Dim7,
        ChordType::Major6,
        ChordType::Minor6,
        ChordType::Major7,
        ChordType::Minor7,
        ChordType::Major9,
        ChordType::Minor9,
        ChordType::MajorMaj7,
        ChordType::MinorMaj7,
        ChordType::MajorAdd9,
        ChordType::MinorAdd9,
        ChordType::MajorAdd11,
        ChordType::MinorAdd11,
        ChordType::Sus2,
        ChordType::Sus2Addb6,
        ChordType::Sus4,
        ChordType::Sus4Addb6,
        ChordType::Aug,
        ChordType::MajorAddb6,
        ChordType::MinorAddb6,
        ChordType::MajorAddb9,
        ChordType::MinorAddb9,
        ChordType::MajorAdds11,
        ChordType::_5Adds11,
        ChordType::MajorMaj7Adds11,
        ChordType::MajorMaj7sus2
    };
    return allChords;
}

std::string ChordTypeToString(ChordType type)
{
    switch (type)
    {
    case ChordType::Major: return "major";
    case ChordType::Minor: return "minor";
    case ChordType::_5: return "5";
    case ChordType::_5b5: return "5b5";
    case ChordType::Dim: return "dim";
    case ChordType::Dim7: return "dim7";
    case ChordType::Major6: return "major 6";
    case ChordType::Minor6: return "minor 6";
    case ChordType::Major7: return "major 7";
    case ChordType::Minor7: return "minor 7";
    case ChordType::Major9: return "major 9";
    case ChordType::Minor9: return "minor 9";
    case ChordType::MajorMaj7: return "major maj7";
    case ChordType::MinorMaj7: return "minor maj7";
    case ChordType::MajorAdd9: return "major add9";
    case ChordType::MinorAdd9: return "minor add9";
    case ChordType::MajorAdd11: return "major add11";
    case ChordType::MinorAdd11: return "minor add11";
    case ChordType::Sus2: return "sus2";
    case ChordType::Sus2Addb6: return "sus2 addb6";
    case ChordType::Sus4: return "sus4";
    case ChordType::Sus4Addb6: return "sus4 addb6";
    case ChordType::Aug: return "aug";
    case ChordType::MajorAddb6: return "major addb6";
    case ChordType::MinorAddb6: return "minor addb6";
    case ChordType::MajorAddb9: return "major addb9";
    case ChordType::MinorAddb9: return "minor addb9";
    case ChordType::MajorAdds11: return "major add#11";
    case ChordType::_5Adds11: return "5 add#11";
    case ChordType::MajorMaj7Adds11: return "maj7 add#11";
    case ChordType::MajorMaj7sus2: return "maj7 sus2";
    }
    return std::string();
}

std::string ChordTypeToSuffixString(ChordType type)
{
    switch (type)
    {
    case ChordType::Major: return "";
    case ChordType::Minor: return "m";
    case ChordType::_5: return "5";
    case ChordType::_5b5: return "5b5";
    case ChordType::Dim: return "dim";
    case ChordType::Dim7: return "dim7";
    case ChordType::Major6: return "6";
    case ChordType::Minor6: return "m6";
    case ChordType::Major7: return "7";
    case ChordType::Minor7: return "m7";
    case ChordType::Major9: return "9";
    case ChordType::Minor9: return "m9";
    case ChordType::MajorMaj7: return "maj7";
    case ChordType::MinorMaj7: return "mmaj7";
    case ChordType::MajorAdd9: return "add9";
    case ChordType::MinorAdd9: return "madd9";
    case ChordType::MajorAdd11: return "add11";
    case ChordType::MinorAdd11: return "madd11";
    case ChordType::Sus2: return "sus2";
    case ChordType::Sus2Addb6: return "sus2addb6";
    case ChordType::Sus4: return "sus4";
    case ChordType::Sus4Addb6: return "sus4addb6";
    case ChordType::Aug: return "aug";
    case ChordType::MajorAddb6: return "addb6";
    case ChordType::MinorAddb6: return "maddb6";
    case ChordType::MajorAddb9: return "addb9";
    case ChordType::MinorAddb9: return "maddb9";
    case ChordType::MajorAdds11: return "add#11";
    case ChordType::_5Adds11: return "5add#11";
    case ChordType::MajorMaj7Adds11: return "maj7add#11";
    case ChordType::MajorMaj7sus2: return "maj7sus2";
    }
    return std::string();
}

Chord::Chord()
{
}

Chord::Chord(const Chord &obj)
    : m_baseNote(obj.m_baseNote)
    , m_notes(obj.m_notes)
    , m_name(obj.m_name)
{
}

const std::list<Note> &Chord::GetNotes() const
{
    return m_notes;
}

void Chord::AddNote(const Note &note)
{
    auto it = std::find(m_notes.begin(), m_notes.end(), note);
    if (it == m_notes.end())
    {
        m_notes.push_back(note);
    }
}

void Chord::SetName(const std::string &name)
{
    m_name = name;
}

const std::string &Chord::GetName() const
{
    return m_name;
}

void Chord::SetBaseNote(Note baseNote)
{
    m_baseNote = baseNote;
}

const std::optional<Note> Chord::GetBaseNote() const
{
    return m_baseNote;
}

std::set<NoteType> Chord::ToNoteTypeSet() const
{
    std::set<NoteType> result;
    for (const auto& note : this->GetNotes())
    {
        result.insert(note.GetNoteType());
    }
    return result;
}

bool Chord::operator ==(const Chord &right) const
{
    std::set<NoteType> leftChordNotes = this->ToNoteTypeSet();
    std::set<NoteType> rightChordNotes = right.ToNoteTypeSet();

    return leftChordNotes == rightChordNotes;
}

bool Chord::operator <=(const Chord &right) const
{
    std::set<NoteType> leftChordNotes = this->ToNoteTypeSet();
    std::set<NoteType> rightChordNotes = right.ToNoteTypeSet();

    // check if all the notes of left chord belongs to right chord
    bool result = true;
    for (auto nt : leftChordNotes)
    {
        if (rightChordNotes.find(nt) == rightChordNotes.end())
        {
            result = false;
            break;
        }
    }
    return result;
}
