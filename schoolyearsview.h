#ifndef SCHOOLYEARSVIEW_H
#define SCHOOLYEARSVIEW_H

#include <QListView>

#include "schoolyearsmodel.h"

class SchoolYearsView : public QListView
{
    Q_OBJECT
public:
    SchoolYearsView(QListView *parent=nullptr);
    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

    SchoolYearsModel *model = nullptr;

signals:
    void currentSchoolYearChanged(const QString &current);
};

#endif // SCHOOLYEARSVIEW_H
