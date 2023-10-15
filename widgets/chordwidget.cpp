#include "chordwidget.h"
#include "draw/drawer.h"
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QMenu>
#include <QContextMenuEvent>

#define MAX_WIDGET_HEIGHT ((double)(100))

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
    m_actionCopyImageHalfSize = new QAction("Copy image (1/2 size)");
    connect(m_actionCopyImageHalfSize, &QAction::triggered, this, &ChordWidget::CopyImageToClipboardHalfSize);
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
    menu.addAction(m_actionCopyImageHalfSize);
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

void ChordWidget::CopyImageToClipboardHalfSize()
{
    QImage image = m_neckPixmap.toImage();
    if (image.isNull()) return;

    image = image.scaled(image.width() / 2, image.height() / 2,
                         Qt::AspectRatioMode::KeepAspectRatio,
                         Qt::TransformationMode::SmoothTransformation);

    QApplication::clipboard()->setImage(image, QClipboard::Clipboard);
}
