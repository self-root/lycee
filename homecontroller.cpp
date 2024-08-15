#include "homecontroller.h"
#include "databaseaccess.h"

HomeController::HomeController(QObject *parent)
    : QObject{parent}
{
    model = new SchoolYEarListModel;
    init();
}

QString HomeController::getSchoolName() const
{
    return schoolName;
}

int HomeController::numberOfStudents() const
{
    return mNumberOfStudents;
}

void HomeController::setNumberOfStudents(int newNumberOfStudents)
{
    mNumberOfStudents = newNumberOfStudents;
    emit numberOfStudentChanged();
}

void HomeController::init()
{
    auto years = DatabaseAccess::instance()->schoolYears();
    QString schoolyear;
    if (!years.empty())
        schoolyear = years.at(0);
    setNumberOfStudents(DatabaseAccess::instance()->getStudentCount(schoolyear));
    setNumberOfMale(DatabaseAccess::instance()->getSexeCount(schoolyear, "Garçon"));
    setNumberOfFemale(DatabaseAccess::instance()->getSexeCount(schoolyear, "Fille"));
}

SchoolYEarListModel *HomeController::yearsModel() const
{
    return model;
}

int HomeController::numberOfMale() const
{
    return mNumberOfMale;
}


int HomeController::numberOfFemale() const
{
    return mNumberOfFemale;
}

void HomeController::setNumberOfFemale(int newNumberOfFemale)
{
    if (mNumberOfFemale == newNumberOfFemale)
        return;
    mNumberOfFemale = newNumberOfFemale;
    emit numberOfFemaleChanged();
}

void HomeController::setNumberOfMale(int newNumberOfMale)
{
    if (mNumberOfMale == newNumberOfMale)
        return;
    mNumberOfMale = newNumberOfMale;
    emit numberOfMaleChanged();
}
