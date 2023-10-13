#include "chordguesser.h"

ChordGuesser::ChordGuesser()
{
    // init all chords
    for (auto& noteType : AllNoteTypes())
    {
        for (auto& chordType : AllChordsTypes())
        {
            Chord chord = ChordFactory::GenerateChord(Note(noteType), chordType);
            m_allKnownChords.push_back(chord);
        }
    }
}

std::list<Chord> ChordGuesser::Guess(const Chord &chord) const
{
    std::list<Chord> result;

    // find similar chords
    for (const auto &c : m_allKnownChords)
    {
        // compare chords
        if (chord == c)
        {
            result.push_back(c);
        }
    }

    return result;
}

std::list<Chord> ChordGuesser::Guess(const std::string &notation) const
{
    std::list<Chord> result;

    // get notes from neck selection
    Neck neck = NeckEncoder::StringToNeck(notation);
    Chord chordFromNotation;
    for (const auto& string : neck.GetStrings())
    {
        if (string.isMuted()) continue;
        for (int i = 0; i < string.getFretNum(); i++)
        {
            if (string.isNoteSelected(i))
            {
                Note note = string.GetNoteByFretNumber(i);
                chordFromNotation.AddNote(note);
                break;
            }
        }
    }

    return Guess(chordFromNotation);
}

std::list<Chord> ChordGuesser::GuessUncomplete(const Chord &chord) const
{
    std::list<Chord> result;

    // find similar chords
    for (const auto &c : m_allKnownChords)
    {
        // compare chords
        if (chord <= c)
        {
            result.push_back(c);
        }
    }

    return result;
}
