#ifndef NOTESPAGE_H
#define NOTESPAGE_H

#include <QWidget>
#include <QTabWidget>
#include <QToolBar>
#include <QAction>

#include "gradestableview.h"
#include "finalavgview.h"
#include "utils.h"
#include "totalisationcreateform.h"


namespace Ui {
class NotesPage;
}

class NotesPage : public QWidget
{
    Q_OBJECT

public:
    explicit NotesPage(QWidget *parent = nullptr);
    ~NotesPage();

private slots:
    void on_schoolYearCombo_activated(int index);

    void on_classCombo_activated(int index);

    void on_trimestreCombo_activated(int index);

    void setUpsubjectLists(int klassID);

    void onStudentSelected(const Student &student);

    void setGradeData();

    void on_subjectCombo_activated(int index);

    void on_saveGradeBtn_clicked();

    void onPaste(bool _);

    void onCompute(bool _);

    void onComputeFinalAVG(bool _);

    void onCreateTranscript(bool _);

    void onSaveGradesPDFAction(bool _);

    void onSaveGradesExcelAction(bool _);

    void onFicheDeNote(bool _);

public slots:
    void loadSchoolYear();
    void onToPDF(Order order, FilterBy by);
    void onToExcel(Order order, FilterBy by);

private:
    Ui::NotesPage *ui;
    QTabWidget *tabWidget = nullptr;
    GradesTableView *gradesView = nullptr;
    FinalAVGView *generalAVGView = nullptr;
    void clearGradeForm();
    void setupToolBar();
    QToolBar *toolbar = nullptr;
    QAction *pasteAction = nullptr;
    QAction *computeAction = nullptr;
    QAction *generalComputeAction = nullptr;
    QAction *createTranscriptAction = nullptr;
    QAction *gradesToPDFAction = nullptr;
    QAction *gradesToExcelAction = nullptr;
    QAction *ficheDeNoteAction = nullptr;
    GradeMetaData currentGrade;
    TotalisationCreateForm *totalisationForm = nullptr;

};

#endif // NOTESPAGE_H
