#include "pdfgenerator.h"
#include "draw/drawer.h"
#include "scales/scalefactory.h"
#include "chords/chordfactory.h"
#include "file/notationreader.h"
#include "neck/neckencoder.h"
#include <QDebug>

#define PAGE_WIDTH (2100)
#define PAGE_HEIGHT (2970)
#define PAGE_SCALE ((double)(0.7))

#define LEFT_MARGIN (100)
#define TOP_MARGIN (100)
#define NOTES_VISUALIZER_SIZE_WIDTH (100)
#define NOTES_VISUALIZER_SIZE_HEIGHT (50)
#define VERTICAL_SPACE (100)
#define NECK_LEFT_CORRECTION (-20)

#define CHORDS_TABLE_ITEM_WIDTH (50)
#define CHORDS_TABLE_ITEM_HEIGHT (40)
#define CHORDS_TABLE_LEFT_OFFSET (200)

#define CHORDS_NOTATION_CHORD_HEIGHT (170)
#define CHORDS_NOTATION_SPACE_VER (5)
#define CHORDS_NOTATION_SPACE_HOR (5)
#define CHORDS_NOTATION_IMAGE_LEFT_OFFSET (150)

QString PDFGenerator::GenerateScaleReport(NoteType baseNote, ScaleType scaleType)
{
    // prepare data
    Scale scale = ScaleFactory::GenerateScale(baseNote, scaleType);
    Neck neck;
    neck.SelectAllNotes(scale.GetNotes());

    std::set<NoteType> scaleNoteTypes;
    for (const auto& note : scale.GetNotes())
    {
        NoteType nt = note.GetNoteType();
        scaleNoteTypes.insert(nt);
    }

    auto allScaleChords = ChordFactory::GenerateAllChordsForScale(Note(baseNote), scaleType);

    QString headerStr = QString::fromStdString(scale.GetName());

    // create fonts
    QFont fontHeader;
    fontHeader.setPixelSize(70);
    fontHeader.setBold(true);

    // create pdf printer
    QPrinter printer;
    printer.setPageSize(QPageSize(QSize(PAGE_WIDTH * PAGE_SCALE, PAGE_HEIGHT * PAGE_SCALE)));
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(headerStr + ".pdf");

    QPainter painter(&printer);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    // current printing position
    int xPos = LEFT_MARGIN;
    int yPos = TOP_MARGIN;

    // print header
    painter.setFont(fontHeader);
    painter.drawText(xPos, yPos, headerStr);
    yPos += VERTICAL_SPACE;

    // print selected notes
    PrintSelectedNotes(painter, QPoint(xPos, yPos), baseNote, scaleNoteTypes);
    yPos += VERTICAL_SPACE;

    // draw neck
    QPixmap neckImage = Drawer::GenerateNeckImage(neck, baseNote, Drawer::GetDefaultFretRange());
    painter.drawImage(QPoint(xPos + NECK_LEFT_CORRECTION, yPos), neckImage.toImage());
    yPos += neckImage.height() + VERTICAL_SPACE;

    // separate chords by their bases

    // 1. Create chord lists
    std::unordered_map<NoteType, std::list<Chord>> separatedScaleChords;
    for (const auto &nt : AllNoteTypes())
    {
        separatedScaleChords.insert({ nt, std::list<Chord>() });
    }

    // 2. separate chords
    for (const auto &chord : allScaleChords)
    {
        auto baseNote = chord.GetBaseNote();
        if (baseNote.has_value())
        {
            NoteType nt = baseNote.value().GetNoteType();
            separatedScaleChords[nt].push_back(chord);
        }
    }

    // draw chords table
    int chordsTableY = yPos;
    auto chordsTableNextLine = [&]()
    {
        yPos += CHORDS_TABLE_ITEM_HEIGHT;
        // check if we have enough space below for the next line
        if (yPos + CHORDS_TABLE_ITEM_HEIGHT > printer.height() - TOP_MARGIN)
        {
            yPos = chordsTableY;
            xPos = printer.width() / 2; // middle
        }
    };
    for (const auto &nt : AllNoteTypes())
    {
        const auto &chords = separatedScaleChords[nt];
        if (chords.empty()) continue;

        std::string str;
        for (const auto &chord : chords)
        {
            PrintChordTableItem(painter, QPoint(xPos, yPos), chord);
            chordsTableNextLine();
        }
        chordsTableNextLine(); // space between chor groups
    }

    //######################### NEW PAGE
    printer.newPage();
    xPos = LEFT_MARGIN;
    yPos = TOP_MARGIN;

    // print chords from this scale
    PrintChordNotation(painter, QPoint(xPos, yPos), allScaleChords, printer);

    // done
    painter.end();
    //LogPrinterStatus(printer);
    return printer.outputFileName();
}

