#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "notes/note.h"
#include "chords/chord.h"
#include "chords/chordfactory.h"
#include "scales/scalefactory.h"
#include "neck/neck.h"
#include "neck/neckencoder.h"
#include "file/notationreader.h"
#include "pdf/pdfgenerator.h"

#include <unordered_map>
#include <list>
#include <QLayout>
#include <QMessageBox>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // chord database
    connect(ui->ChordDatabase_BaseNoteSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::ChordDatabase_ShowChord);
    connect(ui->ChordDatabase_ChordTypeSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::ChordDatabase_ShowChord);
    ChordDatabase_ShowChord();

    // chord guesser
    connect(ui->ChordIdentifier_ChordSelector, &NeckWidgetChordSelector::OnFretManuallyUpdated, this, &MainWindow::ChordIdentifier_UpdateCompleteChords);
    connect(ui->ChordIdentifier_ChordSelector, &NeckWidgetChordSelector::OnFretManuallyUpdated, this, &MainWindow::ChordIdentifier_UpdateUncompleteChords);
    connect(ui->ChordIdentifier_ChordSelector, &NeckWidgetChordSelector::OnFretManuallyUpdated, this, &MainWindow::ChordIdentifier_UpdateNoteVisualizer);

    // scale identifier
    connect(ui->ScaleIdentifier_NotesSelector, &NeckWidgetNotesSelector::OnFretManuallyUpdated, this, &MainWindow::ScaleIdentifier_UpdateNoteVisualizer);
    connect(ui->ScaleIdentifier_NotesSelector, &NeckWidgetNotesSelector::OnFretManuallyUpdated, this, &MainWindow::ScaleIdentifier_UpdateUncompletedScales);

    // scale generator
    connect(ui->ScaleGenerator_SelectBaseNote, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::ScaleGenerator_Update);
    connect(ui->ScaleGenerator_SelectScale, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::ScaleGenerator_Update);
    ScaleGenerator_Update();

    // dev
    connect(ui->Dev_BaseNoteSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::Dev_ShowChord);
    connect(ui->Dev_ChordTypeSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::Dev_ShowChord);
    connect(ui->Dev_ChordSelector, &NeckWidgetChordSelector::OnFretManuallyUpdated, this, &MainWindow::Dev_UpdateNeckNotation);
    Dev_ShowChord();
    Dev_UpdateStatistics();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ChordIdentifier_UpdateCompleteChords()
{
    ui->ChordIdentifier_ListCompletedChords->clear();

    Neck neck = ui->ChordIdentifier_ChordSelector->GetNeck();
    Chord neckChord = neck.toChord();

    // this chord should have at least 2 notes
    if (neckChord.GetNotes().size() < 2) return;

    // completed chords sorted by note type
    auto completedChords = m_chordGuesser.Guess(neckChord);
    std::vector<std::list<Chord>> completedChordsSorted(AllNoteTypes().size(), std::list<Chord>());
    for (const auto &chord : completedChords)
    {
        auto baseNote = chord.GetBaseNote();
        if (baseNote.has_value())
        {
            NoteType nt = baseNote.value().GetNoteType();
            int listNum = static_cast<int>(nt);
            completedChordsSorted[listNum].push_back(chord);
        }
    }

    // complete strings
    for (const auto& lst : completedChordsSorted)
    {
        QString str;
        for (const auto& chord : lst)
        {
            if (!str.isEmpty())
            {
                str += " ";
            }
            str += QString::fromStdString(chord.GetName());
        }

        // if has chords, complete the list
        if (!str.isEmpty())
        {
            ui->ChordIdentifier_ListCompletedChords->addItem(str);
        }
    }
}

