#ifndef NOTATIONREADER_H
#define NOTATIONREADER_H

#include <string>
#include <list>
#include <QString>
#include "chords/chord.h"

class NotationReader
{
public:
    static std::list<std::string> GetNeckNotation(const Chord& chord);
};

#endif // NOTATIONREADER_H
