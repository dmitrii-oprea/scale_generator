#include "chordtypewidget.h"

ChordTypeWidget::ChordTypeWidget(QWidget *parent)
    : QComboBox(parent)
{
    for (auto c : AllChordsTypes())
    {
        QString text = QString::fromStdString(ChordTypeToString(c));
        addItem(text, (int)c);
    }
}

ChordType ChordTypeWidget::GetChordType() const
{
    QVariant data = currentData();
    return (ChordType)data.toInt();
}
