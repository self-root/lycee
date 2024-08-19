#include "homecontroller.h"
#include "databaseaccess.h"
#include "controller.h"

HomeController::HomeController(QObject *parent)
    : QObject{parent}
{
    model = new SchoolYEarListModel;
    levelPieModel = new StudentPieModel;
    auto schoolInfo = Controller::instance()->getSchoolSettings();
    schoolName = schoolInfo.value("school_name", "Lycee");
    //init();
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
    setNumberOfStudents(DatabaseAccess::instance()->getStudentCount(currentSchoolYear));
    setNumberOfMale(DatabaseAccess::instance()->getSexeCount(currentSchoolYear, "Garçon"));
    setNumberOfFemale(DatabaseAccess::instance()->getSexeCount(currentSchoolYear, "Fille"));
}

SchoolYEarListModel *HomeController::yearsModel() const
{
    return model;
}

StudentPieModel *HomeController::getDistroByLevel() const
{
    return levelPieModel;
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

QString HomeController::getCurrentSchoolYear() const
{
    return currentSchoolYear;
}

void HomeController::setCurrentSchoolYear(const QString &newCurrentSchoolYear)
{
    if (newCurrentSchoolYear == currentSchoolYear)
        return;
    currentSchoolYear = newCurrentSchoolYear;
    setNumberOfStudents(DatabaseAccess::instance()->getStudentCount(currentSchoolYear));
    setNumberOfMale(DatabaseAccess::instance()->getSexeCount(currentSchoolYear, "Garçon"));
    setNumberOfFemale(DatabaseAccess::instance()->getSexeCount(currentSchoolYear, "Fille"));
    emit currentSchoolYearChanged();

}

void HomeController::onSchoolYearsChanged()
{
    model->loadData();
}
