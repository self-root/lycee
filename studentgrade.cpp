#include "studentgrade.h"

StudentGrade::StudentGrade(Student &student, Subject &subject, int trimester)
    : mStudent(student), mSubject(subject), mTrimester(trimester)
{

}

bool StudentGrade::skip() const
{
    return mSkip;
}

void StudentGrade::setSkip(bool newSkip)
{
    mSkip = newSkip;
}

double StudentGrade::grade20() const
{
    return mGrade20;
}

void StudentGrade::setGrade20(double newGrade20)
{
    mGrade20 = newGrade20;
}

int StudentGrade::getGradeID() const
{
    return gradeID;
}

void StudentGrade::setGradeID(int newGradeID)
{
    gradeID = newGradeID;
}

double StudentGrade::grade() const
{
    return mGrade;
}

void StudentGrade::setGrade(double newGrade)
{
    mGrade = newGrade;
}

int StudentGrade::coef() const
{
    return mCoef;
}

void StudentGrade::setCoef(int newCoef)
{
    mCoef = newCoef;
}
