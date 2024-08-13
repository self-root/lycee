#ifndef KLASSESPAGE_H
#define KLASSESPAGE_H

#include <QWidget>
#include "klasslistview.h"
#include "subjectlisttableview.h"
#include "schoolyearsview.h"

namespace Ui {
class KlassesPage;
}

class KlassesPage : public QWidget
{
    Q_OBJECT

public:
    explicit KlassesPage(QWidget *parent = nullptr);
    ~KlassesPage();

private slots:


    void on_addClassBtn_clicked();

    void setSubjectEditComboValues();

    void on_addSubjectBtn_clicked();

    void on_schoolYearBtn_clicked();

    void onSubjectSelected(const Subject &subject);

private:
    Ui::KlassesPage *ui;
    KlassListView *klassLisetView = nullptr;
    SubjectListTableView *subjectTableView = nullptr;
    SchoolYearsView *schoolYearsView = nullptr;
    Subject currentSubject;
};

#endif // KLASSESPAGE_H
