#ifndef NECKWIDGET_H
#define NECKWIDGET_H

#include <QFrame>
#include <QWidget>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <optional>
#include "neck/neck.h"
#include "chords/chordguesser.h"

class NeckWidget : public QFrame
{
    Q_OBJECT

protected:
    Neck m_neck;
    std::optional<NoteType> m_baseNote;
    std::pair<int, int> m_drawRange;
    QPixmap m_neckPixmap;
    QAction *actionCopyImage;

public:
    explicit NeckWidget(QWidget* parent = nullptr);
    virtual ~NeckWidget() = default;
    virtual void SetNeck(Neck& neck, std::optional<NoteType> baseNote);
    Neck& GetNeck();

    void recalculateMinimumHeight();

signals:
    void OnFretClicked(int stringNum, int fretNum);

protected:
    void resizeEvent(QResizeEvent *) override;
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

    double GetScale(const QSize& resultImageSize) const;
    void CopyImageToClipboard();
};

class NeckWidgetChordSelector : public NeckWidget
{
    Q_OBJECT

public:
    explicit NeckWidgetChordSelector(QWidget* parent = nullptr);
    virtual ~NeckWidgetChordSelector() = default;

signals:
    void OnFretManuallyUpdated();

private:
    void HandleFretClicked(int stringNum, int fretNum);
};

class NeckWidgetNotesSelector : public NeckWidget
{
    Q_OBJECT

public:
    explicit NeckWidgetNotesSelector(QWidget* parent = nullptr);
    virtual ~NeckWidgetNotesSelector() = default;

signals:
    void OnFretManuallyUpdated();

private:
    void HandleFretClicked(int stringNum, int fretNum);
};

class NeckWidgetTrunc : public NeckWidget
{
    Q_OBJECT

    ChordGuesser m_chordGuesser;

public:
    explicit NeckWidgetTrunc(QWidget* parent = nullptr);
    virtual ~NeckWidgetTrunc() = default;

    void SetNeck(Neck &neck, std::optional<NoteType> baseNote) override;

private:
    void UpdateTooltip();
};

#endif // NECKWIDGET_H
