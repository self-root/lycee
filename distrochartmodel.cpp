#include "distrochartmodel.h"

DistroChartModel::DistroChartModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int DistroChartModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return series.size();
}

QVariant DistroChartModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const PieSlice &slice = series.at(index.row());

    switch (role) {
    case Roles::LabelRole:
        return slice.label;
        break;
    case Roles::ValueRole:
        return slice.value;
        break;
    case Roles::ColorRole:
        return slice.color;
        break;
    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> DistroChartModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Roles::LabelRole] = "label";
    roles[Roles::ValueRole] = "value";
    roles[Roles::ColorRole] = "color";

    return roles;
}
