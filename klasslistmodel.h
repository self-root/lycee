#ifndef KLASSLISTMODEL_H
#define KLASSLISTMODEL_H

#include <QAbstractListModel>

#include <vector>

#include "klass.h"


class KlassListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit KlassListModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void loadData(const QString &schoolYear);

    Klass klassAt(int index);

    void addClass(const QString &className);

    void removeKlass(const Klass &klass);

private:
    std::vector<Klass> klasses;
    QString currentSchoolYear;
};

#endif // KLASSLISTMODEL_H
