#include "finalavgview.h"

FinalAVGView::FinalAVGView(QTableView *parent)
    : QTableView(parent)
{
    setSortingEnabled(true);
    setSelectionBehavior(QTableView::SelectRows);
    proxyModel = new QSortFilterProxyModel;
    model = new FinalAVGTableModel;
    proxyModel->setSourceModel(model);
    this->setModel(proxyModel);
}
