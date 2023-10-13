#include "scale.h"

const std::vector<ScaleType>& AllScaleTypes()
{
    static std::vector<ScaleType> allScaleTypes =
    {
        ScaleType::Major,
        ScaleType::Minor,
        ScaleType::Dorian,
        ScaleType::Phrygian,
        ScaleType::Lydian,
        ScaleType::Mixolydian,
        ScaleType::HarmonicMinor,
        ScaleType::DoubleHarmonic,
        ScaleType::CustomMushroomBlues
    };
    return allScaleTypes;
}

std::string ScaleTypeToString(ScaleType type)
{
    switch (type)
    {
    case ScaleType::Major: return "Major";
    case ScaleType::Minor: return "Minor";
    case ScaleType::Dorian: return "Dorian";
    case ScaleType::Phrygian: return "Phrygian";
    case ScaleType::Lydian: return "Lydian";
    case ScaleType::Mixolydian: return "Mixolydian";
    case ScaleType::HarmonicMinor: return "Harmonic minor";
    case ScaleType::DoubleHarmonic: return "Double harmonic";
    case ScaleType::CustomMushroomBlues: return "Custom - mushroom blues";
    }
    return std::string();
}

Scale::Scale(ScaleType scaleType)
    : m_scaleType(scaleType)
{
}

const std::list<Note> &Scale::GetNotes() const
{
    return m_notes;
}

void Scale::AddNote(const Note &note)
{
    if (m_notesSet.find(note) == m_notesSet.end())
    {
        m_notes.push_back(note);
        m_notesSet.insert(note);
    }
}

bool Scale::HasNote(const Note &note) const
{
    return m_notesSet.find(note) != m_notesSet.end();
}

void Scale::SetName(const std::string &name)
{
    m_name = name;
}

const std::string &Scale::GetName() const
{
    return m_name;
}

std::set<NoteType> Scale::ToNoteTypeSet() const
{
    std::set<NoteType> result;
    for (const auto& note : this->GetNotes())
    {
        result.insert(note.GetNoteType());
    }
    return result;
}