void MainWindow::ChordIdentifier_UpdateUncompleteChords()
{
    ui->ChordIdentifier_ListUncompletedChords->clear();

    Neck neck = ui->ChordIdentifier_ChordSelector->GetNeck();
    Chord neckChord = neck.toChord();

    // this chord should have at least 2 notes
    if (neckChord.GetNotes().size() < 2) return;

    // uncompleted chords sorted by note type
    auto uncompletedChords = m_chordGuesser.GuessUncomplete(neckChord);
    std::vector<std::list<Chord>> uncompletedChordsSorted(AllNoteTypes().size(), std::list<Chord>());
    for (const auto &chord : uncompletedChords)
    {
        auto baseNote = chord.GetBaseNote();
        if (baseNote.has_value())
        {
            NoteType nt = baseNote.value().GetNoteType();
            int listNum = static_cast<int>(nt);
            uncompletedChordsSorted[listNum].push_back(chord);
        }
    }

    // complete strings
    for (const auto& lst : uncompletedChordsSorted)
    {
        QString str;
        for (const auto& chord : lst)
        {
            if (!str.isEmpty())
            {
                str += " ";
            }
            str += QString::fromStdString(chord.GetName());
        }

        // if has chords, complete the list
        if (!str.isEmpty())
        {
            ui->ChordIdentifier_ListUncompletedChords->addItem(str);
        }
    }
}

void MainWindow::ChordIdentifier_UpdateNoteVisualizer()
{
    Neck neck = ui->ChordIdentifier_ChordSelector->GetNeck();
    Chord neckChord = neck.toChord();
    ui->ChordIdentifier_NotesVisualizer->SetNotes(neckChord.GetNotes());
    ui->ChordIdentifier_NotesVisualizer->update();
}

void MainWindow::ChordDatabase_ShowChord()
{
    NoteType baseNote = ui->ChordDatabase_BaseNoteSelector->GetNoteType();
    ChordType chordType = ui->ChordDatabase_ChordTypeSelector->GetChordType();
    Chord chord = ChordFactory::GenerateChord(Note(baseNote), chordType);

    // update chord name
    ui->ChordDatabase_ChordName->setText(QString::fromStdString(chord.GetName()));

    // update neck notes
    Neck neck;
    neck.SelectAllNotes(chord.GetNotes());
    ui->ChordDatabase_NeckChordNotes->SetNeck(neck, baseNote);
    ui->ChordDatabase_NeckChordNotes->update();

    // show chords positions
    auto chordNotations = NotationReader::GetNeckNotation(chord);
    std::list<Neck> neckLst;
    for (auto& notation : chordNotations)
    {
        neckLst.push_back(NeckEncoder::StringToNeck(notation));
    }
    ui->ChordDatabase_ChordList->SetNeckList(neckLst, baseNote, chord.GetName());
    ui->ChordDatabase_ChordList->update();

    // update notes visualizer
    ui->ChordDatabase_NotesVisualizer->SetNotes(chord.GetNotes());
    ui->ChordDatabase_NotesVisualizer->SetBaseNote(baseNote);
    ui->ChordDatabase_NotesVisualizer->update();
}

void MainWindow::ScaleIdentifier_UpdateNoteVisualizer()
{
    Neck neck = ui->ScaleIdentifier_NotesSelector->GetNeck();
    Chord neckChord = neck.toChord();
    ui->ScaleIdentifier_NotesVisualizer->SetNotes(neckChord.GetNotes());
    ui->ScaleIdentifier_NotesVisualizer->update();
}

void MainWindow::ScaleIdentifier_UpdateUncompletedScales()
{
    ui->ScaleIdentifier_UncompletedScales->clear();

    Neck neck = ui->ScaleIdentifier_NotesSelector->GetNeck();
    auto neckNotes = neck.toChord().GetNotes();

    // 2 notes at least
    if (neckNotes.size() < 2) return;

    auto guessedScales = m_scaleGuesser.GuessUncomplete(neckNotes);
    for (const auto& scale : guessedScales)
    {
        QString scaleStr = QString::fromStdString(scale.GetName());
        ui->ScaleIdentifier_UncompletedScales->addItem(scaleStr);
    }
}

