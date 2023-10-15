#include "neckwidget.h"
#include "draw/drawer.h"
#include <QPainter>
#include <QApplication>
#include <QClipboard>
#include <list>

NeckWidget::NeckWidget(QWidget *parent)
    : QFrame(parent)
{
    setMinimumHeight(100);

    // create context menu
    actionCopyImage = new QAction("Copy image");
    connect(actionCopyImage, &QAction::triggered, this, &NeckWidget::CopyImageToClipboard);

    m_drawRange = Drawer::GetDefaultFretRange();

    // generate empty neck
    m_neckPixmap = Drawer::GenerateNeckImage(m_neck, m_baseNote, m_drawRange);
}

void NeckWidget::SetNeck(Neck &neck, std::optional<NoteType> baseNote)
{
    m_neck = neck;
    m_baseNote = baseNote;

    m_neckPixmap = Drawer::GenerateNeckImage(m_neck, m_baseNote, m_drawRange);
}

Neck &NeckWidget::GetNeck()
{
    return m_neck;
}

void NeckWidget::recalculateMinimumHeight()
{
    if (m_neckPixmap.isNull()) return;
    if (m_neckPixmap.width() <= 0) return;
    if (m_neckPixmap.height() <= 0) return;

    double k = (double)m_neckPixmap.height() / (double)m_neckPixmap.width();
    double h = (double)this->width() * k;
    this->setMinimumHeight(h);
}

void NeckWidget::resizeEvent(QResizeEvent *)
{
    recalculateMinimumHeight();
}

void NeckWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    double scale = GetScale(m_neckPixmap.size());
    if (scale <= 0) return;
    painter.scale(scale, scale);

    painter.drawPixmap(0, 0, m_neckPixmap);
    painter.end();
}

void NeckWidget::mousePressEvent(QMouseEvent *event)
{
    // only left mouse for setting notes
    if (event && event->buttons() & Qt::LeftButton)
    {
        const double scale = GetScale(m_neckPixmap.size());
        if (scale <= 0) return;

        // calculate neck position
        int neckX = (double)(event->pos().x()) / scale;
        int neckY = (double)(event->pos().y()) / scale;

        // calculate string num (starting from 0)
        int stringNum = neckY - Drawer::getNeckStaringPoint().y() - Drawer::GetStringVerticalOffset(0) + Drawer::GetHeightBetweenStrings() / 2;
        if (stringNum < 0) return;
        stringNum = stringNum / Drawer::GetHeightBetweenStrings();
        if (stringNum < 0) return;
        if (stringNum >= (int)m_neck.GetStrings().size()) return;

        // calculate fret num
        int fretNum = neckX - Drawer::getNeckStaringPoint().x() - Drawer::getNeckLeftStartPoint() + Drawer::getNeckFretWidth();
        if (fretNum < 0) fretNum = 0;
        fretNum = fretNum / Drawer::getNeckFretWidth();

        emit OnFretClicked(stringNum, fretNum);
    }
}

void NeckWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(actionCopyImage);
    menu.exec(event->globalPos());
}

double NeckWidget::GetScale(const QSize& resultImageSize) const
{
    if (resultImageSize.width() <= 0) return 0;
    if (resultImageSize.height() <= 0) return 0;

    // calculate scale
    double scaleX = (double)this->width() / (double)resultImageSize.width();
    double scaleY = (double)this->height() / (double)resultImageSize.height();
    return std::min(scaleX, scaleY);
}

void NeckWidget::CopyImageToClipboard()
{
    QImage image = m_neckPixmap.toImage();
    if (image.isNull()) return;

    QApplication::clipboard()->setImage(image, QClipboard::Clipboard);
}

NeckWidgetChordSelector::NeckWidgetChordSelector(QWidget *parent)
    : NeckWidget(parent)
{
    connect(this, &NeckWidgetChordSelector::OnFretClicked, this, &NeckWidgetChordSelector::HandleFretClicked);
}

void NeckWidgetChordSelector::HandleFretClicked(int stringNum, int fretNum)
{
    auto& strings = m_neck.GetStrings();

    if (stringNum < 0) return;
    if (stringNum >= (int)strings.size()) return;
    if (fretNum < 0) return;
    if (fretNum >= strings[stringNum].getFretNum()) return;
    if (fretNum > Drawer::GetDefaultFretRange().second) return;

    if (fretNum == 0)
    {
        // not selected yet - select
        if (strings[stringNum].isNoteSelected(fretNum) == false &&
            strings[stringNum].isMuted() == false)
        {
            // clean and select
            strings[stringNum].cleanAllSelections();
            strings[stringNum].SelectNote(fretNum, true);
        }
        else
        {
            // selected -> mute
            if (strings[stringNum].isNoteSelected(fretNum))
            {
                strings[stringNum].cleanAllSelections();
                strings[stringNum].setMuted(true);
            }
            // clean all
            else
            {
                strings[stringNum].cleanAllSelections();
                strings[stringNum].setMuted(false);
            }
        }
    }
    else
    {
        // already selected?
        if (strings[stringNum].isNoteSelected(fretNum))
        {
            // unselect
            strings[stringNum].cleanAllSelections();
            strings[stringNum].setMuted(false);
        }
        else
        {
            // clean and select
            strings[stringNum].cleanAllSelections();
            strings[stringNum].SelectNote(fretNum, true);
            strings[stringNum].setMuted(false);
        }
    }

    // generate new neck image
    m_neckPixmap = Drawer::GenerateNeckImage(m_neck, m_baseNote, m_drawRange);

    this->update();
    emit OnFretManuallyUpdated();
}

NeckWidgetTrunc::NeckWidgetTrunc(QWidget *parent)
    : NeckWidget(parent)
{
}

void NeckWidgetTrunc::SetNeck(Neck &neck, std::optional<NoteType> baseNote)
{
    NeckWidget::SetNeck(neck, baseNote);

    // range to draw this chord
    m_drawRange = neck.GetNeckRangeTrunked();

    // tooltip - chord detector
    UpdateTooltip();
}

void NeckWidgetTrunc::UpdateTooltip()
{
    // Get all chord notes
    Chord chord = m_neck.toChord();

    // guess the chord
    auto guessedChords = m_chordGuesser.Guess(chord);
    QString guessedChordsStr;
    for (const auto& chord : guessedChords)
    {
        guessedChordsStr += QString::fromStdString(chord.GetName()) + " ";
    }

    this->setToolTip(guessedChordsStr);
}

NeckWidgetNotesSelector::NeckWidgetNotesSelector(QWidget *parent)
    : NeckWidget(parent)
{
    connect(this, &NeckWidgetNotesSelector::OnFretClicked, this, &NeckWidgetNotesSelector::HandleFretClicked);
}

void NeckWidgetNotesSelector::HandleFretClicked(int stringNum, int fretNum)
{
    auto& strings = m_neck.GetStrings();

    if (stringNum < 0) return;
    if (stringNum >= (int)strings.size()) return;
    if (fretNum < 0) return;
    if (fretNum >= strings[stringNum].getFretNum()) return;
    if (fretNum > Drawer::GetDefaultFretRange().second) return;

    bool selected = strings[stringNum].isNoteSelected(fretNum);
    strings[stringNum].SelectNote(fretNum, !selected);

    // generate new neck image
    m_neckPixmap = Drawer::GenerateNeckImage(m_neck, m_baseNote, m_drawRange);

    this->update();
    emit OnFretManuallyUpdated();
}
