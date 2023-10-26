#include "scalefactory.h"

Scale ScaleFactory::GenerateScale(const Note &baseNote, ScaleType scaleType)
{
    Scale result(scaleType);

    switch (scaleType)
    {
    case ScaleType::Major:
        result.AddNote(baseNote);
        result.AddNote(baseNote + 2);
        result.AddNote(baseNote + 4);
        result.AddNote(baseNote + 5);
        result.AddNote(baseNote + 7);
        result.AddNote(baseNote + 9);
        result.AddNote(baseNote + 11);
        break;
    case ScaleType::Minor:
        result.AddNote(baseNote);
        result.AddNote(baseNote + 2);
        result.AddNote(baseNote + 3);
        result.AddNote(baseNote + 5);
        result.AddNote(baseNote + 7);
        result.AddNote(baseNote + 8);
        result.AddNote(baseNote + 10);
        break;
    case ScaleType::Dorian:
        result.AddNote(baseNote);
        result.AddNote(baseNote + 2);
        result.AddNote(baseNote + 3);
        result.AddNote(baseNote + 5);
        result.AddNote(baseNote + 7);
        result.AddNote(baseNote + 9);
        result.AddNote(baseNote + 10);
        break;
    case ScaleType::Phrygian:
        result.AddNote(baseNote);
        result.AddNote(baseNote + 1);
        result.AddNote(baseNote + 3);
        result.AddNote(baseNote + 5);
        result.AddNote(baseNote + 7);
        result.AddNote(baseNote + 8);
        result.AddNote(baseNote + 10);
        break;
    case ScaleType::Lydian:
        result.AddNote(baseNote);
        result.AddNote(baseNote + 2);
        result.AddNote(baseNote + 4);
        result.AddNote(baseNote + 6);
        result.AddNote(baseNote + 7);
        result.AddNote(baseNote + 9);
        result.AddNote(baseNote + 11);
        break;
    case ScaleType::Mixolydian:
        result.AddNote(baseNote);
        result.AddNote(baseNote + 2);
        result.AddNote(baseNote + 4);
        result.AddNote(baseNote + 5);
        result.AddNote(baseNote + 7);
        result.AddNote(baseNote + 9);
        result.AddNote(baseNote + 10);
        break;
    case ScaleType::HarmonicMinor:
        result.AddNote(baseNote);
        result.AddNote(baseNote + 2);
        result.AddNote(baseNote + 3);
        result.AddNote(baseNote + 5);
        result.AddNote(baseNote + 7);
        result.AddNote(baseNote + 8);
        result.AddNote(baseNote + 11);
        break;
    case ScaleType::DoubleHarmonic:
        result.AddNote(baseNote);
        result.AddNote(baseNote + 1);
        result.AddNote(baseNote + 4);
        result.AddNote(baseNote + 5);
        result.AddNote(baseNote + 7);
        result.AddNote(baseNote + 8);
        result.AddNote(baseNote + 11);
        break;
    case ScaleType::JapaneseInsen:
        result.AddNote(baseNote);
        result.AddNote(baseNote + 1);
        result.AddNote(baseNote + 5);
        result.AddNote(baseNote + 7);
        result.AddNote(baseNote + 10);
        break;
    case ScaleType::CustomMushroomBlues:
        result.AddNote(baseNote);
        result.AddNote(baseNote + 3);
        result.AddNote(baseNote + 5);
        result.AddNote(baseNote + 6);
        result.AddNote(baseNote + 7);
        result.AddNote(baseNote + 10);
        result.AddNote(baseNote + 11);
        break;
    }

    // set name
    std::string scaleName = NoteTypeToString(baseNote.GetNoteType());
    scaleName += " - ";
    scaleName += ScaleTypeToString(scaleType);
    result.SetName(scaleName);

    return result;
}
