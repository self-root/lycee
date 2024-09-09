#ifndef SCHOOLYEARLISTMODEL_H
#define SCHOOLYEARLISTMODEL_H

#include <QAbstractListModel>
#include <QHash>

class SchoolYEarListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        SchoolYear = Qt::UserRole + 1,
    };
    explicit SchoolYEarListModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void loadData();

    QHash<int, QByteArray> roleNames() const override;

private:
    QStringList schoolYears;
};

#endif // SCHOOLYEARLISTMODEL_H
