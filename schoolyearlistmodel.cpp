#include "schoolyearlistmodel.h"
#include "databaseaccess.h"

#include <QByteArray>

SchoolYEarListModel::SchoolYEarListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    loadData();
}

int SchoolYEarListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    /*if (parent.isValid())
        return 0;*/

    return schoolYears.length();
}

QVariant SchoolYEarListModel::data(const QModelIndex &index, int role) const
{
    /*if (!index.isValid())
        return QVariant();*/

    switch (role) {
    case SchoolYear:
        return schoolYears.at(index.row());
        break;
    default:
        break;
    }
    return QVariant();
}

void SchoolYEarListModel::loadData()
{
    beginResetModel();
    schoolYears = DatabaseAccess::instance()->schoolYears();
    qDebug() << schoolYears;
    endResetModel();
}

QHash<int, QByteArray> SchoolYEarListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Roles::SchoolYear] = "schoolYear";

    return roles;
}