void MainWindow::ScaleGenerator_Update()
{
    ScaleGenerator_UpdateNoteVisualizer();
    ScaleGenerator_UpdateNeckWidget();
    ScaleGenerator_UpdateChordsList();
}

void MainWindow::ScaleGenerator_UpdateNoteVisualizer()
{
    NoteType baseNote = ui->ScaleGenerator_SelectBaseNote->GetNoteType();
    ScaleType scaleType = ui->ScaleGenerator_SelectScale->GetScaleType();
    Scale scale = ScaleFactory::GenerateScale(baseNote, scaleType);

    ui->ScaleGenerator_NotesVisualizer->SetNotes(scale.GetNotes());
    ui->ScaleGenerator_NotesVisualizer->SetBaseNote(baseNote);
    ui->ScaleGenerator_NotesVisualizer->update();
}

void MainWindow::ScaleGenerator_UpdateNeckWidget()
{
    NoteType baseNote = ui->ScaleGenerator_SelectBaseNote->GetNoteType();
    ScaleType scaleType = ui->ScaleGenerator_SelectScale->GetScaleType();
    Scale scale = ScaleFactory::GenerateScale(baseNote, scaleType);

    Neck neck;
    neck.SelectAllNotes(scale.GetNotes());
    ui->ScaleGenerator_NeckVisualizer->SetNeck(neck, baseNote);
    ui->ScaleGenerator_NeckVisualizer->update();
}

void MainWindow::ScaleGenerator_UpdateChordsList()
{
    ui->ScaleGenerator_ChordsList->clear();

    NoteType baseNote = ui->ScaleGenerator_SelectBaseNote->GetNoteType();
    ScaleType scaleType = ui->ScaleGenerator_SelectScale->GetScaleType();
    auto chords = ChordFactory::GenerateAllChordsForScale(Note(baseNote), scaleType);

    // separate chords by their bases

    // 1. Create chord lists
    std::unordered_map<NoteType, std::list<Chord>> separatedChords;
    for (const auto &nt : AllNoteTypes())
    {
        separatedChords.insert({ nt, std::list<Chord>() });
    }

    // 2. separate chords
    for (const auto &chord : chords)
    {
        auto baseNote = chord.GetBaseNote();
        if (baseNote.has_value())
        {
            NoteType nt = baseNote.value().GetNoteType();
            separatedChords[nt].push_back(chord);
        }
    }

    // 3. Fill the list
    for (const auto &nt : AllNoteTypes())
    {
        const auto &chords = separatedChords[nt];
        if (chords.empty()) continue;

        std::string str;
        for (const auto &chord : chords)
        {
            if (!str.empty())
            {
                str += " ";
            }
            str += chord.GetName();
        }
        ui->ScaleGenerator_ChordsList->addItem(QString::fromStdString(str));
    }
}

void MainWindow::Dev_ShowChord()
{
    NoteType baseNote = ui->Dev_BaseNoteSelector->GetNoteType();
    ChordType chordType = ui->Dev_ChordTypeSelector->GetChordType();
    Chord chord = ChordFactory::GenerateChord(Note(baseNote), chordType);

    // update chord name
    ui->Dev_ChordName->setText(QString::fromStdString(chord.GetName()));

    // update neck notes
    Neck neck;
    neck.SelectAllNotes(chord.GetNotes());
    ui->Dev_NeckChordNotes->SetNeck(neck, baseNote);
    ui->Dev_NeckChordNotes->update();

    // update note visualizer
    ui->Dev_NotesVisualizer->SetNotes(chord.GetNotes());
    ui->Dev_NotesVisualizer->SetBaseNote(baseNote);
    ui->Dev_NotesVisualizer->update();
}

