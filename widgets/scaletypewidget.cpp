#include "scaletypewidget.h"

ScaleTypeWidget::ScaleTypeWidget(QWidget *parent)
    : QComboBox(parent)
{
    for (auto s : AllScaleTypes())
    {
        QString text = QString::fromStdString(ScaleTypeToString(s));
        addItem(text, (int)s);
    }
}

ScaleType ScaleTypeWidget::GetScaleType() const
{
    QVariant data = currentData();
    return (ScaleType)data.toInt();
}
