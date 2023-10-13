#ifndef NOTESVISUALIZERWIDGET_H
#define NOTESVISUALIZERWIDGET_H

#include <QFrame>
#include <QFont>
#include <QFontMetrics>
#include <list>
#include <set>
#include "notes/note.h"

class NotesVisualizerWidget : public QFrame
{
    Q_OBJECT

public:
    explicit NotesVisualizerWidget(QWidget *parent = nullptr);

    void SetNotes(const std::list<Note>& notes);
    void SetBaseNote(std::optional<NoteType> baseNote);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    double GetScale();

    const int NoteTypeCount;
    std::set<NoteType> m_noteTypes;
    std::optional<NoteType> m_baseNote;
    QFont fontNote;
};

#endif // NOTESVISUALIZERWIDGET_H
