#ifndef KLASSLISTVIEW_H
#define KLASSLISTVIEW_H

#include <QListView>
#include <QSortFilterProxyModel>
#include "klasslistmodel.h"

class KlassListView : public QListView
{
    Q_OBJECT
public:
    KlassListView(QListView *parent=nullptr);

    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

public slots:
    void loadKlasses(const QString &schoolYear);
    void addClass(const QString &className);

private:
    KlassListModel *model = nullptr;
    //QSortFilterProxyModel *proxyModel = nullptr;

signals:
    void getSubjectListsFor(const Klass &klass);
};

#endif // KLASSLISTVIEW_H
