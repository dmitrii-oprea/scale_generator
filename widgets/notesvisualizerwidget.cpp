#include "notesvisualizerwidget.h"
#include <QPainter>

#define SQUARE_SIZE_WIDTH (100)
#define SQUARE_SIZE_HEIGHT (50)

NotesVisualizerWidget::NotesVisualizerWidget(QWidget *parent)
    : QFrame(parent)
    , NoteTypeCount(AllNoteTypes().size())
{
    fontNote.setPixelSize(25);
    fontNote.setBold(true);
}

void NotesVisualizerWidget::SetNotes(const std::list<Note> &notes)
{
    m_noteTypes.clear();
    for (const auto& note : notes)
    {
        NoteType nt = note.GetNoteType();
        m_noteTypes.insert(nt);
    }
}

void NotesVisualizerWidget::SetBaseNote(std::optional<NoteType> baseNote)
{
    m_baseNote = baseNote;
}

void NotesVisualizerWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    double scale = GetScale();
    if (scale <= 0) return;
    painter.scale(scale, scale);

    QFontMetrics fontMetrics(fontNote);

    painter.setPen(Qt::black);
    painter.setFont(fontNote);

    for (int i = 0; i < NoteTypeCount; i++)
    {
        NoteType currentNoteType = static_cast<NoteType>(i);

        // rect color
        if (m_baseNote.has_value() && m_baseNote.value() == currentNoteType)
        {
            painter.setBrush(Qt::darkGray);
        }
        else if (m_noteTypes.find(currentNoteType) != m_noteTypes.end())
        {
            painter.setBrush(Qt::lightGray);
        }
        else
        {
            painter.setBrush(Qt::white);
        }

        // draw rect
        painter.drawRect(i * SQUARE_SIZE_WIDTH + 1, 1,
                         SQUARE_SIZE_WIDTH + 1, SQUARE_SIZE_HEIGHT - 1);

        // draw note string
        QString str = QString::fromStdString(NoteTypeToString(currentNoteType));
        int textX = i * SQUARE_SIZE_WIDTH + SQUARE_SIZE_WIDTH / 2 - fontMetrics.horizontalAdvance(str) / 2;
        int textY = SQUARE_SIZE_HEIGHT / 2 + fontMetrics.height() / 3;
        painter.drawText(textX, textY, str);

    }
    painter.end();
}

double NotesVisualizerWidget::GetScale()
{
    // calculate scale
    double scaleX = (double)this->width() / (double)SQUARE_SIZE_WIDTH * (double)NoteTypeCount;
    double scaleY = (double)this->height() / (double)SQUARE_SIZE_HEIGHT;
    return std::min(scaleX, scaleY);
}
