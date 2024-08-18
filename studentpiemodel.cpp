#include "studentpiemodel.h"
#include "databaseaccess.h"
#include "klass.h"
#include <QDebug>

StudentPieModel::StudentPieModel(QObject *parent)
    : DistroChartModel{parent}
{

}

void StudentPieModel::loadData()
{
    qDebug() << "LOading student count...";
    beginResetModel();
    series.clear();
    std::vector<Klass> klasses = DatabaseAccess::instance()->gestClasses(currentYear);
    int secondCount = getCountFor("sec", klasses);
    int premiereCount = getCountFor("pre", klasses);
    int termCount = getCountFor("ter", klasses);
    series.push_back(PieSlice("Seconde", secondCount));
    series.push_back(PieSlice("Premiere", premiereCount));
    series.push_back(PieSlice("Terminale", termCount));
    setSecondCount(secondCount);
    setPremiereCount(premiereCount);
    setTerminalCount(termCount);

    qDebug() << "Loaded: " << series.size();

    endResetModel();
}

void StudentPieModel::setSchoolYear(const QString &schoolYear)
{
    currentYear = schoolYear;
}

int StudentPieModel::countData()
{
    return series.size();
}

QString StudentPieModel::labelAt(int i)
{
    return series.at(i).label;
}

double StudentPieModel::valueAt(int i)
{
    return series.at(i).value;
}

int StudentPieModel::getCountFor(const QString &klassLevelName, const std::vector<Klass> &klasses)
{
    int total = 0;
    for (const Klass &k : klasses)
    {
        if (k.className().startsWith(klassLevelName, Qt::CaseInsensitive))
        {
            total += DatabaseAccess::instance()->studentCountByClass(k.classId());
        }
    }

    return total;
}

int StudentPieModel::terminalCount() const
{
    return mTerminalCount;
}

void StudentPieModel::setTerminalCount(int newTerminalCount)
{
    if (mTerminalCount == newTerminalCount)
        return;
    mTerminalCount = newTerminalCount;
    emit terminalCountChanged();
}

int StudentPieModel::premiereCount() const
{
    return mPremiereCount;
}

void StudentPieModel::setPremiereCount(int newPremiereCount)
{
    if (mPremiereCount == newPremiereCount)
        return;
    mPremiereCount = newPremiereCount;
    emit premiereCountChanged();
}

int StudentPieModel::secondCount() const
{
    return mSecondCount;
}

void StudentPieModel::setSecondCount(int newSecondCount)
{
    if (mSecondCount == newSecondCount)
        return;
    mSecondCount = newSecondCount;
    emit secondCountChanged();
}
