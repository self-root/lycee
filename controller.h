#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QStringList>
#include <QObject>
#include "klass.h"

class Controller : public QObject
{
    Q_OBJECT
protected:
    Controller(QObject *parent=nullptr);

private:
    static Controller *_instance;

public:
    static Controller *instance();
    QStringList schoolYears;
    std::vector<Klass> klasses;
    void init();
    void checkDbError();
    void getKlassList(const QString &schoolYear);
    Klass klassByName(const QString &className);

signals:
    void databaseError(const QString &errorMessage);

};

#endif // CONTROLLER_H
