#include "notationreader.h"
#include <QFile>
#include <QTextStream>

std::list<std::string> NotationReader::GetNeckNotation(const Chord &chord)
{
    std::list<std::string> result;

    // ://chordbase/A/Am.txt
    QString path = "://chordbase/";
    auto baseNoteType = chord.GetBaseNote()->GetNoteType();
    path += QString::fromStdString(NoteTypeToString(baseNoteType)) + "/";
    path += QString::fromStdString(chord.GetName()) + ".txt";

    QFile file(path);
    if (file.open(QIODevice::ReadOnly))
    {
       QTextStream in(&file);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          result.push_back(line.toStdString());
       }
       file.close();
    }

    return result;
}
