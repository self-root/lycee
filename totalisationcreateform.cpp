#include "totalisationcreateform.h"
#include "ui_totalisationcreateform.h"

TotalisationCreateForm::TotalisationCreateForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TotalisationCreateForm)
{
    ui->setupUi(this);
    sortBtnGroup = new QButtonGroup;
    orderBtnGroup = new QButtonGroup;
    sortBtnGroup->addButton(ui->rankRadio);
    sortBtnGroup->addButton(ui->numberRadio);
    orderBtnGroup->addButton(ui->ascRadio);
    orderBtnGroup->addButton(ui->descRadio);
    ui->numberRadio->setChecked(true);
}

TotalisationCreateForm::~TotalisationCreateForm()
{
    delete ui;
}

void TotalisationCreateForm::open(Target target)
{
    mTarget = target;
    if (target == Target::PDF)
    {
        setWindowTitle("Totalisation PDF");
        setWindowIcon(QIcon(":/icons/images/topdf.png"));
    }
    else
    {
        setWindowIcon(QIcon(":/icons/images/toexcel.png"));
        setWindowTitle("Totalisation Excel");
    }
    this->show();
}

void TotalisationCreateForm::on_okBtn_clicked()
{
    FilterBy by;
    Order order;
    if (ui->rankRadio->isChecked())
        by = FilterBy::Rank;
    else {
        by = FilterBy::Number;
    }

    if (ui->ascRadio->isChecked())
        order = Order::Ask;
    else
        order = Order::Desc;

    if (mTarget == Target::PDF)
        emit totalisationPdf(order, by);
    else
        emit totalisationExcel(order, by);
    close();
}


void TotalisationCreateForm::on_cancelBtn_clicked()
{
    close();
}


void TotalisationCreateForm::on_numberRadio_toggled(bool checked)
{
    if (checked)
    {
        ui->ascRadio->setChecked(true);
        ui->ascRadio->setEnabled(false);
        ui->descRadio->setEnabled(false);
    }

    else
    {
        ui->ascRadio->setEnabled(true);
        ui->descRadio->setDisabled(false);
    }
}

