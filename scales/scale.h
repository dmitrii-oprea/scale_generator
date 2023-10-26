#ifndef SCALE_H
#define SCALE_H

#include <list>
#include <vector>
#include <string>
#include <set>
#include "notes/note.h"
#include <algorithm>

enum class ScaleType
{
    Major,
    Minor,
    Dorian,         // pink floyd
    Phrygian,       // metallica
    Lydian,         // sympsons
    Mixolydian,     // coldplay
    HarmonicMinor,
    DoubleHarmonic,
    JapaneseInsen,

    CustomMushroomBlues // porcupine tree: up to downstair
};

std::string ScaleTypeToString(ScaleType type);
const std::vector<ScaleType>& AllScaleTypes();

class Scale
{
    ScaleType m_scaleType;
    std::list<Note> m_notes;
    std::string m_name;
    std::set<Note> m_notesSet;

public:
    Scale(ScaleType scaleType);
    const std::list<Note>& GetNotes() const;
    void AddNote(const Note& note);
    bool HasNote(const Note &note) const;
    void SetName(const std::string& name);
    const std::string& GetName() const;
    std::set<NoteType> ToNoteTypeSet() const;
};

#endif // SCALE_H
