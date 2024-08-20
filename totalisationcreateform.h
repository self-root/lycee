#ifndef TOTALISATIONCREATEFORM_H
#define TOTALISATIONCREATEFORM_H

#include <QWidget>
#include <QButtonGroup>
#include "utils.h"

namespace Ui {
class TotalisationCreateForm;
}

class TotalisationCreateForm : public QWidget
{
    Q_OBJECT

public:
    explicit TotalisationCreateForm(QWidget *parent = nullptr);
    ~TotalisationCreateForm();

    void open(Target target);

private slots:
    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::TotalisationCreateForm *ui;
    QButtonGroup *sortBtnGroup = nullptr;
    QButtonGroup *orderBtnGroup = nullptr;
    Target mTarget;

signals:
    void totalisationPdf(Order order, FilterBy by);
    void totalisationExcel(Order order, FilterBy by);
};

#endif // TOTALISATIONCREATEFORM_H
