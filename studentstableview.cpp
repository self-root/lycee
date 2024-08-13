#include "studentstableview.h"
#include "controller.h"

#include <QDropEvent>
#include <QMimeData>

StudentsTableView::StudentsTableView(QTableView *parent) : QTableView(parent)
{
    setSortingEnabled(true);
    proxyModel = new QSortFilterProxyModel;
    model = new StudentListModel;
    proxyModel->setSourceModel(model);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setModel(proxyModel);
}

void StudentsTableView::loadStudents(int id)
{
    model->loadStudents(id);
}

Student StudentsTableView::selectedStudent()
{
    Student student;
    if (currentIndex().row() >= 0)
    {
        QModelIndex index = currentIndex().siblingAtColumn(1);

        qDebug() << "Current student: " << proxyModel->data(index);
        student = model->studentByName(proxyModel->data(index).toString());
    }
    return student;
}

void StudentsTableView::deleteStudent()
{
    Student student = selectedStudent();
    model->deleteStudent(student);
    Controller::instance()->checkDbError();
}

void StudentsTableView::dropEvent(QDropEvent *event)
{
    qDebug() << "Drop event";
    qDebug() << event->mimeData();
}

void StudentsTableView::onSaveStudent(Student student)
{
    model->addStudent(student);
    Controller::instance()->checkDbError();
}

void StudentsTableView::onUpdateStudent(Student student)
{
    qDebug() << "Updating student: " << student.name() << "ID: " << student.id();
    model->updateStudent(student);
    Controller::instance()->checkDbError();
}
