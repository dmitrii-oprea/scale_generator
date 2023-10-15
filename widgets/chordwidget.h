#ifndef CHORDWIDGET_H
#define CHORDWIDGET_H

#include <QFrame>
#include <optional>
#include "neck/neck.h"

class ChordWidget : public QFrame
{
    Q_OBJECT

public:
    ChordWidget(const Neck& neck, std::optional<NoteType> baseNote, const std::string& chordName, QWidget* parent = nullptr);

    QSize GetIntendedSize() const;

protected:
    void paintEvent(QPaintEvent*) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    void InitSizes();
    void CopyImageToClipboard();
    void CopyImageToClipboardHalfSize();

    Neck m_neck;
    QPixmap m_neckPixmap;
    std::string m_chordName;
    QAction *m_actionCopyImage;
    QAction *m_actionCopyImageHalfSize;
    QSize m_intendedSize;
};

#endif // CHORDWIDGET_H