void PDFGenerator::PrintSelectedNotes(QPainter &painter, const QPoint &point,
                                      NoteType baseNote, const std::set<NoteType> &noteTypes)
{
    QFont fontNoteVisualizer;
    fontNoteVisualizer.setPixelSize(35);
    fontNoteVisualizer.setBold(true);

    QFontMetrics fontMetrics(fontNoteVisualizer);
    painter.setPen(Qt::black);
    painter.setFont(fontNoteVisualizer);
    for (int i = 0; i < (int)(AllNoteTypes().size()); i++)
    {
        NoteType currentNoteType = static_cast<NoteType>(i);

        // rect color
        if (baseNote == currentNoteType)
        {
            painter.setBrush(Qt::darkGray);
        }
        else if (noteTypes.find(currentNoteType) != noteTypes.end())
        {
            painter.setBrush(Qt::lightGray);
        }
        else
        {
            painter.setBrush(Qt::white);
        }

        // draw rect
        painter.drawRect(point.x() + i * NOTES_VISUALIZER_SIZE_WIDTH + 1, point.y() + 1,
                         NOTES_VISUALIZER_SIZE_WIDTH + 1, NOTES_VISUALIZER_SIZE_HEIGHT - 1);

        // draw note string
        QString str = QString::fromStdString(NoteTypeToString(currentNoteType));
        int textX = point.x() + i * NOTES_VISUALIZER_SIZE_WIDTH + NOTES_VISUALIZER_SIZE_WIDTH / 2 -
                fontMetrics.horizontalAdvance(str) / 2;
        int textY = point.y() + NOTES_VISUALIZER_SIZE_WIDTH / 2 - fontMetrics.height() / 4;
        painter.drawText(textX, textY, str);
    }
}

void PDFGenerator::PrintChordTableItem(QPainter &painter, const QPoint &point, const Chord &chord)
{
    QFont fontMainNote;
    fontMainNote.setPixelSize(35);
    fontMainNote.setBold(true);

    QFont fontNoteVisualizer;
    fontNoteVisualizer.setPixelSize(20);
    QFontMetrics fontMetrics(fontNoteVisualizer);

    // print chord text
    QString chordStr = QString::fromStdString(chord.GetName());
    painter.setFont(fontMainNote);
    painter.setPen(Qt::black);
    painter.drawText(QPoint(point.x(),
                            point.y() + CHORDS_TABLE_ITEM_HEIGHT - 5),
                     chordStr);

    auto noteTypes = chord.ToNoteTypeSet();

    // draw notes visualizer
    painter.setFont(fontNoteVisualizer);
    for (int i = 0; i < (int)(AllNoteTypes().size()); i++)
    {
        NoteType currentNoteType = static_cast<NoteType>(i);

        // rect color
        if (chord.GetBaseNote() == currentNoteType)
        {
            painter.setBrush(Qt::darkGray);
        }
        else if (noteTypes.find(currentNoteType) != noteTypes.end())
        {
            painter.setBrush(Qt::lightGray);
        }
        else
        {
            painter.setBrush(Qt::white);
        }

        // draw rect
        int x = CHORDS_TABLE_LEFT_OFFSET + point.x() + i * CHORDS_TABLE_ITEM_WIDTH + 1;
        int y = point.y() + 1;
        painter.drawRect(x, y,
                         CHORDS_TABLE_ITEM_WIDTH + 1,
                         CHORDS_TABLE_ITEM_HEIGHT - 1);

        // draw note string
        QString str = QString::fromStdString(NoteTypeToString(currentNoteType));
        int textX = point.x() + CHORDS_TABLE_LEFT_OFFSET +
                i * CHORDS_TABLE_ITEM_WIDTH + CHORDS_TABLE_ITEM_WIDTH / 2 -
                fontMetrics.horizontalAdvance(str) / 2;
        int textY = point.y() + CHORDS_TABLE_ITEM_HEIGHT / 2 + fontMetrics.height() / 3;
        painter.drawText(textX, textY, str);
    }
}

