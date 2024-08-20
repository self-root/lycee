#ifndef UTILS_H
#define UTILS_H

#include <QMetaType>

enum Order{
    Ask,
    Desc
};
Q_DECLARE_METATYPE(Order);
enum FilterBy{
    Number,
    Rank
};
Q_DECLARE_METATYPE(FilterBy);
enum Target{
    PDF,
    Excel
};
Q_DECLARE_METATYPE(Target);

#endif // UTILS_H
