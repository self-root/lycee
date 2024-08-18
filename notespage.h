#ifndef NOTESPAGE_H
#define NOTESPAGE_H

#include <QWidget>
#include <QTabWidget>
#include <QToolBar>
#include <QAction>

#include "gradestableview.h"
#include "finalavgview.h"


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

public slots:
    void loadSchoolYear();

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
    GradeMetaData currentGrade;

};

#endif // NOTESPAGE_H
