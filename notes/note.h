#ifndef NOTE_H
#define NOTE_H

#include <vector>
#include <string>

enum class NoteType : int
{
    A, A_, B, C, C_, D, D_, E, F, F_, G, G_
};

std::vector<NoteType> &AllNoteTypes();
std::string NoteTypeToString(NoteType n);

class Note
{
    NoteType m_note;

public:
    Note(NoteType note);
    Note(const Note &obj);

    NoteType GetNoteType() const;

    Note operator =(const Note& right);
    bool operator ==(const Note& right) const;
    Note operator +(int offset) const;
    bool operator <(const Note& right) const;
};


#endif // NOTE_H
