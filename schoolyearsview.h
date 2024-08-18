#ifndef SCHOOLYEARSVIEW_H
#define SCHOOLYEARSVIEW_H

#include <QListView>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>

#include "schoolyearsmodel.h"

class SchoolYearsView : public QListView
{
    Q_OBJECT
public:
    SchoolYearsView(QListView *parent=nullptr);

    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

    void contextMenuEvent(QContextMenuEvent *event) override;

    SchoolYearsModel *model = nullptr;

signals:
    void currentSchoolYearChanged(const QString &current);

private:
    QMenu *menu = nullptr;
    QAction *deleteMenu = nullptr;

    void setupMenu();

private slots:
    void onDelete(bool triggered);
signals:
    void schoolYearDeleted();
};

#endif // SCHOOLYEARSVIEW_H
