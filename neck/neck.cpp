#include "neck.h"
#include "draw/drawer.h"

#define FRET_NUM (24)

NeckString::NeckString(NoteType startingNote)
{
    for (int i = 0; i < FRET_NUM; i++)
    {
        Note n(startingNote);
        n = n + i;
        m_notes.push_back(NoteAndSelection(n));
    }
}

Note NeckString::GetNoteByFretNumber(int fretNum) const
{
    return m_notes[fretNum].m_note;
}

void NeckString::SelectNote(int fretNum, bool select)
{
    m_notes[fretNum].m_isSelected = select;
}

bool NeckString::isNoteSelected(int fretNum) const
{
    return m_notes[fretNum].m_isSelected;
}

void NeckString::cleanAllSelections()
{
    for (auto& obj : m_notes)
    {
        obj.m_isSelected = false;
    }
}

int NeckString::getFretNum()
{
    return FRET_NUM;
}

void NeckString::setMuted(bool muted)
{
    m_isMuted = muted;
}

bool NeckString::isMuted() const
{
    return m_isMuted;
}

//#######################################

Neck::Neck()
{
    // from 1-st to 6-th string
    m_strings.push_back(NeckString(NoteType::E));
    m_strings.push_back(NeckString(NoteType::B));
    m_strings.push_back(NeckString(NoteType::G));
    m_strings.push_back(NeckString(NoteType::D));
    m_strings.push_back(NeckString(NoteType::A));
    m_strings.push_back(NeckString(NoteType::E));
}

Neck::Neck(const Neck &obj)
    : m_strings(obj.m_strings)
{
}

Neck Neck::operator =(const Neck &right)
{
    this->m_strings = right.m_strings;
    return *this;
}

std::vector<NeckString> &Neck::GetStrings()
{
    return m_strings;
}

void Neck::SelectAllNotes(const std::list<Note> &notes)
{
    // create set of notes
    std::set<NoteType> noteSet;
    for (const auto &note : notes)
    {
        noteSet.insert(note.GetNoteType());
    }

    // selects all notes from the list (set)
    for (auto& str : m_strings)
    {
        for (int i = 0; i < NeckString::getFretNum(); i++)
        {
            Note n = str.GetNoteByFretNumber(i);
            if (noteSet.find(n.GetNoteType()) != noteSet.end())
            {
                str.SelectNote(i, true);
            }
        }
    }
}

int Neck::getFretNum()
{
    return FRET_NUM;
}

Chord Neck::toChord() const
{
    // Get all chord notes
    Chord chord;
    for (const auto &string : m_strings)
    {
        for (int i = 0; i < string.getFretNum(); i++)
        {
            if (string.isNoteSelected(i))
            {
                Note n = string.GetNoteByFretNumber(i);
                chord.AddNote(n);
            }
        }
    }

    return chord;
}

std::pair<int, int> Neck::GetNeckRangeTrunked() const
{
    // calculate range
    std::pair<int, int> rangeTruncked = Drawer::GetDefaultFretRange();
    std::swap(rangeTruncked.first, rangeTruncked.second);
    auto strings = Neck(*this).GetStrings();
    for (auto& string : strings)
    {
        for (int i = 1; i < string.getFretNum(); i++)
        {
            if (string.isNoteSelected(i))
            {
                rangeTruncked.first = std::min(rangeTruncked.first, i);
                rangeTruncked.second = std::max(rangeTruncked.second, i);
            }
        }
    }

    // corrections
    if (rangeTruncked.first <= 2) rangeTruncked.first = 0;

    return rangeTruncked;
}
