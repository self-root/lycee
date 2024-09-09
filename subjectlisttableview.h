#ifndef SUBJECTLISTTABLEVIEW_H
#define SUBJECTLISTTABLEVIEW_H

#include <QTableView>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>

#include "klass.h"

#include "subjectlistmodel.h"

class SubjectListTableView : public QTableView
{
    Q_OBJECT
public:
    SubjectListTableView(QTableView *parent=nullptr);

    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

    void contextMenuEvent(QContextMenuEvent *event) override;

    SubjectListModel *model = nullptr;
public slots:
    void onGetSubjectsFor(const Klass &klass);

    void addSubject(const QString &subjectName, int subjectCoef);

    void onDeleteAction();

signals:
    void selectedSubject(const Subject &subject);

private:
    QMenu *contextMenu = nullptr;
    //QAction *deleteAction = nullptr;
};

#endif // SUBJECTLISTTABLEVIEW_H
