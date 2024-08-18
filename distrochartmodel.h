#ifndef DISTROCHARTMODEL_H
#define DISTROCHARTMODEL_H

#include <QAbstractListModel>
#include <QColor>

class DistroChartModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles{
        LabelRole = Qt::UserRole + 1,
        ValueRole,
        ColorRole
    };
    explicit DistroChartModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    virtual void loadData() = 0;

protected:
    struct PieSlice
    {
        PieSlice(){}
        PieSlice(const QString &l, double v)
        {
            label = l;
            value = v;
        }
        QString label;
        double value;
        QColor color;
    };

    std::vector<PieSlice> series;
    std::vector<QColor> colors;
};

#endif // DISTROCHARTMODEL_H
