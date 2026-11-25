#include "controller.h"
#include "databaseaccess.h"
#include "pdfcreator.h"
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
    pdfCreationThread.setObjectName("pdf-export");
    creator->moveToThread(&pdfCreationThread);
    pdfCreationThread.start();


    QObject::connect(creator, &PdfCreator::pdfCreated, this, &Controller::onTrancriptCreated);
    QObject::connect(creator, &PdfCreator::totalisationPDFCreated,this, &Controller::onToTalisationPDFCreated);
    QObject::connect(creator, &PdfCreator::finalTotalisationExcelCreated, this, &Controller::onFinalTotalisationPDFCreated);
    QObject::connect(creator, &PdfCreator::ficheDeNoteCreated, this, &Controller::onToTalisationPDFCreated);
    QObject::connect(creator, &PdfCreator::exportFailed, this, &Controller::exportFailed);
    QObject::connect(creator, &PdfCreator::excelCreated, this, &Controller::onToTalisationPDFCreated);

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

    getSchoolyears();
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
    emit exportStarted("Creation du bulletin PDF...");
    if (trimester == 3)
    {
        QMetaObject::invokeMethod(creator,
                                  "createFinalTranscipt",
                                  Qt::QueuedConnection,
                                  Q_ARG(int, classID),
                                  Q_ARG(QString, filepath),
                                  Q_ARG(QString, schoolyear));
    }
    else
    {
        QMetaObject::invokeMethod(creator,
                                  "createTranscript",
                                  Qt::QueuedConnection,
                                  Q_ARG(int, classID),
                                  Q_ARG(int, trimester),
                                  Q_ARG(QString, filepath),
                                  Q_ARG(QString, schoolyear));
    }
}

void Controller::createTotalisationPDF(int classID,
                                       int trimester,
                                       const QString &filepath,
                                       const QString &schoolyear,
                                       Order by,
                                       FilterBy filter)
{
    emit exportStarted("Creation de la totalisation PDF...");
    qRegisterMetaType<Order>();
    qRegisterMetaType<FilterBy>();
    if (trimester == 3)
    {
        /*    void  createFinalTotalisationPDF(int classID,
                                     QString out ,
                                     const QString &schoolYear,
                                     Order order,
                                     FilterBy by);*/
        QMetaObject::invokeMethod(creator,
                                  "createFinalTotalisationPDF",
                                  Qt::QueuedConnection,
                                  Q_ARG(int, classID),
                                  Q_ARG(QString, filepath),
                                  Q_ARG(QString, schoolyear),
                                  Q_ARG(Order, by),
                                  Q_ARG(FilterBy, filter));
    }

    else
    {
        QMetaObject::invokeMethod(creator,
                                  "createTotalisationPDF",
                                  Qt::QueuedConnection,
                                  Q_ARG(int, classID),
                                  Q_ARG(int, trimester),
                                  Q_ARG(QString, filepath),
                                  Q_ARG(QString, schoolyear),
                                  Q_ARG(Order, by),
                                  Q_ARG(FilterBy, filter));
    }
}

void Controller::createTotalisationExcel(int classID,
                                         int trimester,
                                         const QString &filepath,
                                         Order order,
                                         FilterBy by)
{
    emit exportStarted("Creation de la totalisation Excel...");
    qRegisterMetaType<Order>();
    qRegisterMetaType<FilterBy>();
    QMetaObject::invokeMethod(creator,
                              "createTotalisationExcel",
                              Qt::QueuedConnection,
                              Q_ARG(int, classID),
                              Q_ARG(int, trimester),
                              Q_ARG(QString, filepath),
                              Q_ARG(Order, order),
                              Q_ARG(FilterBy, by));
}

void Controller::createFicheDeNote(int klassID, const QString &schoolYear, int trimester, const QString &filepath)
{
    emit exportStarted("Creation de la fiche de notes PDF...");
    QMetaObject::invokeMethod(creator,
                              "createFicheDeNote",
                              Qt::QueuedConnection,
                              Q_ARG(int, klassID),
                              Q_ARG(QString, filepath),
                              Q_ARG(QString, schoolYear),
                              Q_ARG(int, trimester));
}

void Controller::getSchoolyears()
{
    schoolYears = DatabaseAccess::instance()->schoolYears();
}

QMap<QString, QString> Controller::getTranscriptFormatSettings()
{
    QSettings settings;
    QMap<QString, QString> values;
    settings.beginGroup("transcript_format");
    values["font"] = settings.value("font", "Arial").toString();
    values["fontsize"] = settings.value("fontsize", "10").toString();
    values["paddings"] = settings.value("paddings", "1").toString();
    settings.endGroup();
    return values;
}

QMap<QString, QString> Controller::getTotalizationFormatSettings()
{
    QSettings settings;
    QMap<QString, QString> values;
    settings.beginGroup("totalization_format");
    values["font"] = settings.value("font", "Arial").toString();
    values["fontsize"] = settings.value("fontsize", "10").toString();
    values["paddings"] = settings.value("paddings", "1").toString();
    settings.endGroup();
    return values;
}

void Controller::saveTranscriptFormatSettings(const QMap<QString, QString> &values)
{
    QSettings settings;
    settings.beginGroup("transcript_format");
    settings.setValue("font", values.value("font", "Arial"));
    settings.setValue("fontsize", values.value("fontsize", "10"));
    settings.setValue("paddings", values.value("paddings", "1"));
    settings.endGroup();
}

void Controller::saveTotalizationFormatSettings(const QMap<QString, QString> &values)
{
    QSettings settings;
    settings.beginGroup("totalization_format");
    settings.setValue("font", values.value("font", "Arial"));
    settings.setValue("fontsize", values.value("fontsize", "10"));
    settings.setValue("paddings", values.value("paddings", "1"));
    settings.endGroup();
}

void Controller::onTrancriptCreated(const QString &filePath)
{
    qDebug() << "Transcript created";
    emit exportSucceeded(filePath);
}

void Controller::onToTalisationPDFCreated(const QString &filePath)
{
    qDebug() << "Totalisation saved at: " << filePath;
    emit exportSucceeded(filePath);
}

void Controller::onFinalTotalisationPDFCreated(const QString &filepath)
{
    qDebug() << "Final totalisation PDF created at: " << filepath;
    emit exportSucceeded(filepath);
}
