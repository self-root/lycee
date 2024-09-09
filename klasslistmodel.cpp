#include "klasslistmodel.h"

#include "databaseaccess.h"
#include "controller.h"

KlassListModel::KlassListModel(QObject *parent)
    : QAbstractListModel(parent)
{}

QVariant KlassListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal)
    {
        if (section == 0)
        {
            switch (role) {
            case Qt::DisplayRole:
                return "Classes";
                break;
            default:
                break;
            }
        }
    }
    else
    {
        switch (role) {
        case Qt::DisplayRole:
            return section+1;
            break;
        default:
            break;
        }
    }

    return QVariant();
}

int KlassListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return klasses.size();
}

QVariant KlassListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        return klasses.at(index.row()).className();
        break;
    default:
        break;
    }

    return QVariant();
}

void KlassListModel::loadData(const QString &schoolYear)
{
    beginResetModel();
    klasses.clear();
    klasses = DatabaseAccess::instance()->gestClasses(schoolYear);
    currentSchoolYear = schoolYear;
    endResetModel();
}

Klass KlassListModel::klassAt(int index)
{
    Klass klass;
    if (std::size_t(index) < klasses.size())
        klass = klasses.at(index);
    return klass;
}

void KlassListModel::addClass(const QString &className)
{
    int schoolYearId = DatabaseAccess::instance()->schoolYearID(currentSchoolYear);
    if (schoolYearId > 0)
    {
        DatabaseAccess::instance()->addClass(className, schoolYearId);
        loadData(currentSchoolYear);
    }
    Controller::instance()->checkDbError();

}

void KlassListModel::removeKlass(const Klass &klass)
{
    DatabaseAccess::instance()->removeKlass(klass);
    Controller::instance()->checkDbError();
    loadData(currentSchoolYear);
}
