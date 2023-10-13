#include "scaleguesser.h"
#include "scales/scalefactory.h"

ScaleGuesser::ScaleGuesser()
{
    // init all chords
    for (auto& noteType : AllNoteTypes())
    {
        for (auto& scaleType : AllScaleTypes())
        {
            Scale scale = ScaleFactory::GenerateScale(noteType, scaleType);
            m_allKnownScales.push_back(scale);
        }
    }
}

std::list<Scale> ScaleGuesser::GuessUncomplete(const std::list<Note> &notes) const
{
    std::set<NoteType> scalesNoteTypesSet;
    for (const auto& note : notes)
    {
        scalesNoteTypesSet.insert(note.GetNoteType());
    }

    std::list<Scale> result;
    for (const auto& s : m_allKnownScales)
    {
        std::set<NoteType> sNoteTypesSet = s.ToNoteTypeSet();

        // check if all the notes of left chord belongs to right chord
        bool isSubset = true;
        for (auto nt : scalesNoteTypesSet)
        {
            if (sNoteTypesSet.find(nt) == sNoteTypesSet.end())
            {
                isSubset = false;
                break;
            }
        }


        if (isSubset)
        {
            result.push_back(s);
        }
    }

    return result;
}
