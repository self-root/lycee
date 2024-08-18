#ifndef STUDENTPIEMODEL_H
#define STUDENTPIEMODEL_H

#include "distrochartmodel.h"

class Klass;

class StudentPieModel : public DistroChartModel
{
    Q_OBJECT
    Q_PROPERTY(int secondCount READ secondCount WRITE setSecondCount NOTIFY secondCountChanged)
    Q_PROPERTY(int premiereCount READ premiereCount WRITE setPremiereCount NOTIFY premiereCountChanged)
    Q_PROPERTY(int terminalCount READ terminalCount WRITE setTerminalCount NOTIFY terminalCountChanged)
public:
    explicit StudentPieModel(QObject *parent = nullptr);
    Q_INVOKABLE void loadData() override;
    Q_INVOKABLE void setSchoolYear(const QString &schoolYear);
    Q_INVOKABLE int countData();
    Q_INVOKABLE QString labelAt(int i);
    Q_INVOKABLE double valueAt(int i);

    int secondCount() const;
    void setSecondCount(int newSecondCount);


    int premiereCount() const;
    void setPremiereCount(int newPremiereCount);

    int terminalCount() const;
    void setTerminalCount(int newTerminalCount);

private:
    QString currentYear;

    /**
     * @brief getCountFor Count number of students in a same level
     * @param klassLevelName, the level name should be one of [second, premiere, terminale], but this function
     * will check only the beginning of the class name, so we only need to passd sec, pre, or ter for example
     * @return return the total number of students for that section
     */
    int getCountFor(const QString &klassLevelName, const std::vector<Klass> &klasses);
    int mSecondCount = 0;
    int mPremiereCount = 0;
    int mTerminalCount = 0;

signals:
    void secondCountChanged();
    void premiereCountChanged();
    void terminalCountChanged();
};

#endif // STUDENTPIEMODEL_H