void MainWindow::Dev_UpdateNeckNotation()
{
    ui->Dev_LineChordNames->clear();
    ui->Dev_LineNeckNotation->clear();

    Neck neck = ui->Dev_ChordSelector->GetNeck();
    Chord neckChord = neck.toChord();

    // update chord notation
    auto chordStr = NeckEncoder::NeckToString(neck);
    ui->Dev_LineNeckNotation->setText(QString::fromStdString(chordStr));

    // this chord should have at least 2 notes
    if (neckChord.GetNotes().size() < 2) return;

    // completed chords sorted by note type
    auto completedChords = m_chordGuesser.Guess(neckChord);

    // complete list of chords
    QString chordsStr;
    for (const auto &chord : completedChords)
    {
        if (!chordsStr.isEmpty())
        {
            chordsStr += " ";
        }
        chordsStr += QString::fromStdString(chord.GetName());
    }
    ui->Dev_LineChordNames->setText(chordsStr);
}

void MainWindow::Dev_UpdateStatistics()
{
    int allKnownChordsNumber = 0;
    int allChordDiagramNumber = 0;

    // init all chords
    for (auto& noteType : AllNoteTypes())
    {
        for (auto& chordType : AllChordsTypes())
        {
            allKnownChordsNumber++;
            Chord chord = ChordFactory::GenerateChord(Note(noteType), chordType);
            auto chordNotations = NotationReader::GetNeckNotation(chord);
            allChordDiagramNumber += chordNotations.size();
        }
    }

    ui->Dev_StatisticsChordNumber->setText("Database chord number: " +
                                           QString::number(allKnownChordsNumber));
    ui->Dev_StatisticsChordDiagramNumber->setText("Database diagram number: " +
                                                   QString::number(allChordDiagramNumber));
}

void MainWindow::on_Dev_TestAllChordsValid_clicked()
{
    QString failChordsStr;

    // generate all known chords
    std::list<Chord> m_allKnownChords;
    for (auto& noteType : AllNoteTypes())
    {
        for (auto& chordType : AllChordsTypes())
        {
            Chord chord = ChordFactory::GenerateChord(Note(noteType), chordType);
            m_allKnownChords.push_back(chord);
        }
    }

    // generate notations for each chord
    for (const auto &chord : m_allKnownChords)
    {
        // chord 1 set
        std::set<NoteType> chordNotesSet;
        for (const auto& note : chord.GetNotes())
        {
            chordNotesSet.insert(note.GetNoteType());
        }

        auto chordNotations = NotationReader::GetNeckNotation(chord);

        // no chord notation? very bad
        if (chordNotations.empty())
        {
            failChordsStr += QString::fromStdString(chord.GetName()) + " ";
        }
        else
        {
            // try to validate every chord notation
            for (auto& notation : chordNotations)
            {
                Neck neck = NeckEncoder::StringToNeck(notation);
                Chord notationChord = neck.toChord();
                auto possibleChordsFromNotation = m_chordGuesser.Guess(notationChord);

                // check if one of the chords has the same name
                bool validChords = false;
                for (const auto& c : possibleChordsFromNotation)
                {
                    if (c.GetName() == chord.GetName())
                    {
                        validChords = true;
                        break;
                    }
                }

                if (chord.GetName().empty())
                {
                    validChords = false;
                }

                if (!validChords)
                {
                    failChordsStr += QString::fromStdString(chord.GetName()) + " ";
                }
            }
        }
    }

    if (failChordsStr.isEmpty())
    {
        QMessageBox::information(this, "Chords validation", "All chords fine!");
    }
    else
    {
        QMessageBox::information(this, "Chords validation", "Wrong chords: " + failChordsStr);
    }
}

void MainWindow::on_ScaleGenerator_GenerateScalePDF_clicked()
{
    NoteType baseNote = ui->ScaleGenerator_SelectBaseNote->GetNoteType();
    ScaleType scaleType = ui->ScaleGenerator_SelectScale->GetScaleType();
    QString filename = PDFGenerator::GenerateScaleReport(baseNote, scaleType);

    // open file
    QDesktopServices::openUrl(QUrl::fromLocalFile(filename));
}


void MainWindow::on_ChordDatabase_GenerateAllChordsPDF_clicked()
{
}

