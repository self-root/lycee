#ifndef SCHOOLYEARSMODEL_H
#define SCHOOLYEARSMODEL_H

#include <QAbstractListModel>

class SchoolYearsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SchoolYearsModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void loadSchoolYears();

    void addScoolYear(const QString &SchoolYear);

    void removeSchoolYear(const QString &schoolYear);

private:
    QStringList schoolYears;
};

#endif // SCHOOLYEARSMODEL_H
