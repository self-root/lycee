#include "subjectlistmodel.h"
#include "databaseaccess.h"
#include "controller.h"

SubjectListModel::SubjectListModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

QVariant SubjectListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role) {
    case Qt::DisplayRole:{
        if (orientation == Qt::Horizontal)
        {
            switch (section) {
            case 0:
                return "Matière";
                break;
            case 1:
                return "Coefficient";
                break;
            default:
                break;
            }
        }

        else if (orientation == Qt::Vertical)
        {
            return section + 1;
        }
        break;
    }
    default:
        break;
    }

    return QVariant();
}

int SubjectListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return subjects.size();
}

int SubjectListModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 2;
}

QVariant SubjectListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    Subject subject = subjects.at(index.row());
    int column = index.column();
    switch (role) {
    case Qt::DisplayRole:
        switch (column) {
        case 0:
            return subject.subjectName();
            break;
        case 1:
            return subject.subjectCoef();
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return QVariant();
}

void SubjectListModel::loadSubjectLists(int klassID)
{
    currentClasss =  klassID;
    beginResetModel();
    subjects.clear();
    subjects = DatabaseAccess::instance()->getSubjectByClass(currentClasss);
    endResetModel();
}

void SubjectListModel::addSubject(const QString &subjectName, int subjectCoef)
{
    Subject subject;
    subject.setSubjectName(subjectName);
    subject.setSubjectCoef(subjectCoef);
    DatabaseAccess::instance()->addSubject(subject, currentClasss);
    loadSubjectLists(currentClasss);

}

Subject SubjectListModel::subjectAt(int index) const
{
    return subjects.at(index);
}

void SubjectListModel::updateSubject(const Subject &subject)
{
    DatabaseAccess::instance()->updateSubject(subject);
    Controller::instance()->checkDbError();
    loadSubjectLists(currentClasss);
}

void SubjectListModel::removeSubject(const Subject &subject)
{
    DatabaseAccess::instance()->removeSubject(subject);
    Controller::instance()->checkDbError();
    loadSubjectLists(currentClasss);
}
