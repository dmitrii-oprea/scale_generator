#ifndef SCALEFACTORY_H
#define SCALEFACTORY_H

#include "notes/note.h"
#include "scale.h"

class ScaleFactory
{
public:
    static Scale GenerateScale(const Note &baseNote, ScaleType scaleType);
};

#endif // SCALEFACTORY_H
