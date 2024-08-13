#include "klasslistview.h"
#include "controller.h"

KlassListView::KlassListView(QListView *parent) : QListView(parent)
{
    //proxyModel = new QSortFilterProxyModel;
    model = new KlassListModel;
    //proxyModel->setSourceModel(model);
    this->setModel(model);
}

void KlassListView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);

    int currentRow = current.row();
    if (currentRow >= 0)
    {
        emit getSubjectListsFor(model->subjectAt(currentRow));
    }
}

void KlassListView::loadKlasses(const QString &schoolYear)
{
    model->loadData(schoolYear);
    Controller::instance()->checkDbError();
}

void KlassListView::addClass(const QString &className)
{
    model->addClass(className);
}

