QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# icon
RC_ICONS = icons/main_icon2.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chords/chord.cpp \
    chords/chordfactory.cpp \
    chords/chordguesser.cpp \
    draw/drawer.cpp \
    file/notationreader.cpp \
    main.cpp \
    mainwindow.cpp \
    neck/neck.cpp \
    neck/neckencoder.cpp \
    notes/note.cpp \
    pdf/pdfgenerator.cpp \
    scales/scale.cpp \
    scales/scalefactory.cpp \
    scales/scaleguesser.cpp \
    widgets/chordlistwidget.cpp \
    widgets/chordtypewidget.cpp \
    widgets/chordwidget.cpp \
    widgets/neckwidget.cpp \
    widgets/notesvisualizerwidget.cpp \
    widgets/notetypewidget.cpp \
    widgets/scaletypewidget.cpp

HEADERS += \
    chords/chord.h \
    chords/chordfactory.h \
    chords/chordguesser.h \
    draw/drawer.h \
    file/notationreader.h \
    mainwindow.h \
    neck/neck.h \
    neck/neckencoder.h \
    notes/note.h \
    pdf/pdfgenerator.h \
    scales/scale.h \
    scales/scalefactory.h \
    scales/scaleguesser.h \
    widgets/chordlistwidget.h \
    widgets/chordtypewidget.h \
    widgets/chordwidget.h \
    widgets/neckwidget.h \
    widgets/notesvisualizerwidget.h \
    widgets/notetypewidget.h \
    widgets/scaletypewidget.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/chordbase.qrc \
    resources/images.qrc

DISTFILES += \
    resources/chordbase/A/A.txt \
    resources/chordbase/A/Am.txt

# static MinGW build
CONFIG += static
QMAKE_LFLAGS += -static -static-libgcc -static-libstdc++ -lstdc++
DEFINES += STATIC
