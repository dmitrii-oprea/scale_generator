#ifndef CHORDGUESSER_H
#define CHORDGUESSER_H

#include "notes/note.h"
#include "chords/chord.h"
#include "neck/neckencoder.h"
#include "chordfactory.h"

class ChordGuesser
{
    std::list<Chord> m_allKnownChords;

public:
    ChordGuesser();

    std::list<Chord> Guess(const Chord& chord) const;
    std::list<Chord> Guess(const std::string& notation) const;

    std::list<Chord> GuessUncomplete(const Chord& chord) const;
};

#endif // CHORDGUESSER_H
