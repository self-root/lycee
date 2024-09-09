#ifndef FINALAVGTABLEMODEL_H
#define FINALAVGTABLEMODEL_H

#include <QAbstractTableModel>
#include <vector>

#include "student.h"
#include "trimesteravg.h"
#include "finalavg.h"

class FinalAVGTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit FinalAVGTableModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void loadData(int klassID);

    TrimesterAVG trimAVGFor(int studenID, const std::vector<TrimesterAVG> &trimAVGs) const;
    FinalAVG finalAVGFor(int studentID, const std::vector<FinalAVG> &finalAVGs) const;

    //TODO: compute final avg
    void computeFinalAVG();

private:
    std::vector<Student> students;
    std::vector<TrimesterAVG> trim1AVGs;
    std::vector<TrimesterAVG> trim2AVGs;
    std::vector<TrimesterAVG> trim3AVGs;
    std::vector<FinalAVG> finalAVGs;
    QStringList headers;
    int currentKlass = -1;
};

#endif // FINALAVGTABLEMODEL_H
