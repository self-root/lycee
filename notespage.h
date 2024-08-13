#ifndef NOTESPAGE_H
#define NOTESPAGE_H

#include <QWidget>
#include <QTabWidget>

#include "gradestableview.h"


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

    void on_pushButton_clicked();

    void on_pasteBtn_clicked();

private:
    Ui::NotesPage *ui;
    QTabWidget *tabWidget = nullptr;
    GradesTableView *gradesView = nullptr;
    void loadSchoolYear();
    void clearGradeForm();

    GradeMetaData currentGrade;

};

#endif // NOTESPAGE_H
