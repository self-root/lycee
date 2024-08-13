#ifndef HOMECONTROLLER_H
#define HOMECONTROLLER_H

#include <QObject>

class HomeController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString schoolName READ getSchoolName CONSTANT)
public:
    explicit HomeController(QObject *parent = nullptr);

    QString getSchoolName() const;

private:
    QString schoolName = "lycée de Sainte Marie";
signals:
};

#endif // HOMECONTROLLER_H
