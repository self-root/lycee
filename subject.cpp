#include "subject.h"

Subject::Subject() {}

QString Subject::subjectName() const
{
    return mSubjectName;
}

void Subject::setSubjectName(const QString &newSubjectName)
{
    mSubjectName = newSubjectName;
}

int Subject::subjectId() const
{
    return mSubjectId;
}

void Subject::setSubjectId(int newSubjectId)
{
    mSubjectId = newSubjectId;
}

int Subject::subjectCoef() const
{
    return mSubjectCoef;
}

void Subject::setSubjectCoef(int newSubjectCoef)
{
    mSubjectCoef = newSubjectCoef;
}
