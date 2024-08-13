#ifndef SUBJECT_H
#define SUBJECT_H

#include <QString>

class Subject
{
public:
    Subject();

    QString subjectName() const;
    void setSubjectName(const QString &newSubjectName);

    int subjectId() const;
    void setSubjectId(int newSubjectId);

    int subjectCoef() const;
    void setSubjectCoef(int newSubjectCoef);

private:
    int mSubjectId = -1;
    QString mSubjectName;
    int mSubjectCoef = 0;
};

#endif // SUBJECT_H
