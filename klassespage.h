#ifndef KLASSESPAGE_H
#define KLASSESPAGE_H

#include <QWidget>
#include "klasslistview.h"
#include "subjectlisttableview.h"

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


    void on_schoolYearCombo_activated(int index);

    void on_addClassBtn_clicked();

    void setSubjectEditComboValues();

    void on_addSubjectBtn_clicked();

private:
    Ui::KlassesPage *ui;
    void setSchoolYearComboboxValues();
    KlassListView *klassLisetView = nullptr;
    SubjectListTableView *subjectTableView = nullptr;
};

#endif // KLASSESPAGE_H
