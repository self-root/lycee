#include "controller.h"
#include "databaseaccess.h"
#include <QDir>
#include <QStandardPaths>
#include <QSettings>
#include <QCoreApplication>

Controller *Controller::_instance = 0;
Controller::Controller(QObject *parent) : QObject(parent)
{
    QCoreApplication::setOrganizationName("iroot");
    QCoreApplication::setOrganizationDomain("irootsoftware.com");
    QCoreApplication::setApplicationName("Lycee");
    creator = new PdfCreator;
    creator->moveToThread(&pdfCreationThread);

    QObject::connect(creator, &PdfCreator::pdfCreated, this, &Controller::onTrancriptCreated);
}

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

QMap<QString, QString> Controller::getSchoolSettings()
{
    QMap<QString, QString> values;
    QSettings setting;
    setting.beginGroup("etablissement");
    values["principal"] = setting.value("principal", "Proviseur").toString();
    values["school_name"] = setting.value("school_name", "Lycée").toString();
    values["place"] = setting.value("place", "Lieu").toString();
    values["code"] = setting.value("code", "0000000").toString();
    setting.endGroup();

    return values;
}

void Controller::saveSchoolSettings(const QMap<QString, QString> &setting)
{
    QSettings settings;
    settings.beginGroup("etablissement");
    settings.setValue("principal", setting.value("principal"));
    settings.setValue("school_name", setting.value("school_name"));
    settings.setValue("place", setting.value("place"));
    settings.setValue("code", setting.value("code"));
    settings.endGroup();
}

void Controller::createTranscript(int classID, int trimester, const QString &filepath, const QString &schoolyear)
{
    //QMetaObject::invokeMethod(creator, &PdfCreator::createTranscript, );
    if (trimester == 3)
        creator->createFinalTranscipt(classID, filepath, getSchoolSettings(), schoolyear);
    else
        creator->createTranscript(classID, trimester, filepath, getSchoolSettings(), schoolyear);
}

void Controller::onTrancriptCreated()
{
    qDebug() << "Transcropt created";
}
