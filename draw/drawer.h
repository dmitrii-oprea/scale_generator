#ifndef DRAWER_H
#define DRAWER_H

#include <optional>
#include <QPainter>
#include <QPixmap>
#include <QImage>
#include "notes/note.h"
#include "neck/neck.h"

class Drawer
{
public:
    static QPixmap GenerateNeckImage(Neck &neck, std::optional<NoteType> baseNoteType,
                                     const std::pair<int, int> &range);

    static const QPoint &getNeckStaringPoint();
    static double getNeckFretWidth();
    static int getNeckLeftStartPoint();
    static double GetHeightBetweenStrings();
    static int GetStringVerticalOffset(int stringNum);
    static QSize GetNeckImageSize();
    static std::pair<int, int> GetDefaultFretRange();
};


#endif // DRAWER_H
