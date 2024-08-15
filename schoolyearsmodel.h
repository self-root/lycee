#ifndef SCHOOLYEARSMODEL_H
#define SCHOOLYEARSMODEL_H

#include <QAbstractListModel>
#include <QByteArray>
#include <QHash>

class SchoolYearsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles{
        SchoolYearRole = Qt::UserRole + 1
    };
    explicit SchoolYearsModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void loadSchoolYears();

    void addScoolYear(const QString &SchoolYear);

    void removeSchoolYear(const QString &schoolYear);

    QHash<int, QByteArray> roleNames() const override;

private:
    QStringList schoolYears;
};

#endif // SCHOOLYEARSMODEL_H
