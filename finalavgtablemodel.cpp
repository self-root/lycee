#include "finalavgtablemodel.h"
#include "databaseaccess.h"
#include "controller.h"
#include "avgcalculator.h"

FinalAVGTableModel::FinalAVGTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    headers << "Numéro et Prénom" << "Nom" << "Trimestre 1" << "Trimestre 2" << "Trimestre 3" << "Moyenne Générale" << "Rang";
}

QVariant FinalAVGTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal)
    {
        if (role == Qt::DisplayRole)
        {
            return headers.at(section);
        }
    }

    return QVariant();
}

int FinalAVGTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return students.size();
}

int FinalAVGTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return headers.length();
}

QVariant FinalAVGTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    int col = index.column();
    Student student = students.at(row);
    TrimesterAVG trim_1 = trimAVGFor(student.id(), trim1AVGs);
    TrimesterAVG trim_2 = trimAVGFor(student.id(), trim2AVGs);
    TrimesterAVG trim_3 = trimAVGFor(student.id(), trim3AVGs);
    FinalAVG finalAVG = finalAVGFor(student.id(), finalAVGs);
    switch (role) {
    case Qt::DisplayRole:{
        switch (col) {
        case 0:
            return student.number();
            break;
        case 1:
            return student.name();
            break;
        case 2:
            return trim_1.avg;
            break;
        case 3:
            return trim_2.avg;
            break;
        case 4:
            return trim_3.avg;
            break;
        case 5:
            return finalAVG.avg();
            break;
        case 6:
            return finalAVG.rank();
            break;
        default:
            break;
        }
        break;
    }
    default:
        break;
    }


    return QVariant();
}

void FinalAVGTableModel::loadData(int klassID)
{
    currentKlass = klassID;
    beginResetModel();
    students.clear();
    trim1AVGs.clear();
    trim2AVGs.clear();
    trim3AVGs.clear();
    finalAVGs.clear();
    students = DatabaseAccess::instance()->loadStudentsByClass(klassID);
    trim1AVGs = DatabaseAccess::instance()->getTrimesterAVGs(1, klassID);
    trim2AVGs = DatabaseAccess::instance()->getTrimesterAVGs(2, klassID);
    trim3AVGs = DatabaseAccess::instance()->getTrimesterAVGs(3, klassID);
    finalAVGs = DatabaseAccess::instance()->getFinalAVGs(klassID);
    Controller::instance()->checkDbError();
    endResetModel();

}

TrimesterAVG FinalAVGTableModel::trimAVGFor(int studenID, const std::vector<TrimesterAVG> &trimAVGs) const
{
    TrimesterAVG trimAVG;
    for (const TrimesterAVG &avg: trimAVGs)
    {
        if (avg.studentid == studenID)
        {
            trimAVG = avg;
            break;
        }
    }

    return trimAVG;
}

FinalAVG FinalAVGTableModel::finalAVGFor(int studentID, const std::vector<FinalAVG> &finalAVGs) const
{
    FinalAVG finalAVG;
    for (const FinalAVG &avg : finalAVGs)
    {
        if (avg.studentId() == studentID)
        {
            finalAVG = avg;
            break;
        }
    }

    return finalAVG;
}

void FinalAVGTableModel::computeFinalAVG()
{
    std::vector<FinalAVG> finals = AVGCalculator::computeFinalAVG(
        trim1AVGs,
        trim2AVGs,
        trim3AVGs,
        students
        );

    for (FinalAVG &final_ : finals)
    {
        FinalAVG previous = finalAVGFor(final_.studentId(), this->finalAVGs);
        if (previous.id() > 0)
        {
            qDebug() << "Update final average";
            //previous.setAvg(final_.avg());
            //previous.setRank(final_.rank());
            final_.setId(previous.id());
            DatabaseAccess::instance()->updateFinalAverage(final_);
        }

        else {
            qDebug() << "Add new final average";
            DatabaseAccess::instance()->addFinalAverage(final_);
        }
    }

    /*beginInsertColumns(QModelIndex(), 5, 6);
    finalAVGs = finals;
    endInsertColumns();*/
    loadData(currentKlass);
    Controller::instance()->checkDbError();
}
