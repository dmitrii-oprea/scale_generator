#include "notetypewidget.h"

NoteTypeWidget::NoteTypeWidget(QWidget *parent)
    : QComboBox(parent)
{
    for (auto n : AllNoteTypes())
    {
        QString text = QString::fromStdString(NoteTypeToString(n));
        addItem(text, (int)n);
    }
}

NoteType NoteTypeWidget::GetNoteType() const
{
    QVariant data = currentData();
    return (NoteType)data.toInt();
}
