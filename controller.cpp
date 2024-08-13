#include "controller.h"
#include "databaseaccess.h"

Controller *Controller::_instance = 0;
Controller::Controller(QObject *parent) : QObject(parent){}

Controller *Controller::instance()
{
    if (_instance == 0)
        _instance = new Controller;
    return _instance;
}

void Controller::init()
{
    DatabaseAccess::instance()->openDB();
    if (DatabaseAccess::instance()->hasDbError())
    {
        emit databaseError(DatabaseAccess::instance()->getErrorMessage());
        return;
    }

    schoolYears = DatabaseAccess::instance()->schoolYears();
    checkDbError();

}

void Controller::checkDbError()
{
    if (DatabaseAccess::instance()->hasDbError())
    {
        emit databaseError(DatabaseAccess::instance()->getErrorMessage());
        return;
    }
}

void Controller::getKlassList(const QString &schoolYear)
{
    klasses.clear();
    klasses = DatabaseAccess::instance()->gestClasses(schoolYear);
    checkDbError();
}

Klass Controller::klassByName(const QString &className)
{
    Klass klass;
    for (const Klass &k : klasses)
    {
        if (k.className() == className)
        {
            klass = k;
        }
    }

    return klass;
}
