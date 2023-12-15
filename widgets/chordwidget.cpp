#include "chordwidget.h"
#include "draw/drawer.h"
#include "chordwidgetdef.h"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QMenu>
#include <QContextMenuEvent>
#include <QPainter>
#include <QFont>



ChordWidget::ChordWidget(const Neck &neck, std::optional<NoteType> baseNote, const std::string &chordName, QWidget *parent)
    : QFrame(parent)
{
    m_neck = neck;
    m_chordName = chordName;
    auto drawRange = neck.GetNeckRangeTrunked();
    m_neckPixmap = Drawer::GenerateNeckImage(m_neck, baseNote, drawRange);

    this->setContentsMargins(0, 0, 0, 0);
    InitSizes();

    // create context menu
    m_actionCopyImage = new QAction("Copy image");
    connect(m_actionCopyImage, &QAction::triggered, this, &ChordWidget::CopyImageToClipboard);
    m_actionCopyImageSmallSizeAndChordName = new QAction("Copy small image + chord name");
    connect(m_actionCopyImageSmallSizeAndChordName, &QAction::triggered, this, &ChordWidget::CopyImageToClipboardSmallSizeAndChordName);
}

QSize ChordWidget::GetIntendedSize() const
{
    return m_intendedSize;
}

void ChordWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    painter.drawPixmap(this->rect(), m_neckPixmap, m_neckPixmap.rect());
    painter.end();
}

void ChordWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(m_actionCopyImage);
    menu.addAction(m_actionCopyImageSmallSizeAndChordName);
    menu.exec(event->globalPos());
}

void ChordWidget::InitSizes()
{
    m_intendedSize.setWidth(1);
    m_intendedSize.setHeight(MAX_WIDGET_HEIGHT);
    if (m_neckPixmap.height() > 0)
    {
        auto w = (double)m_neckPixmap.width() * MAX_WIDGET_HEIGHT / (double)m_neckPixmap.height();
        m_intendedSize.setWidth(w);
    }

    this->setFixedHeight(m_intendedSize.height());
    this->setMaximumWidth(m_intendedSize.width());

}

void ChordWidget::CopyImageToClipboard()
{
    QImage image = m_neckPixmap.toImage();
    if (image.isNull()) return;

    QApplication::clipboard()->setImage(image, QClipboard::Clipboard);
}

void ChordWidget::CopyImageToClipboardSmallSizeAndChordName()
{
    QImage image = m_neckPixmap.toImage();
    if (image.isNull()) return;

    // font
    QFont chordNameFont;
    chordNameFont.setPixelSize(40);
    chordNameFont.setBold(true);
    QFontMetrics fm(chordNameFont);

    // new image -> clear
    QImage extendedImage(image.width(), image.height() + CHORD_EXTRA_HEIGHT, image.format());
    QPainter painter(&extendedImage);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::white);
    painter.setFont(chordNameFont);
    painter.drawRect(extendedImage.rect());

    // draw chord
    painter.drawImage(0, CHORD_EXTRA_HEIGHT, image);

    // add chord name
    painter.setPen(Qt::black);
    QString chordName = QString::fromStdString(m_chordName);
    int xPos = extendedImage.width() / 2 - fm.horizontalAdvance(chordName) / 2;
    int yPos = fm.height() * 0.7;
    painter.drawText(xPos, yPos, chordName);
    painter.end();

    // smaller size
    extendedImage = extendedImage.scaled(extendedImage.width() * SMALLER_IMAGE_COEF, extendedImage.height() * SMALLER_IMAGE_COEF,
                         Qt::AspectRatioMode::KeepAspectRatio,
                         Qt::TransformationMode::SmoothTransformation);

    QApplication::clipboard()->setImage(extendedImage, QClipboard::Clipboard);
}
