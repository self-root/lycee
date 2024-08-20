#ifndef RWEXCEL_H
#define RWEXCEL_H

#include <QObject>
#include <vector>
#include <QStringList>
#include "utils.h"

namespace xlnt {
    class worksheet;
}
class RWExcel : public QObject
{
    Q_OBJECT
public:
    explicit RWExcel(QObject *parent = nullptr);
    static void createTotalisation(int classID,
                                    int trimester,
                                    QString out,
                                    Order by,
                                    FilterBy filter);
    static void createFinalTotalisation(int classID, const QString out, Order order, FilterBy filter);
    template <typename T>
    static void insertColumn(const std::vector<T> &column, int colIndex, xlnt::worksheet &sheet);

    static void addHeader(const QStringList &header, xlnt::worksheet &sheet);

signals:

};

#endif // RWEXCEL_H
