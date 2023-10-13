#ifndef CHORDFACTORY_H
#define CHORDFACTORY_H

#include "notes/note.h"
#include "scales/scale.h"
#include "chord.h"
#include <list>

class ChordFactory
{
public:
    static std::list<int> GenerateChordOffsets(ChordType chordType);
    static Chord GenerateChord(const Note &baseNote, ChordType chordType);
    static std::list<Chord> GenerateAllChordsForScale(const Note &scaleBaseNote, ScaleType scaleType);
};

#endif // CHORDFACTORY_H
