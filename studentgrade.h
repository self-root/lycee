#ifndef STUDENTGRADE_H
#define STUDENTGRADE_H

#include "student.h"
#include "subject.h"

class GradeMetaData
{
public:
    int id = -1;
    QString subjectName;
    double grade = -1;
    double grade20 = -1;
    bool skip = false;
    int coef = 0;

    bool operator==(const GradeMetaData &other) const
    {
        return (id == other.id) &&
               (subjectName == other.subjectName) &&
               (grade == other.grade) &&
               (grade20 == other.grade20) &&
               (skip == other.skip);
    }

    bool operator!=(const GradeMetaData &other) const
    {
        return !(*this == other);
    }
};

class StudentGrade
{
public:
    StudentGrade(Student &student, Subject &subject, int trimester);

    bool skip() const;
    void setSkip(bool newSkip);

    Student &mStudent;
    Subject &mSubject;

    double grade20() const;
    void setGrade20(double newGrade20);

    int getGradeID() const;
    void setGradeID(int newGradeID);

    double grade() const;
    void setGrade(double newGrade);

    int coef() const;
    void setCoef(int newCoef);

private:
    int gradeID = -1;
    double mGrade = 0.0;
    double mGrade20 = 0.0;
    bool mSkip = false;
    int mTrimester;
    int mCoef;
};

#endif // STUDENTGRADE_H
