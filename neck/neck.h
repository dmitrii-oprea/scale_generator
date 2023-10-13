#ifndef NECK_H
#define NECK_H

#include "notes/note.h"
#include "chords/chord.h"
#include <vector>
#include <list>
#include <set>
#include <string>

class NeckString
{
    struct NoteAndSelection
    {
        Note m_note;
        bool m_isSelected = false;
        NoteAndSelection(Note note) : m_note(note) {}
    };
    std::vector<NoteAndSelection> m_notes;
    bool m_isMuted = false;

public:
    NeckString(NoteType startingNote);
    Note GetNoteByFretNumber(int fretNum) const;
    void SelectNote(int fretNum, bool select);
    bool isNoteSelected(int fretNum) const;
    void cleanAllSelections();
    static int getFretNum();
    void setMuted(bool muted);
    bool isMuted() const;

};

class Neck
{
    std::vector<NeckString> m_strings;

public:
    Neck();
    Neck(const Neck& obj);
    Neck operator =(const Neck& right);

    std::vector<NeckString>& GetStrings();
    void SelectAllNotes(const std::list<Note>& notes);
    static int getFretNum();
    Chord toChord() const;
    std::pair<int, int> GetNeckRangeTrunked() const;
};

#endif // NECK_H
