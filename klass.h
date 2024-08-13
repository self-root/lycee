#ifndef KLASS_H
#define KLASS_H

#include <QString>

class Klass
{
public:
    Klass();

    int classId() const;
    void setClassId(int newClassId);

    QString className() const;
    void setClassName(const QString &newClassName);

private:
    int mClassId = -1;
    QString mClassName;
};

#endif // KLASS_H
