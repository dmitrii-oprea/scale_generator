#ifndef NECKENCODER_H
#define NECKENCODER_H

#include <QString>
#include "neck/neck.h"

class NeckEncoder
{
public:
    static std::string NeckToString(Neck &neck);
    static Neck StringToNeck(const std::string& chordNotation);
};

#endif // NECKENCODER_H
