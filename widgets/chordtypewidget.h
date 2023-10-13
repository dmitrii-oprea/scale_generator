#ifndef CHORDTYPEWIDGET_H
#define CHORDTYPEWIDGET_H

#include <QComboBox>
#include "chords/chord.h"

class ChordTypeWidget : public QComboBox
{
    Q_OBJECT

public:
    ChordTypeWidget(QWidget *parent = nullptr);
    ChordType GetChordType() const;
};

#endif // CHORDTYPEWIDGET_H
