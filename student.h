#ifndef STUDENT_H
#define STUDENT_H

#include <QString>

class Student
{
public:
    Student();

    int id() const;
    void setId(int newId);

    int number() const;
    void setNumber(int newNumber);

    QString matricule() const;
    void setMatricule(const QString &newMatricule);

    QString name() const;
    void setName(const QString &newName);

    QString sexe() const;
    void setSexe(const QString &newSexe);

    QString situation() const;
    void setSituation(const QString &newSituation);

    QString birthDay() const;
    void setBirthDay(const QString &newBirthDay);

    double finalAVG() const;
    void setFinalAVG(double newFinalAVG);

    int fInalRank() const;
    void setFinalRank(int newFInalRank);

private:
    int mId = -1;
    int mNumber = -1;
    QString mMatricule;
    QString mName;
    QString mSexe;
    QString mSituation;
    QString mBirthDay;
    double mFinalAVG = 0.0;
    int mFInalRank = - 1;
};

#endif // STUDENT_H
