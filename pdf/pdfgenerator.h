#ifndef PDFGENERATOR_H
#define PDFGENERATOR_H

#include <QString>
#include <QPainter>
#include <QPrinter>
#include "scales/scale.h"
#include "notes/note.h"
#include "chords/chord.h"

class PDFGenerator
{
public:  
    static QString GenerateScaleReport(NoteType baseNote, ScaleType scaleType);
    static QString GenerateAllChords();

private:
    static void PrintSelectedNotes(QPainter &painter, const QPoint &point,
                                   NoteType baseNote, const std::set<NoteType> &noteTypes);
    static void PrintChordTableItem(QPainter &painter, const QPoint &point, const Chord &chord);
    static void PrintChordNotation(QPainter &painter, const QPoint &point,
                                     const std::list<Chord> &chords, QPrinter &printer);

    static void LogPrinterStatus(QPrinter &printer);
};

#endif // PDFGENERATOR_H
