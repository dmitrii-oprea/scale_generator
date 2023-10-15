#ifndef CHORDLISTWIDGET_H
#define CHORDLISTWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QPixmap>
#include <list>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include "neck/neck.h"
#include "chordwidget.h"

class ChordListWidget : public QFrame
{
    Q_OBJECT

public:
    explicit ChordListWidget(QWidget *parent = nullptr);

    void SetNeckList(const std::list<Neck>& neckLst, std::optional<NoteType> baseNoteType);
    void SetNeckList(const std::list<Neck>& neckLst, std::optional<NoteType> baseNoteType, const std::string& chordName);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    void ClearWidgetList();
    void UpdateWidgetsInLayout();
    void ResizeDone();

    QVBoxLayout *m_mainLayout = nullptr;
    std::list<ChordWidget*> m_widgets;
    QTimer m_resizeTimer;
};

#endif // CHORDLISTWIDGET_H
