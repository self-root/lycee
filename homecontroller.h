#ifndef HOMECONTROLLER_H
#define HOMECONTROLLER_H

#include <QObject>
#include "schoolyearlistmodel.h"
#include "studentpiemodel.h"

class HomeController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString schoolName READ getSchoolName CONSTANT)
    Q_PROPERTY(int students READ numberOfStudents NOTIFY numberOfStudentChanged)
    Q_PROPERTY(SchoolYEarListModel *schoolYearModel READ yearsModel CONSTANT)
    Q_PROPERTY(int males READ numberOfMale NOTIFY numberOfMaleChanged)
    Q_PROPERTY(int females READ numberOfFemale NOTIFY numberOfFemaleChanged)
    Q_PROPERTY(StudentPieModel *distroByLevel READ getDistroByLevel CONSTANT)
    Q_PROPERTY(QString currentSchoolYear READ getCurrentSchoolYear WRITE setCurrentSchoolYear NOTIFY currentSchoolYearChanged)
public:
    explicit HomeController(QObject *parent = nullptr);

    QString getSchoolName() const;

    int numberOfStudents() const;
    void setNumberOfStudents(int newNumberOfStudents);

    Q_INVOKABLE void init();

    SchoolYEarListModel *yearsModel() const;
    StudentPieModel *getDistroByLevel() const;

    int numberOfMale() const;


    int numberOfFemale() const;
    void setNumberOfFemale(int newNumberOfFemale);

    void setNumberOfMale(int newNumberOfMale);

    QString getCurrentSchoolYear() const;
    void setCurrentSchoolYear(const QString &newCurrentSchoolYear);

public slots:
    void onSchoolYearsChanged();

private:
    QString schoolName;
    int mNumberOfStudents = 0;
    int mNumberOfMale = 0;
    int mNumberOfFemale = 0;
    SchoolYEarListModel *model = nullptr;
    StudentPieModel *levelPieModel = nullptr;
    QString currentSchoolYear;

signals:
    void numberOfStudentChanged();
    void numberOfMaleChanged();
    void numberOfFemaleChanged();
    void currentSchoolYearChanged();
};

#endif // HOMECONTROLLER_H
