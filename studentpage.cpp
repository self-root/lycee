#include "studentpage.h"
#include "ui_studentpage.h"
#include "controller.h"
#include <QMessageBox>

StudentPage::StudentPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StudentPage)
{
    ui->setupUi(this);
    tableView = new StudentsTableView;
    ui->studentPageLeftLayout->addWidget(tableView);
    setSchoolYearComboboxValues();
    newStudentForm = new NewStudentForm;

    QObject::connect(ui->schoolYearCombo, &QComboBox::activated, this, &StudentPage::onScoolYearComboIndexChanged);
    QObject::connect(ui->classCombo, &QComboBox::activated, this, &StudentPage::onKlassComboActivated);
    QObject::connect(newStudentForm, &NewStudentForm::saveStudent, tableView, &StudentsTableView::onSaveStudent);
    QObject::connect(newStudentForm, &NewStudentForm::updateStudent, tableView, &StudentsTableView::onUpdateStudent);
}

StudentPage::~StudentPage()
{
    delete ui;
}

void StudentPage::setSchoolYearComboboxValues()
{
    for (const QString &year: Controller::instance()->schoolYears)
        ui->schoolYearCombo->addItem(year);
}

void StudentPage::onScoolYearComboIndexChanged(int i)
{
    QString schoolYear = ui->schoolYearCombo->currentText();
    qDebug() << __FUNCTION__ << " " << i << " -> " << schoolYear;
    Controller::instance()->getKlassList(schoolYear);
    ui->classCombo->clear();
    ui->classCombo->addItem("--");
    for (const Klass &klass : Controller::instance()->klasses)
    {
        ui->classCombo->addItem(klass.className());
    }
}

void StudentPage::onKlassComboActivated(int index)
{
    Q_UNUSED(index)
    Klass klass = Controller::instance()->klassByName(ui->classCombo->currentText());
    tableView->loadStudents(klass.classId());
    Controller::instance()->checkDbError();

}

void StudentPage::on_addStudentBtn_clicked()
{
    if (ui->classCombo->currentIndex() < 1)
        return;

    newStudentForm->display(NewStudentForm::Intent::Save);
}


void StudentPage::on_editStidentBtn_clicked()
{
    if (tableView->currentIndex().row() >= 0)
    {
        newStudentForm->setEditingStudent(tableView->selectedStudent());
        newStudentForm->display(NewStudentForm::Intent::Update);
    }
}


void StudentPage::on_removeStudentBtn_clicked()
{

    if (tableView->currentIndex().row() >= 0)
    {
        int ret = QMessageBox::warning(this, "Effacer un/une élève", QString("Supprimer %1 de la base de données?").arg(tableView->selectedStudent().name()), QMessageBox::Button::Cancel | QMessageBox::Button::Ok);
        if (ret == QMessageBox::Button::Ok)
            tableView->deleteStudent();
        else {
            return;
        }
    }
}

