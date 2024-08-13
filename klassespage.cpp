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
    schoolYearsView = new SchoolYearsView;
    ui->leftLayout->insertWidget(1, klassLisetView);
    ui->rightLayout->insertWidget(1, subjectTableView);
    ui->schoolyearLayout->insertWidget(1, schoolYearsView);
    setSubjectEditComboValues();

    QObject::connect(klassLisetView, &KlassListView::getSubjectListsFor, subjectTableView, &SubjectListTableView::onGetSubjectsFor);
    QObject::connect(schoolYearsView, &SchoolYearsView::currentSchoolYearChanged, klassLisetView, &KlassListView::loadKlasses);
    QObject::connect(subjectTableView, &SubjectListTableView::selectedSubject, this, &KlassesPage::onSubjectSelected);
}

KlassesPage::~KlassesPage()
{
    delete ui;
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

    if (subjectName.compare(currentSubject.subjectName(), Qt::CaseInsensitive) == 0)
    {
        qDebug() << "Update subject";
        currentSubject.setSubjectName(subjectName);
        currentSubject.setSubjectCoef(ui->coefSpinBox->value());
        subjectTableView->model->updateSubject(currentSubject);
    }

    else
    {
        int coef = ui->coefSpinBox->value();
        subjectTableView->addSubject(subjectName, coef);
    }
    ui->subjectNameCombo->clearEditText();
    setSubjectEditComboValues();
}


void KlassesPage::on_schoolYearBtn_clicked()
{
    if (!ui->schoolYearEdit->text().isEmpty())
    {
        schoolYearsView->model->addScoolYear(ui->schoolYearEdit->text());
        ui->schoolYearEdit->clear();
    }
}

void KlassesPage::onSubjectSelected(const Subject &subject)
{
    currentSubject = subject;
    ui->subjectNameCombo->setCurrentText(subject.subjectName());
    ui->coefSpinBox->setValue(subject.subjectCoef());
}

