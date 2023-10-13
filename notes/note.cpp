#include "note.h"

std::vector<NoteType>& AllNoteTypes()
{
    static std::vector<NoteType> allnotes = {
        NoteType::A,
        NoteType::A_,
        NoteType::B,
        NoteType::C,
        NoteType::C_,
        NoteType::D,
        NoteType::D_,
        NoteType::E,
        NoteType::F,
        NoteType::F_,
        NoteType::G,
        NoteType::G_
    };
    return allnotes;
}

std::string NoteTypeToString(NoteType n)
{
    switch (n)
    {
    case NoteType::A: return "A";
    case NoteType::A_: return "A#";
    case NoteType::B: return "B";
    case NoteType::C: return "C";
    case NoteType::C_: return "C#";
    case NoteType::D: return "D";
    case NoteType::D_: return "D#";
    case NoteType::E: return "E";
    case NoteType::F: return "F";
    case NoteType::F_: return "F#";
    case NoteType::G: return "G";
    case NoteType::G_: return "G#";
    }

    return std::string();
}

Note::Note(NoteType note)
    : m_note(note)
{
}

Note::Note(const Note &obj)
{
    m_note = obj.m_note;
}

NoteType Note::GetNoteType() const
{
    return m_note;
}

Note Note::operator =(const Note &right)
{
    this->m_note = right.m_note;
    return *this;
}

bool Note::operator ==(const Note &right) const
{
    return right.m_note == m_note;
}

Note Note::operator +(int offset) const
{
    int i = ((int)m_note + offset) % AllNoteTypes().size();
    return Note((NoteType)i);
}

bool Note::operator <(const Note &right) const
{
    return (int)m_note < (int)right.m_note;
}


