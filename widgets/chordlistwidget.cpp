#include "chordlistwidget.h"
#include "draw/drawer.h"
#include <QPainter>
#include <QDebug>

ChordListWidget::ChordListWidget(QWidget *parent)
    : QFrame(parent)
{
    this->setContentsMargins(0, 0, 0, 0);
    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(m_mainLayout);

    // handle resize event
    m_resizeTimer.setSingleShot(true);
    connect(&m_resizeTimer, &QTimer::timeout, this, &ChordListWidget::ResizeDone);
}

void ChordListWidget::SetNeckList(const std::list<Neck> &neckLst, std::optional<NoteType> baseNoteType)
{
    SetNeckList(neckLst, baseNoteType, "");
}

void ChordListWidget::SetNeckList(const std::list<Neck> &neckLst, std::optional<NoteType> baseNoteType, const std::string &chordName)
{
    ClearWidgetList();
    for (auto &neck : neckLst)
    {
        auto widget = new ChordWidget(neck, baseNoteType, chordName);
        m_widgets.push_back(widget);
    }
    UpdateWidgetsInLayout();
}

void ChordListWidget::resizeEvent(QResizeEvent *event)
{
    m_resizeTimer.start(200);
    QWidget::resizeEvent(event);
}

void ChordListWidget::ClearWidgetList()
{
    for (auto widget : m_widgets)
    {
        delete widget;
    }
    m_widgets.clear();
}

void ChordListWidget::UpdateWidgetsInLayout()
{
    // clean, but widgets are not deleted
    while (auto verticalLayoutItem = m_mainLayout->takeAt(0))
    {
        if (auto horizontalLayout = verticalLayoutItem->layout())
        {
            while (auto horizontalLayoutItem = horizontalLayout->takeAt(0))
            {
                delete horizontalLayoutItem;
            }
        }
        delete verticalLayoutItem;
    }

    // create another layout struture
    auto widgetIt = m_widgets.begin();
    int nextVerticalLayoutIndex = 0;
    while (widgetIt != m_widgets.end())
    {
        // create horizontal layout
        auto horizontalLayout = new QHBoxLayout;
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        m_mainLayout->insertLayout(nextVerticalLayoutIndex, horizontalLayout);

        // insert widgets until it can
        int nextHorizontalLayoutIndex = 0;
        int currentHorizontalCursonPos = 0;
        do
        {
            horizontalLayout->insertWidget(nextHorizontalLayoutIndex, *widgetIt, 1);
            currentHorizontalCursonPos += (*widgetIt)->GetIntendedSize().width();
            nextHorizontalLayoutIndex++;
            widgetIt++;

            // no more widgets
            if (widgetIt == m_widgets.end())
            {
                break;
            }

            // next widget can be placed in the same row?
            if (currentHorizontalCursonPos  + (*widgetIt)->GetIntendedSize().width() + 50 > this->width())
            {
                break;
            }
        } while (true);

        // insert stretch in the end to have all chords alighned to the left
        horizontalLayout->addStretch();
        nextVerticalLayoutIndex++;
    }

    // insert stretch in the end to have all chords alighned to the top
    m_mainLayout->addStretch();
}

void ChordListWidget::ResizeDone()
{
    UpdateWidgetsInLayout();
}
