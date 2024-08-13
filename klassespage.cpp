#include "klassespage.h"
#include "ui_klassespage.h"
#include "controller.h"
#include "databaseaccess.h"

KlassesPage::KlassesPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::KlassesPage)
{
    ui->setupUi(this);
    klassLisetView = new KlassListView;
    subjectTableView = new SubjectListTableView;
    ui->leftLayout->insertWidget(0, klassLisetView);
    ui->rightLayout->insertWidget(0, subjectTableView);
    setSchoolYearComboboxValues();
    setSubjectEditComboValues();

    QObject::connect(klassLisetView, &KlassListView::getSubjectListsFor, subjectTableView, &SubjectListTableView::onGetSubjectsFor);
}

KlassesPage::~KlassesPage()
{
    delete ui;
}


void KlassesPage::setSchoolYearComboboxValues()
{
    for (const QString &year: Controller::instance()->schoolYears)
        ui->schoolYearCombo->addItem(year);
}


void KlassesPage::on_schoolYearCombo_activated(int index)
{
    QString schoolYear = ui->schoolYearCombo->currentText();
    qDebug() << __FUNCTION__ << " " << index << " -> " << schoolYear;
    klassLisetView->loadKlasses(schoolYear);
}


void KlassesPage::on_addClassBtn_clicked()
{
    if (ui->classLineEdit->text().isEmpty())
        return;

    klassLisetView->addClass(ui->classLineEdit->text());
    ui->classLineEdit->clear();
}

void KlassesPage::setSubjectEditComboValues()
{
    ui->subjectNameCombo->clear();
    QStringList subjects = DatabaseAccess::instance()->groupedSubjectNames();
    subjects.insert(0, "");
    ui->subjectNameCombo->addItems(subjects);
    Controller::instance()->checkDbError();
}


void KlassesPage::on_addSubjectBtn_clicked()
{
    QString subjectName = ui->subjectNameCombo->currentText();
    if (subjectName.isEmpty())
        return;

    int coef = ui->coefSpinBox->value();
    subjectTableView->addSubject(subjectName, coef);
    ui->subjectNameCombo->clearEditText();
    setSubjectEditComboValues();
}

