#ifndef SCALETYPEWIDGET_H
#define SCALETYPEWIDGET_H

#include <QComboBox>
#include "scales/scale.h"

class ScaleTypeWidget : public QComboBox
{
    Q_OBJECT

public:
    ScaleTypeWidget(QWidget *parent = nullptr);
    ScaleType GetScaleType() const;
};

#endif // SCALETYPEWIDGET_H
