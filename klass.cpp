#include "klass.h"

Klass::Klass() {}

int Klass::classId() const
{
    return mClassId;
}

void Klass::setClassId(int newClassId)
{
    mClassId = newClassId;
}

QString Klass::className() const
{
    return mClassName;
}

void Klass::setClassName(const QString &newClassName)
{
    mClassName = newClassName;
}
