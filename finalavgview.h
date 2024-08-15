#ifndef FINALAVGVIEW_H
#define FINALAVGVIEW_H

#include <QTableView>
#include <QSortFilterProxyModel>

#include "finalavgtablemodel.h"

class FinalAVGView : public QTableView
{
    Q_OBJECT
public:
    FinalAVGView(QTableView *parent=nullptr);
    FinalAVGTableModel *model = nullptr;

private:
    QSortFilterProxyModel *proxyModel = nullptr;

};

#endif // FINALAVGVIEW_H
