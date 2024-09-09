#include "studentpage.h"
#include "ui_studentpage.h"
#include "controller.h"
#include <QMessageBox>
#include <QIcon>

StudentPage::StudentPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StudentPage)
{
    ui->setupUi(this);
    tableView = new StudentsTableView;
    ui->studentPageLeftLayout->addWidget(tableView);
    setupActions();
    setSchoolYearComboboxValues();
    newStudentForm = new NewStudentForm;

    QObject::connect(ui->schoolYearCombo, &QComboBox::activated, this, &StudentPage::onScoolYearComboIndexChanged);
    QObject::connect(ui->classCombo, &QComboBox::activated, this, &StudentPage::onKlassComboActivated);
    QObject::connect(newStudentForm, &NewStudentForm::saveStudent, tableView, &StudentsTableView::onSaveStudent);
    QObject::connect(newStudentForm, &NewStudentForm::updateStudent, tableView, &StudentsTableView::onUpdateStudent);
    QObject::connect(tableView, &StudentsTableView::addStudent, this, &StudentPage::on_addStudentBtn_clicked);
    QObject::connect(tableView, &StudentsTableView::updateStudent, this, &StudentPage::on_editStidentBtn_clicked);
}

StudentPage::~StudentPage()
{
    delete ui;
}

void StudentPage::setSchoolYearComboboxValues()
{
    ui->schoolYearCombo->clear();
    ui->schoolYearCombo->addItem("--");
    Controller::instance()->getSchoolyears();
    for (const QString &year: Controller::instance()->schoolYears)
        ui->schoolYearCombo->addItem(year);
}

void StudentPage::setupActions()
{
    toolBar = new QToolBar;
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolBar->setMovable(true);
    addAction = new QAction(QIcon(":/icons/images/add_student.png"), "Ajouter");
    editAction = new QAction(QIcon(":/icons/images/edit-profile.png"), "Editer");
    removeAction = new QAction(QIcon(":/icons/images/delete.png"), "Supprimer");
    toolBar->addAction(addAction);
    toolBar->addAction(editAction);
    toolBar->addAction(removeAction);

    ui->actionLayout->addWidget(toolBar);

    QObject::connect(addAction, &QAction::triggered, this, [&](bool _){
        Q_UNUSED(_)
        this->on_addStudentBtn_clicked();
    });

    QObject::connect(removeAction, &QAction::triggered, this, [&](bool _){
        Q_UNUSED(_)
        this->on_removeStudentBtn_clicked();
    });

    QObject::connect(editAction, &QAction::triggered, this, [&](bool _){
        Q_UNUSED(_)
        this->on_editStidentBtn_clicked();
    });

}


void StudentPage::onSchoolYearAdded()
{
    setSchoolYearComboboxValues();
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

