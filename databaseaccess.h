#ifndef DATABASEACCESS_H
#define DATABASEACCESS_H

#include <QObject>
#include <vector>
#include "student.h"
#include "klass.h"
#include "subject.h"
#include "studentgrade.h"
#include "trimesteravg.h"

#include <QSqlDatabase>

class DatabaseAccess : public QObject
{
    Q_OBJECT
protected:
    explicit DatabaseAccess(QObject *parent = nullptr);

private:
    static DatabaseAccess *_instance;
    bool dbError = false;
    QString errorMessage;
    const QString dbName = "lyceedata";
    void setErrorMessage(const QString &error);
    void initDB(const QSqlDatabase &db);

public:
    static DatabaseAccess *instance();
    bool hasDbError();
    QString getErrorMessage();
    void openDB();

    std::vector<Student> loadStudentsByClass(int classID);

    std::vector<Klass> gestClasses(const QString &schoolYear);

    void updateStudent(const Student &student, int classID);

    void addStudent(Student &student, int classID);

    void deleteStudent(const Student &student, int classID);

    std::vector<Subject> getSubjectByClass(int classID);

    QStringList groupedSubjectNames();

    QStringList schoolYears();

    int schoolYearID(const QString &schoolYear);

    void addSchoolYear(const QString &schoolYear);

    void removeSchoolYear(const QString &schoolYear);

    void addClass(const QString &className, int schoolYearID);

    void addSubject(Subject &subject, int classID);

    void studentGrades(int klassID,
                       int trimester,
                       std::vector<StudentGrade> &grades,
                       std::vector<Student> &students,
                       std::vector<Subject> &subjects);

    static std::size_t indexOF(std::vector<Student> &students, int id);
    static std::size_t indexOF(std::vector<Subject> &subjects, int id);

    void updateStudentGrade(const GradeMetaData &gradeMeta);

    void addGrade(GradeMetaData &gradeMeta, int trimesterID, int studentID, int subjectID);

    void getTrimesterAverages(std::vector<TrimesterAVG> &averages, int classID, int trimesterID);

    void addTrimesterAVG(TrimesterAVG &trimAVG);

    void updateTrimAVG(const TrimesterAVG &trimAVG);

    void updateSubject(const Subject &subject);

    void removeSubject(const Subject &subject);

};

#endif // DATABASEACCESS_H

/*
 * select name, subjectName, grade, grade20, f_trimester, f_student from grade
INNER JOIN student on student.id = f_student
INNER JOIN subject on subject.id = f_subject
INNER JOIN trimester on trimester.id = f_trimester;

select name, subjectName, grade, grade.id as grade_id ,grade20, f_trimester, f_student from grade
INNER JOIN student on student.id = f_student
INNER JOIN subject on subject.id = f_subject
INNER JOIN trimester on trimester.id = f_trimester;

select name, subjectName, grade, grade.id as grade_id ,grade20, f_trimester, f_student, klassName from grade
INNER JOIN student on student.id = f_student
INNER JOIN subject on subject.id = f_subject
INNER JOIN trimester on trimester.id = f_trimester
INNER JOIN klass on klass.id = student.f_klass
WHERE klass.id = 3;

SELECT student.id, name, subjectName, subject.id AS subjectID ,grade, grade.id AS grade_id, skip ,grade20, f_trimester, f_student, klassName FROM grade
INNER JOIN student on student.id = f_student
INNER JOIN subject on subject.id = f_subject
INNER JOIN trimester on trimester.id = f_trimester
INNER JOIN klass on klass.id = student.f_klass
WHERE klass.id = 3

SELECT student.id, name, subjectName, subject.id AS subjectID ,grade,
grade.id AS grade_id, skip ,grade20, f_trimester, grade.f_student, klassName
FROM grade
INNER JOIN student on student.id = grade.f_student
INNER JOIN subject on subject.id = f_subject
INNER JOIN trimester on trimester.id = f_trimester
INNER JOIN klass on klass.id = student.f_klass
WHERE klass.id = 3 AND f_trimester  = 1;
*/
