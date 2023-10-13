#ifndef CHORDLISTWIDGET_H
#define CHORDLISTWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QPixmap>
#include <list>
#include "neck/neck.h"

class ChordListWidget : public QFrame
{
    Q_OBJECT

public:
    explicit ChordListWidget(QWidget *parent = nullptr);

    void SetNeckList(const std::list<Neck>& neckLst, std::optional<NoteType> baseNoteType);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    std::list<QPixmap> chordImages;
};

#endif // CHORDLISTWIDGET_H
