#include "chordfactory.h"
#include "scales/scalefactory.h"

std::list<int> ChordFactory::GenerateChordOffsets(ChordType chordType)
{
    std::list<int> result;
    switch (chordType)
    {
    case ChordType::_5:
        result.push_back(0);
        result.push_back(7);
        break;
    case ChordType::Major:
        result.push_back(0);
        result.push_back(4);
        result.push_back(7);
        break;
    case ChordType::Minor:
        result.push_back(0);
        result.push_back(3);
        result.push_back(7);
        break;
    case ChordType::Dim:
        result.push_back(0);
        result.push_back(3);
        result.push_back(6);
        break;
    case ChordType::Dim7:
        result.push_back(0);
        result.push_back(3);
        result.push_back(6);
        result.push_back(9);
        break;
    case ChordType::Major6:
        result.push_back(0);
        result.push_back(4);
        result.push_back(7);
        result.push_back(9);
        break;
    case ChordType::Minor6:
        result.push_back(0);
        result.push_back(3);
        result.push_back(7);
        result.push_back(9);
        break;
    case ChordType::Major7:
        result.push_back(0);
        result.push_back(4);
        result.push_back(7);
        result.push_back(10);
        break;
    case ChordType::Minor7:
        result.push_back(0);
        result.push_back(3);
        result.push_back(7);
        result.push_back(10);
        break;
    case ChordType::MajorMaj7:
        result.push_back(0);
        result.push_back(4);
        result.push_back(7);
        result.push_back(11);
        break;
    case ChordType::MinorMaj7:
        result.push_back(0);
        result.push_back(3);
        result.push_back(7);
        result.push_back(11);
        break;
    case ChordType::Major9:
        result.push_back(0);
        result.push_back(4);
        result.push_back(7);
        result.push_back(10);
        result.push_back(12 + 2);
        break;
    case ChordType::Minor9:
        result.push_back(0);
        result.push_back(3);
        result.push_back(7);
        result.push_back(10);
        result.push_back(12 + 2);
        break;
    case ChordType::MajorAdd9:
        result.push_back(0);
        result.push_back(4);
        result.push_back(7);
        result.push_back(12 + 2);
        break;
    case ChordType::MinorAdd9:
        result.push_back(0);
        result.push_back(3);
        result.push_back(7);
        result.push_back(12 + 2);
        break;
    case ChordType::MajorAdd11:
        result.push_back(0);
        result.push_back(4);
        result.push_back(7);
        result.push_back(12 + 5);
        break;
    case ChordType::MinorAdd11:
        result.push_back(0);
        result.push_back(3);
        result.push_back(7);
        result.push_back(12 + 5);
        break;
    case ChordType::Sus2:
        result.push_back(0);
        result.push_back(2);
        result.push_back(7);
        break;
    case ChordType::Sus4:
        result.push_back(0);
        result.push_back(5);
        result.push_back(7);
        break;
    case ChordType::Aug:
        result.push_back(0);
        result.push_back(4);
        result.push_back(8);
        break;
    case ChordType::MajorAddb6:
        result.push_back(0);
        result.push_back(4);
        result.push_back(7);
        result.push_back(8);
        break;
    case ChordType::MinorAddb6:
        result.push_back(0);
        result.push_back(3);
        result.push_back(7);
        result.push_back(8);
        break;
    case ChordType::MajorAddb9:
        result.push_back(0);
        result.push_back(4);
        result.push_back(7);
        result.push_back(12 + 1);
        break;
    case ChordType::MinorAddb9:
        result.push_back(0);
        result.push_back(3);
        result.push_back(7);
        result.push_back(12 + 1);
        break;
    case ChordType::MajorAdds11:
        result.push_back(0);
        result.push_back(4);
        result.push_back(7);
        result.push_back(12 + 6);
        break;
    case ChordType::_5Adds11:
        result.push_back(0);
        result.push_back(7);
        result.push_back(12 + 6);
        break;
    }

    return result;
}

Chord ChordFactory::GenerateChord(const Note &baseNote, ChordType chordType)
{
    Chord result;
    result.SetBaseNote(baseNote);

    auto offsets = GenerateChordOffsets(chordType);
    for (const auto &obj : offsets)
    {
        result.AddNote(baseNote + obj);
    }

    // set name
    std::string chordName = NoteTypeToString(baseNote.GetNoteType());
    chordName += ChordTypeToSuffixString(chordType);
    result.SetName(chordName);

    return result;
}

std::list<Chord> ChordFactory::GenerateAllChordsForScale(const Note &scaleBaseNote, ScaleType scaleType)
{
    Scale scale = ScaleFactory::GenerateScale(scaleBaseNote, scaleType);
    std::list<Chord> validChords;

    // generate all known chords
    for (const NoteType nt : AllNoteTypes())
    {
        for (const ChordType ct : AllChordsTypes())
        {
            Chord chord = GenerateChord(Note(nt), ct);

            // check if the chord's notes belong to the scale
            bool chordIsValid = true;
            for (const Note& note : chord.GetNotes())
            {
                if (!scale.HasNote(note))
                {
                    chordIsValid = false;
                    break;
                }
            }

            // if chord is valid then add it to the list
            if (chordIsValid)
            {
                validChords.push_back(chord);
            }
        }
    }

    return validChords;
}
