#ifndef STUDENTPAGE_H
#define STUDENTPAGE_H

#include <QWidget>
#include <QAction>
#include <QToolBar>
#include "studentstableview.h"
#include "newstudentform.h"

namespace Ui {
class StudentPage;
}

class StudentPage : public QWidget
{
    Q_OBJECT

public:
    explicit StudentPage(QWidget *parent = nullptr);
    ~StudentPage();

private:
    Ui::StudentPage *ui;
    StudentsTableView *tableView = nullptr;
    NewStudentForm *newStudentForm = nullptr;
    QToolBar *toolBar = nullptr;
    QAction *addAction = nullptr;
    QAction *editAction = nullptr;
    QAction *removeAction = nullptr;

    void setSchoolYearComboboxValues();
    void setupActions();

public slots:
    void onSchoolYearAdded();

private slots:
    void onScoolYearComboIndexChanged(int i);
    void onKlassComboActivated(int index);
    void on_addStudentBtn_clicked();
    void on_editStidentBtn_clicked();
    void on_removeStudentBtn_clicked();
};

#endif // STUDENTPAGE_H
