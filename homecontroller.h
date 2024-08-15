#ifndef HOMECONTROLLER_H
#define HOMECONTROLLER_H

#include <QObject>
#include "schoolyearlistmodel.h"

class HomeController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString schoolName READ getSchoolName CONSTANT)
    Q_PROPERTY(int students READ numberOfStudents CONSTANT)
    Q_PROPERTY(SchoolYEarListModel *schoolYearModel READ yearsModel CONSTANT)
    Q_PROPERTY(int males READ numberOfMale CONSTANT)
    Q_PROPERTY(int females READ numberOfFemale CONSTANT)
public:
    explicit HomeController(QObject *parent = nullptr);

    QString getSchoolName() const;

    int numberOfStudents() const;
    void setNumberOfStudents(int newNumberOfStudents);

    void init();

    SchoolYEarListModel *yearsModel() const;

    int numberOfMale() const;


    int numberOfFemale() const;
    void setNumberOfFemale(int newNumberOfFemale);

    void setNumberOfMale(int newNumberOfMale);

private:
    QString schoolName = "lycée de Sainte Marie";
    int mNumberOfStudents = 0;
    int mNumberOfMale = 0;
    int mNumberOfFemale = 0;
    SchoolYEarListModel *model = nullptr;

signals:
    void numberOfStudentChanged();
    void numberOfMaleChanged();
    void numberOfFemaleChanged();
};

#endif // HOMECONTROLLER_H
