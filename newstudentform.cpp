#include "newstudentform.h"
#include "ui_newstudentform.h"
#include <QDate>

NewStudentForm::NewStudentForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NewStudentForm)
{
    ui->setupUi(this);
    ui->dateNaissEdit->setDisplayFormat("dd-MM-yyyy");
    ui->dateNaissEdit->setCalendarPopup(true);
}

NewStudentForm::~NewStudentForm()
{
    delete ui;
}

void NewStudentForm::on_saveBtn_clicked()
{
    if (ui->nameEdit->text().isEmpty())
    {
        return;
    }

    if (intent == Intent::Save)
    {
        Student student;
        student.setName(ui->nameEdit->text());
        student.setBirthDay(ui->dateNaissEdit->text());
        student.setMatricule(ui->matriculeEdit->text());
        student.setNumber(ui->numberSpin->value());
        student.setSexe(ui->sexeCombo->currentText());
        student.setSituation(ui->situationCombo->currentText());
        emit saveStudent(student);
        close();
    }

    else if (intent == Intent::Update){
        Student student = editingStudent;
        student.setName(ui->nameEdit->text());
        student.setBirthDay(ui->dateNaissEdit->text());
        student.setMatricule(ui->matriculeEdit->text());
        student.setNumber(ui->numberSpin->value());
        student.setSexe(ui->sexeCombo->currentText());
        student.setSituation(ui->situationCombo->currentText());
        emit updateStudent(student);
        close();
    }

}


void NewStudentForm::on_cancelBtn_clicked()
{
    close();
}

Student NewStudentForm::getEditingStudent() const
{
    return editingStudent;
}

void NewStudentForm::setEditingStudent(const Student &newEditingStudent)
{
    editingStudent = newEditingStudent;
}

void NewStudentForm::display(Intent _intent)
{
    intent = _intent;
    if (_intent == Intent::Update)
    {
        qDebug() << "Updating: " << editingStudent.name();
        ui->nameEdit->setText(editingStudent.name());
        ui->matriculeEdit->setText(editingStudent.matricule());
        qDebug() << "Date: " << QDate::fromString(editingStudent.birthDay(), "dd-MM-yyyy");
        ui->dateNaissEdit->setDate(QDate::fromString(editingStudent.birthDay(), "dd-MM-yyyy"));
        if (editingStudent.sexe() == "Garçon" || editingStudent.sexe() == "M")
            ui->sexeCombo->setCurrentIndex(0);
        else
            ui->sexeCombo->setCurrentIndex(1);

        if (editingStudent.situation() == "Passant(e)" || editingStudent.situation() == "P")
            ui->situationCombo->setCurrentIndex(0);
        else
            ui->situationCombo->setCurrentIndex(1);
        ui->numberSpin->setValue(editingStudent.number());
    }

    this->show();
}

NewStudentForm::Intent NewStudentForm::getIntent() const
{
    return intent;
}

void NewStudentForm::setIntent(Intent newIntent)
{
    intent = newIntent;
}

