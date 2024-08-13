#include "schoolyearsview.h"

SchoolYearsView::SchoolYearsView(QListView *parent)
    : QListView(parent), model(new SchoolYearsModel)
{
    setModel(model);
    model->loadSchoolYears();
}

void SchoolYearsView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)
    if (current.isValid())
        emit currentSchoolYearChanged(model->data(current).toString());
}
