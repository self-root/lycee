#include "schoolyearsmodel.h"
#include "databaseaccess.h"
#include "controller.h"

SchoolYearsModel::SchoolYearsModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int SchoolYearsModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return schoolYears.length();
}

QVariant SchoolYearsModel::data(const QModelIndex &index, int role) const
{


    switch (role) {
    case Qt::DisplayRole:
        return schoolYears.at(index.row());
        break;
    default:
        break;
    }
    return QVariant();
}

void SchoolYearsModel::loadSchoolYears()
{
    beginResetModel();
    schoolYears = DatabaseAccess::instance()->schoolYears();
    Controller::instance()->checkDbError();
    endResetModel();
}

void SchoolYearsModel::addScoolYear(const QString &SchoolYear)
{
    DatabaseAccess::instance()->addSchoolYear(SchoolYear);
    loadSchoolYears();
}

void SchoolYearsModel::removeSchoolYear(const QString &schoolYear)
{
    DatabaseAccess::instance()->removeSchoolYear(schoolYear);
    loadSchoolYears();
}

QHash<int, QByteArray> SchoolYearsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Roles::SchoolYearRole] = "schoolYear";
    return roles;
}
