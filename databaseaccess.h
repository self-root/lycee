#ifndef DATABASEACCESS_H
#define DATABASEACCESS_H

#include <QObject>
#include <vector>
#include "student.h"
#include "klass.h"
#include "subject.h"
#include "studentgrade.h"
#include "trimesteravg.h"
#include "finalavg.h"

#include <QSqlDatabase>

struct GradeImportRecord
{
    int studentID = -1;
    int subjectID = -1;
    int trimesterID = -1;
    double grade = 0.0;
    bool skip = false;
};

class DatabaseAccess : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseAccess(QObject *parent = nullptr);
    explicit DatabaseAccess(const QString &databasePath, QObject *parent = nullptr);
    ~DatabaseAccess() override;

private:
    static DatabaseAccess *_instance;
    bool dbError = false;
    QString errorMessage;
    const QString dbName = "lyceedata";
    void setErrorMessage(const QString &error);
    bool initDB();
    bool configureConnection();
    QSqlDatabase database;
    QString connectionName;

public:
    static DatabaseAccess *instance();
    bool hasDbError();
    QString getErrorMessage();
    bool openDB();

    bool createBackup(QString *backupPath = nullptr);

    QSqlDatabase getDatabase();

    std::vector<Student> loadStudentsByClass(int classID);

    std::vector<Klass> gestClasses(const QString &schoolYear);

    bool updateStudent(const Student &student, int classID);

    bool addStudent(Student &student, int classID);

    bool deleteStudent(const Student &student, int classID);

    bool addStudents(const std::vector<Student> &students, int classID, int *inserted = nullptr);

    std::vector<Subject> getSubjectByClass(int classID);

    QStringList groupedSubjectNames();

    QStringList schoolYears();

    int schoolYearID(const QString &schoolYear);

    bool addSchoolYear(const QString &schoolYear);

    bool removeSchoolYear(const QString &schoolYear);

    bool addClass(const QString &className, int schoolYearID);

    bool removeKlass(const Klass &klass);

    bool addSubject(Subject &subject, int classID);

    void studentGrades(int klassID,
                       int trimester,
                       std::vector<StudentGrade> &grades,
                       std::vector<Student> &students,
                       std::vector<Subject> &subjects);

    static std::size_t indexOF(std::vector<Student> &students, int id);
    static std::size_t indexOF(std::vector<Subject> &subjects, int id);

    bool updateStudentGrade(const GradeMetaData &gradeMeta);

    bool addGrade(GradeMetaData &gradeMeta, int trimesterID, int studentID, int subjectID);

    bool importGrades(const std::vector<GradeImportRecord> &records, int *imported = nullptr);

    void getTrimesterAverages(std::vector<TrimesterAVG> &averages, int classID, int trimesterID);

    bool addTrimesterAVG(TrimesterAVG &trimAVG);

    bool updateTrimAVG(const TrimesterAVG &trimAVG);

    bool updateSubject(const Subject &subject);

    bool removeSubject(const Subject &subject);

    void getGrade(GradeMetaData &grade, int studentID, int subjectID, int trimester);

    void trimAVG(TrimesterAVG &avg);

    bool updateGrade20(int gradeID, double grade20);

    std::vector<FinalAVG> getFinalAVGs(int classID);

    std::vector<TrimesterAVG> getTrimesterAVGs(int trimester, int klassID);

    bool addFinalAverage(FinalAVG &final);

    bool updateFinalAverage(const FinalAVG &final);

    Klass classByID(int klassID);

    int getStudentCount(const QString &schoolYear);

    int getSexeCount(const QString &schoolYear, const QString &sexe);

    int studentCountByClass(int klassID);

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
