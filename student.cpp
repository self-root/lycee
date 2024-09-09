#include "student.h"

Student::Student() {}

int Student::id() const
{
    return mId;
}

void Student::setId(int newId)
{
    mId = newId;
}

int Student::number() const
{
    return mNumber;
}

void Student::setNumber(int newNumber)
{
    mNumber = newNumber;
}

QString Student::matricule() const
{
    return mMatricule;
}

void Student::setMatricule(const QString &newMatricule)
{
    mMatricule = newMatricule;
}

QString Student::name() const
{
    return mName;
}

void Student::setName(const QString &newName)
{
    mName = newName;
}

QString Student::sexe() const
{
    return mSexe;
}

void Student::setSexe(const QString &newSexe)
{
    mSexe = newSexe;
}

QString Student::situation() const
{
    return mSituation;
}

void Student::setSituation(const QString &newSituation)
{
    mSituation = newSituation;
}

QString Student::birthDay() const
{
    return mBirthDay;
}

void Student::setBirthDay(const QString &newBirthDay)
{
    mBirthDay = newBirthDay;
}

double Student::finalAVG() const
{
    return mFinalAVG;
}

void Student::setFinalAVG(double newFinalAVG)
{
    mFinalAVG = newFinalAVG;
}

int Student::fInalRank() const
{
    return mFInalRank;
}

void Student::setFinalRank(int newFInalRank)
{
    mFInalRank = newFInalRank;
}