void PDFGenerator::PrintChordNotation(QPainter &painter, const QPoint &point,
                                        const std::list<Chord> &chords, QPrinter &printer)
{
    int x = LEFT_MARGIN;
    int y = point.y();

    QFont fontNoteVisualizer;
    fontNoteVisualizer.setPixelSize(25);
    fontNoteVisualizer.setBold(true);
    painter.setPen(Qt::black);
    painter.setFont(fontNoteVisualizer);
    QFontMetrics fontMetrics(fontNoteVisualizer);

    for (const auto &chord : chords)
    {
        // get all chord notations
        auto chordNotations = NotationReader::GetNeckNotation(chord);
        std::list<Neck> neckLst;
        for (auto& notation : chordNotations)
        {
            neckLst.push_back(NeckEncoder::StringToNeck(notation));
        }

        // get all chord images
        std::list<QPixmap> chordImages;
        for (const auto& neck : neckLst)
        {
            auto range = neck.GetNeckRangeTrunked();

            Neck n(neck);
            chordImages.push_back(Drawer::GenerateNeckImage(n, chord.GetBaseNote()->GetNoteType(), range));
        }

        x = LEFT_MARGIN;

        // draw chord name
        painter.drawText(x, y + CHORDS_NOTATION_CHORD_HEIGHT / 2 + fontMetrics.height() / 2,
                         QString::fromStdString(chord.GetName()));
        x += CHORDS_NOTATION_IMAGE_LEFT_OFFSET;

        // paint them
        auto chordNotationNewLine = [&]()
        {
            y += CHORDS_NOTATION_CHORD_HEIGHT + CHORDS_NOTATION_SPACE_VER;
            x = LEFT_MARGIN + CHORDS_NOTATION_IMAGE_LEFT_OFFSET;

            // check if has space below
            if (y + CHORDS_NOTATION_CHORD_HEIGHT > printer.height() - TOP_MARGIN)
            {
                printer.newPage();
                y = TOP_MARGIN;
            }
        };
        for (auto it = chordImages.begin(); it != chordImages.end(); ++it)
        {
            const auto& img = *it;
            QImage scaledChordImage = img.toImage().scaledToHeight(CHORDS_NOTATION_CHORD_HEIGHT, Qt::TransformationMode::SmoothTransformation);
            painter.drawImage(x, y, scaledChordImage);

            x += scaledChordImage.width() + CHORDS_NOTATION_SPACE_HOR;

            // check if next image has space to the right
            auto nextIt = it;
            nextIt++;
            if (nextIt != chordImages.end())
            {
                const auto& nextImage = *nextIt;
                QImage nextScaledChordImage = nextImage.toImage().scaledToHeight(CHORDS_NOTATION_CHORD_HEIGHT);
                if (x + nextScaledChordImage.width() > printer.width() - LEFT_MARGIN)
                {
                    chordNotationNewLine();
                }
            }
        }
        chordNotationNewLine();
    }
}

void PDFGenerator::LogPrinterStatus(QPrinter &printer)
{
    auto state = printer.printerState();
    switch (state)
    {
    case QPrinter::PrinterState::Aborted:
        qDebug() << "PrinterState = Aborted";
        break;
    case QPrinter::PrinterState::Active:
        qDebug() << "PrinterState = Active";
        break;
    case QPrinter::PrinterState::Error:
        qDebug() << "PrinterState = Error";
        break;
    case QPrinter::PrinterState::Idle:
        qDebug() << "PrinterState = Idle";
        break;
    }
}
