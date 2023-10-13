#ifndef SCALEGUESSER_H
#define SCALEGUESSER_H

#include "notes/note.h"
#include "scales/scale.h"


class ScaleGuesser
{
    std::list<Scale> m_allKnownScales;

public:
    ScaleGuesser();

    std::list<Scale> GuessUncomplete(const std::list<Note>& notes) const;
};

#endif // SCALEGUESSER_H
