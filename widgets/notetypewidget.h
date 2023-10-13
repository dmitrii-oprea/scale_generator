#ifndef NOTETYPEWIDGET_H
#define NOTETYPEWIDGET_H

#include <QComboBox>
#include "notes/note.h"

class NoteTypeWidget : public QComboBox
{
    Q_OBJECT

public:
    NoteTypeWidget(QWidget *parent = nullptr);
    NoteType GetNoteType() const;
};

#endif // NOTETYPEWIDGET_H
