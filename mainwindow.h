#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chords/chordguesser.h"
#include "scales/scaleguesser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Dev_TestAllChordsValid_clicked();
    void on_ScaleGenerator_GenerateScalePDF_clicked();
    void on_ChordDatabase_GenerateAllChordsPDF_clicked();

private:
    void ChordIdentifier_UpdateCompleteChords();
    void ChordIdentifier_UpdateUncompleteChords();
    void ChordIdentifier_UpdateNoteVisualizer();
    void ChordDatabase_ShowChord();
    void ScaleIdentifier_UpdateNoteVisualizer();
    void ScaleIdentifier_UpdateUncompletedScales();
    void ScaleGenerator_Update();
    void ScaleGenerator_UpdateNoteVisualizer();
    void ScaleGenerator_UpdateNeckWidget();
    void ScaleGenerator_UpdateChordsList();
    void Dev_ShowChord();
    void Dev_UpdateNeckNotation();
    void Dev_UpdateStatistics();

    Ui::MainWindow *ui;
    ChordGuesser m_chordGuesser;
    ScaleGuesser m_scaleGuesser;
};
#endif // MAINWINDOW_H
