#include "drawer.h"
#include <QBrush>
#include <QColor>
#include <QString>
#include <QFontMetrics>
#include <iostream>

QPixmap Drawer::GenerateNeckImage(Neck &neck, std::optional<NoteType> baseNoteType,
                                  const std::pair<int, int> &range)
{
    // setup font
    QFont fontFretNum;
    fontFretNum.setPixelSize(25);
    //fontFretNum.setBold(true);

    QFont fontNote;
    fontNote.setPixelSize(23);
    fontNote.setBold(true);

    QFont fontSigns;
    fontSigns.setPixelSize(30);
    fontSigns.setBold(true);

    // get neck image
    QImage neckImg(":/img/img/neck_bw4.png");

    // how many frets to draw
    int fretNumToDraw = range.second - range.first + 1;
    if (range.first == 0) fretNumToDraw--;

    // neck image - truncked
    QRect neckImageRect = neckImg.rect();
    if (range.first == 0)
    {
        neckImageRect.setLeft(0);
    }
    else
    {
        neckImageRect.setLeft((range.first - 1) * getNeckFretWidth());
    }
    neckImageRect.setWidth(fretNumToDraw * getNeckFretWidth() + getNeckLeftStartPoint() * 2);

    // create main canvas
    QSize pmSize(getNeckStaringPoint().x() + neckImageRect.width(),
                 getNeckStaringPoint().y() + neckImageRect.height());
    QPixmap pm(pmSize);
    QPainter painter(&pm);

    // smooth
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    // background
    painter.setBrush(Qt::white);
    painter.drawRect(pm.rect().x() - 1, pm.rect().y() - 1,
                     pm.rect().width() + 2, pm.rect().height() + 2);

    // draw neck image
    painter.drawImage(getNeckStaringPoint(), neckImg, neckImageRect);

    // draw fret numbers
    painter.setFont(fontFretNum);
    QFontMetrics fontMetricsFretNumber(fontFretNum);
    for (int i = range.first; i <= range.first + fretNumToDraw; i++)
    {
        // don't draw 0 fret
        if (i == 0) continue;

        QString text = QString::number(i);
        int x = getNeckStaringPoint().x() + getNeckLeftStartPoint() -
                getNeckFretWidth() / 2 + getNeckFretWidth() * i -
                fontMetricsFretNumber.horizontalAdvance(text) / 2;
        int y = getNeckStaringPoint().y() - fontMetricsFretNumber.height() / 3 + 5;

        // truncked correction
        x -= getNeckFretWidth() * range.first;
        if (range.first > 0)
        {
            x += getNeckFretWidth();
        }

        painter.drawText(x, y, text);
    }

    // draw selected notes
    const int noteCircleRadius = 17;
    painter.setBrush(Qt::black);
    auto &strings = neck.GetStrings();
    for (int currentStringNumber = 0; currentStringNumber < (int)strings.size(); currentStringNumber++)
    {
        const auto str = strings[currentStringNumber];
        int y = GetStringVerticalOffset(currentStringNumber) + getNeckStaringPoint().y();

        // is string muted?
        if (str.isMuted())
        {
            // calculate horizontal offset
            int textX = getNeckStaringPoint().x() - 33;
            int textY = y + 10;
            painter.setPen(Qt::black);
            QPoint textPos(textX, textY);
            QString str = QString::fromUtf8("\u2715"); // X
            painter.setFont(fontSigns);
            painter.drawText(textPos, str);
        }

        // string frets
        painter.setPen(Qt::white);
        for (int currentFretNumber = 0; currentFretNumber < str.getFretNum(); currentFretNumber++)
        {
            // selected note is in range, expect 0 fret that should always be shown
            if (currentFretNumber != 0)
            {
                if (currentFretNumber < range.first) continue;
                if (currentFretNumber > range.second) continue;
            }

            // ignore unselected frets
            if (!str.isNoteSelected(currentFretNumber)) continue;
            Note note = str.GetNoteByFretNumber(currentFretNumber);

            // calculate horizontal offset
            int x = getNeckStaringPoint().x() - 20;
            if (currentFretNumber > 0)
            {
                x = getNeckStaringPoint().x() + getNeckLeftStartPoint() +
                        getNeckFretWidth() / 2 + getNeckFretWidth() * (currentFretNumber - 1);

                // truncked correction
                x -= range.first * getNeckFretWidth();
                if (range.first > 0)
                {
                    x += getNeckFretWidth();
                }
            }

            // base note?
            if (baseNoteType.has_value() &&
                note.GetNoteType() == baseNoteType.value())
            {
                painter.setBrush(Qt::black);

            }
            else
            {
                painter.setBrush(QColor::fromRgb(60, 60, 60));
            }

            // draw circle
            //painter.setPen(Qt::black);
            painter.drawEllipse(x - noteCircleRadius, y - noteCircleRadius,
                                noteCircleRadius * 2, noteCircleRadius * 2);

            // get font metrics
            QFontMetrics fontMetricsNote(fontNote);

            // draw note
            //painter.setPen(Qt::white);
            painter.setFont(fontNote);
            QString noteStr = QString::fromStdString(NoteTypeToString(note.GetNoteType()));
            int textWidth = fontMetricsNote.horizontalAdvance(noteStr);
            int textHeight = fontMetricsNote.height();
            int textX = x - textWidth / 2 - 1;
            int textY = y + textHeight / 2 - 6;
            if (noteStr.length() > 1)
            {
                textX += 2;
            }
            QPoint textPos(textX, textY);
            painter.drawText(textPos, noteStr);
        }
    }

    painter.end();
    return pm;
}

const QPoint& Drawer::getNeckStaringPoint()
{
    static QPoint p(50, 30);
    return p;
}

double Drawer::getNeckFretWidth()
{
    return 101.5;
}

int Drawer::getNeckLeftStartPoint()
{
    return 9;
}

double Drawer::GetHeightBetweenStrings()
{
    return 35.5;
}

int Drawer::GetStringVerticalOffset(int stringNum)
{
    return 17 + (double)stringNum * GetHeightBetweenStrings();
}

QSize Drawer::GetNeckImageSize()
{
    return QSize(1537, 214);
}

std::pair<int, int> Drawer::GetDefaultFretRange()
{
    return {0, 15};
}

