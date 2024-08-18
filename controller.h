#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QStringList>
#include <QObject>
#include <QMap>
#include <QThread>

#include "klass.h"
#include "pdfcreator.h"


class Controller : public QObject
{
    Q_OBJECT
protected:
    Controller(QObject *parent=nullptr);

private:
    static Controller *_instance;
    QThread pdfCreationThread;
    PdfCreator *creator = nullptr;

public:
    static Controller *instance();
    QStringList schoolYears;
    std::vector<Klass> klasses;
    void init();
    void checkDbError();
    void getKlassList(const QString &schoolYear);
    Klass klassByName(const QString &className);
    QMap<QString, QString> getSchoolSettings();
    void saveSchoolSettings(const QMap<QString, QString> &setting);
    void createTranscript(int classID, int trimester, const QString &filepath, const QString &schoolyear);
    void getSchoolyears();

private slots:
    void onTrancriptCreated();

signals:
    void databaseError(const QString &errorMessage);
};

#endif // CONTROLLER_H
