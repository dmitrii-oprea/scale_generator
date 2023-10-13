#include "chordlistwidget.h"
#include "draw/drawer.h"
#include <QPainter>

#define START_POINT (5)
#define SCALE ((double)(0.5))
#define MARGIN (20)

ChordListWidget::ChordListWidget(QWidget *parent)
    : QFrame(parent)
{
}

void ChordListWidget::SetNeckList(const std::list<Neck> &neckLst, std::optional<NoteType> baseNoteType)
{
    chordImages.clear();
    for (const auto& neck : neckLst)
    {
        auto range = neck.GetNeckRangeTrunked();

        Neck n(neck);
        chordImages.push_back(Drawer::GenerateNeckImage(n, baseNoteType, range));
    }
}

void ChordListWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    // background
    painter.setBrush(Qt::lightGray);
    painter.drawRect(-1, -1, this->width() + 2, this->height() + 2);

    int x = START_POINT;
    int y = START_POINT;

    painter.scale(SCALE, SCALE);

    for (auto it = chordImages.begin(); it != chordImages.end(); ++it)
    {
        const auto& image = *it;
        painter.drawPixmap(x, y, image);

        x += image.width() + MARGIN;

        // check if next image can be placed next to the right
        auto nextIt = it;
        nextIt++;
        if (nextIt != chordImages.end())
        {
            const auto& nextImage = *nextIt;
            int nextX = x + MARGIN + nextImage.width();
            if ((double)nextX * SCALE > this->width())
            {
                x = START_POINT;
                y += image.height() + MARGIN;
            }
        }
    }

    painter.end();
}
