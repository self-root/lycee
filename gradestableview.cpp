#include "gradestableview.h"
#include "controller.h"

GradesTableView::GradesTableView(QTableView *parent) : QTableView(parent)
{
    setSelectionBehavior(QTableView::SelectRows);
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
        const QModelIndex sourceIndex = proxyModel->mapToSource(current);
        Student student = model->studentAt(sourceIndex.row());
        emit studentSelected(student);
    }

}

void GradesTableView::loadStudentGrades(int klassID, int trimestre)
{
    model->loadModelData(trimestre, klassID);
    Controller::instance()->checkDbError();
}
