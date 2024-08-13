#include "gradestableview.h"
#include "controller.h"

GradesTableView::GradesTableView(QTableView *parent) : QTableView(parent)
{
    setSortingEnabled(true);
    proxyModel = new QSortFilterProxyModel;
    model = new GradeListModel;
    proxyModel->setSourceModel(model);
    this->setModel(proxyModel);
}

void GradesTableView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);

    if (current.isValid())
    {
        QString studentName = current.sibling(current.row(), 0).data().toString();
        qDebug() << "Row selected: " << current.row();
        qDebug() << "Student: " << current.sibling(current.row(), 0).data();
        Student student = model->studentFromName(studentName);
        emit studentSelected(student);
    }

}

void GradesTableView::loadStudentGrades(int klassID, int trimestre)
{
    model->loadModelData(trimestre, klassID);
    Controller::instance()->checkDbError();
}
