#ifndef KLASSLISTVIEW_H
#define KLASSLISTVIEW_H

#include <QListView>
#include <QSortFilterProxyModel>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>

#include "klasslistmodel.h"

class KlassListView : public QListView
{
    Q_OBJECT
public:
    KlassListView(QListView *parent=nullptr);

    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

    void contextMenuEvent(QContextMenuEvent *event) override;

    void setupMenu();

public slots:
    void loadKlasses(const QString &schoolYear);
    void addClass(const QString &className);
    void onDelete(bool triggered);

private:
    KlassListModel *model = nullptr;
    //QSortFilterProxyModel *proxyModel = nullptr;
    QMenu *menu = nullptr;
    QAction *deleteAction = nullptr;

signals:
    void getSubjectListsFor(const Klass &klass);
};

#endif // KLASSLISTVIEW_H
