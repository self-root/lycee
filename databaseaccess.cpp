#include "databaseaccess.h"
#include "qthread.h"
#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QUrl>
#include <QStandardPaths>
#include <QDir>

DatabaseAccess *DatabaseAccess::_instance = 0;
DatabaseAccess::DatabaseAccess(QObject *parent)
    : QObject{parent}
{
    QString connectionName = QThread::currentThread()->objectName();
    QString dbLocation = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QDir dir;
    dir.mkpath(dbLocation);
    dbLocation = QDir::cleanPath(dbLocation + QDir::separator() + dbName);
    qDebug() << "DB location: " << dbLocation;
    if (!QSqlDatabase::contains(connectionName))
        database = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    else
        database = QSqlDatabase::database(connectionName);
    database.setDatabaseName(dbLocation);
    database.open();

    if (!database.isOpen())
    {
        setErrorMessage("Error occuredd while opening database lyceedata\n" + database.lastError().text());
        qDebug() << errorMessage;
        return;

    }

    initDB(database);
}

void DatabaseAccess::setErrorMessage(const QString &error)
{
    dbError = true;
    errorMessage = error;
}

void DatabaseAccess::initDB(const QSqlDatabase &db)
{

    const QString SCHOOLYEAR = R"(
        CREATE TABLE IF NOT EXISTS "school_year" (
            "id"	INTEGER,
            "schoolYear"	TEXT UNIQUE,
            PRIMARY KEY("id" AUTOINCREMENT)
        )
    )";

    const QString KLASS = R"(
        CREATE TABLE IF NOT EXISTS "klass" (
            "id"	INTEGER,
            "f_schoolYear"	INTEGER,
            "klassName"	TEXT NOT NULL,
            PRIMARY KEY("id" AUTOINCREMENT),
            FOREIGN KEY("f_schoolYear") REFERENCES "school_year"("id") ON DELETE CASCADE
        )
    )";

    const QString STUDENT = R"(
        CREATE TABLE IF NOT EXISTS "student" (
            "id"	INTEGER,
            "name"	TEXT NOT NULL,
            "matricule"	TEXT,
            "dateNaiss"	TEXT,
            "situation"	TEXT NOT NULL,
            "sexe"	TEXT NOT NULL,
            "f_klass"	INTEGER,
            "finalRank"	INTEGER,
            "finalAvg"	REAL,
            "studentNumber"	INTEGER,
            FOREIGN KEY("f_klass") REFERENCES "klass"("id") ON DELETE CASCADE,
            PRIMARY KEY("id" AUTOINCREMENT)
        )
    )";

    const QString SUBJECT = R"(
        CREATE TABLE IF NOT EXISTS "subject" (
            "id"	INTEGER,
            "subjectName"	TEXT,
            "subjectCoef"	INTEGER,
            "f_klass"	INTEGER,
            FOREIGN KEY("f_klass") REFERENCES "klass"("id") ON DELETE CASCADE,
            PRIMARY KEY("id" AUTOINCREMENT)
        )
    )";

    const QString GRADE = R"(
        CREATE TABLE IF NOT EXISTS "grade" (
            "id"	INTEGER,
            "grade"	REAL,
            "grade20"	REAL,
            "f_subject"	INTEGER,
            "f_student"	INTEGER,
            "f_trimester"	INTEGER,
            "skip"	INTEGER DEFAULT 0,
            FOREIGN KEY("f_trimester") REFERENCES "trimester"("id") ON DELETE CASCADE,
            FOREIGN KEY("f_subject") REFERENCES "subject"("id") ON DELETE CASCADE,
            FOREIGN KEY("f_student") REFERENCES "student"("id") ON DELETE CASCADE,
            PRIMARY KEY("id" AUTOINCREMENT)
        )
    )";

    const QString FINAL_AVG = R"(
        CREATE TABLE IF NOT EXISTS "final_avg" (
            "avg_id"	INTEGER,
            "avg"	INTEGER NOT NULL,
            "rank"	INTEGER,
            "f_student"	INTEGER,
            PRIMARY KEY("avg_id" AUTOINCREMENT),
            FOREIGN KEY("f_student") REFERENCES "student"("id") ON DELETE CASCADE
        )
    )";

    const QString TRIMESTER = R"(
        CREATE TABLE IF NOT EXISTS "trimester" (
            "id"	INTEGER,
            "trimesterName"	TEXT,
            PRIMARY KEY("id")
        )
    )";

    const QString TRIMAVG = R"(
        CREATE TABLE IF NOT EXISTS "trimavg" (
            "id"	INTEGER,
            "avg"	REAL,
            "rank"	INTEGER,
            "f_trimester"	INTEGER,
            "f_student"	INTEGER,
            "total"	REAL,
            PRIMARY KEY("id" AUTOINCREMENT),
            FOREIGN KEY("f_student") REFERENCES "student"("id") ON DELETE CASCADE,
            FOREIGN KEY("f_trimester") REFERENCES "trimester"("id") ON DELETE CASCADE
        )
    )";

    if (db.tables().contains("trimester"))
        return;

    QStringList queryList;
    queryList << SCHOOLYEAR << KLASS << STUDENT << SUBJECT << GRADE << FINAL_AVG << TRIMESTER << TRIMAVG;

    QSqlQuery query;
    for (const QString &q : queryList)
    {
        if (!query.exec(q))
        {
            setErrorMessage(QString("Database INIT error: %1").arg(query.lastError().text()));
            qDebug() << errorMessage;
            return;
        }
    }

    //TODO: ADD trimesters into database
    QString stringQuery = R"(
        INSERT INTO "trimester" ("id","trimesterName")
        VALUES (1,'Trimestre 1'),
         (2,'Trimestre 2'),
         (3,'Trimestre 3')
    )";

    if (!query.exec(stringQuery))
    {
        setErrorMessage(QString("Trimester creation error: %1").arg(query.lastError().text()));
        qDebug() << errorMessage;
    }

}

DatabaseAccess *DatabaseAccess::instance()
{
    if (_instance == 0)
        _instance = new DatabaseAccess;
    return _instance;
}

bool DatabaseAccess::hasDbError()
{
    return dbError;
}

QString DatabaseAccess::getErrorMessage()
{
    dbError = false;
    return errorMessage;
}

void DatabaseAccess::openDB()
{
    QString connectionName = QThread::currentThread()->objectName();
    QString dbLocation = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QDir dir;
    dir.mkpath(dbLocation);
    dbLocation = QDir::cleanPath(dbLocation + QDir::separator() + dbName);
    qDebug() << "DB location: " << dbLocation;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbLocation);
    db.open();

    if (!db.isOpen())
    {
        setErrorMessage("Error occuredd while opening database lyceedata\n" + db.lastError().text());
        qDebug() << errorMessage;
        return;

    }

    initDB(db);
}

QSqlDatabase DatabaseAccess::getDatabase()
{
    QString connectionName = QThread::currentThread()->objectName();
    if (!QSqlDatabase::contains(connectionName))
    {
        QString dbLocation = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
        QDir dir;
        dir.mkpath(dbLocation);
        dbLocation = QDir::cleanPath(dbLocation + QDir::separator() + dbName);
        qDebug() << "DB location: " << dbLocation;
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        if (!db.open())
        {
            setErrorMessage("Error occuredd while opening database lyceedata\n" + db.lastError().text());
            qDebug() << errorMessage;
        }
    }

   return QSqlDatabase::database(connectionName);
}

std::vector<Student> DatabaseAccess::loadStudentsByClass(int classID)
{
    std::vector<Student> students;
    QSqlQuery query(database);
    query.prepare("SELECT student.id, name, matricule, dateNaiss, situation, sexe, studentNumber FROM student INNER JOIN klass ON klass.id = student.f_klass WHERE klass.id = :classId");
    query.bindValue(":classId", classID);

    if (query.exec())
    {
        while (query.next()) {
            Student student;
            student.setId(query.value("id").toInt());
            student.setName(query.value("name").toString());
            student.setMatricule(query.value("matricule").toString());
            student.setBirthDay(query.value("dateNaiss").toString());
            student.setSituation(query.value("situation").toString());
            student.setSexe(query.value("sexe").toString());
            student.setNumber(query.value("studentNumber").toInt());

            students.push_back(student);
        }
    }

    else{
        setErrorMessage(QString("Error while geting Student list from the database by class ID: %1\n%2")
                            .arg(classID).arg(query.lastError().text()));
    }

    return students;
}


std::vector<Klass> DatabaseAccess::gestClasses(const QString &schoolYear)
{
    std::vector<Klass> klasses;
    QSqlQuery query;
    query.prepare("SELECT * FROM klass INNER JOIN school_year ON school_year.id = klass.f_schoolYear WHERE school_year.schoolYear = :schoolYear");
    query.bindValue(":schoolYear", schoolYear);
    if (query.exec())
    {
        while (query.next())
        {
            Klass klass;
            klass.setClassId(query.value("id").toInt());
            klass.setClassName(query.value("klassName").toString());
            klasses.push_back(klass);
        }
    }
    else
        setErrorMessage("Error while geting classes list from the database. School year: " + schoolYear + "\n" + query.lastError().text());
    return klasses;
}

void DatabaseAccess::updateStudent(const Student &student, int classID)
{
    qDebug() << "Updateid: " << student.id() << " classId: " << classID;
    QSqlQuery query;
    query.prepare("UPDATE student SET name = :name, matricule = :matricule, dateNaiss = :dateNaiss, situation = :situation, sexe = :sexe, studentNumber = :studentNumber, finalRank = :finalRank, finalAvg = :finalAvg WHERE id = :id AND f_klass = :classId");
    query.bindValue(":name", student.name());
    query.bindValue(":matricule", student.matricule());
    query.bindValue(":dateNaiss", student.birthDay());
    query.bindValue(":situation", student.situation());
    query.bindValue(":sexe", student.sexe());
    query.bindValue(":studentNumber", student.number());
    query.bindValue(":finalRank", student.fInalRank());
    query.bindValue(":finalAvg", student.finalAVG());
    query.bindValue(":classId", classID);
    query.bindValue(":id", student.id());

    if (!query.exec())
        setErrorMessage(QString("Error while Updating Student in the database. Student name: %1\n%2")
                            .arg(student.name()).arg(query.lastError().text()));


}

void DatabaseAccess::addStudent(Student &student, int classID)
{
    QSqlQuery query;
    query.prepare("INSERT INTO student(name, matricule, dateNaiss, situation, sexe, studentNumber, f_klass) VALUES (:name, :matricule, :dateNaiss, :situation, :sexe, :studentNumber, :f_klass)");
    query.bindValue(":name", student.name());
    query.bindValue(":matricule", student.matricule());
    query.bindValue(":dateNaiss", student.birthDay());
    query.bindValue(":situation", student.situation());
    query.bindValue(":sexe", student.sexe());
    query.bindValue(":studentNumber", student.number());
    query.bindValue(":f_klass", classID);


    if (query.exec())
        student.setId(query.lastInsertId().toInt());
    else
        setErrorMessage(QString("Error while Adding new Student to the database. Student name: %1\n%2")
                            .arg(student.name()).arg(query.lastError().text()));
}

void DatabaseAccess::deleteStudent(const Student &student, int classID)
{
    QSqlQuery query;
    query.prepare("DELETE FROM student WHERE id = :id AND f_klass = :classID");
    query.bindValue(":id", student.id());
    query.bindValue(":classID", classID);

    if (!query.exec())
        setErrorMessage(QString("Error while Deleting Student from the database. Student name: %1\n%2")
                            .arg(student.name()).arg(query.lastError().text()));
}

std::vector<Subject> DatabaseAccess::getSubjectByClass(int classID)
{
    std::vector<Subject> subjects;
    QSqlQuery query(database);
    query.prepare(R"(SELECT subject.id, subjectName, subjectCoef
                     FROM subject
                     INNER JOIN klass ON f_klass = klass.id
                     WHERE f_klass = :klassID
                     ORDER BY subjectName ASC)");
    query.bindValue(":klassID", classID);

    if (query.exec())
    {
        Subject subject;
        while (query.next())
        {
            subject.setSubjectName(query.value("subjectName").toString());
            subject.setSubjectId(query.value("id").toInt());
            subject.setSubjectCoef(query.value("subjectCoef").toInt());
            subjects.push_back(subject);
        }
    }

    return subjects;
}

QStringList DatabaseAccess::groupedSubjectNames()
{
    QSqlQuery query("SELECT subjectName FROM subject GROUP BY (subjectName)");
    QStringList subjectNames;

    if (query.exec())
    {
        while (query.next())
        {
            subjectNames << query.value("subjectName").toString();
        }
    }

    else
    {
        setErrorMessage(QString("Error while getting subject list from the database.\n%2")
                            .arg(query.lastError().text()));
    }

    return subjectNames;
}


QStringList DatabaseAccess::schoolYears()
{
    QStringList schoolY;
    QSqlQuery query("SELECT * FROM school_year ORDER BY id DESC");
    if (query.exec())
    {
        while (query.next()) {
            schoolY << query.value("schoolYear").toString();
        }
    }
    else
    {
        setErrorMessage("Error occured while getting schoolyear list from database\n" + query.lastError().text());
        qDebug() << errorMessage;
    }
    return schoolY;
}

int DatabaseAccess::schoolYearID(const QString &schoolYear)
{
    QSqlQuery query;
    query.prepare("SELECT id FROM school_year WHERE schoolYear = :year");
    query.bindValue(":year", schoolYear);

    if (query.exec())
    {
        if (query.next())
            return query.value("id").toInt();
    }
    else {
        setErrorMessage(QString("Error occured while getting schoolyear %1 id from the database\n%2")
                            .arg(schoolYear).arg(query.lastError().text()));
        qDebug() << errorMessage;
    }

    return -1;
}

void DatabaseAccess::addSchoolYear(const QString &schoolYear)
{
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO school_year(schoolYear)
        VALUES (:schoolYear)
    )");

    query.bindValue(":schoolYear", schoolYear);

    if (!query.exec())
    {
        setErrorMessage(QString("Error adding schoolyear %1 in the database\n%2")
                            .arg(schoolYear).arg(query.lastError().text()));
        qDebug() << errorMessage;
    }
}

void DatabaseAccess::removeSchoolYear(const QString &schoolYear)
{
    QSqlQuery query;
    query.prepare("DELETE from school_year WHERE schoolYear = :schoolYear");
    query.bindValue(":schoolYear", schoolYear);

    if (!query.exec())
    {
        setErrorMessage(QString("Error removing schoolyear %1 from the database\n%2")
                            .arg(schoolYear).arg(query.lastError().text()));
        qDebug() << errorMessage;
    }
}

void DatabaseAccess::addClass(const QString &className, int schoolYearID)
{
    QSqlQuery query;
    query.prepare(R"(INSERT INTO klass(f_schoolYear, klassName)
                    VALUES (:schoolYear, :klassName))");
    query.bindValue(":schoolYear", schoolYearID);
    query.bindValue(":klassName", className);

    if (!query.exec())
        setErrorMessage(QString("Error while Adding new Class to the database. Class name: %1\n%2")
                            .arg(className).arg(query.lastError().text()));
}

void DatabaseAccess::removeKlass(const Klass &klass)
{
    QSqlQuery query(database);
    query.prepare(R"(
        DELETE FROM klass
        WHERE klass.id = :klassID
    )");

    query.bindValue(":klassID", klass.classId());
    if (!query.exec())
    {
        setErrorMessage(QString("Classe %1 delete error: %2").arg(klass.className()).arg(query.lastError().text()));
    }
}

void DatabaseAccess::addSubject(Subject &subject, int classID)
{
    QSqlQuery query;
    query.prepare(R"(INSERT INTO subject(subjectName, subjectCoef, f_klass)
                    VALUES (:subjectName, :subjectCoef, :f_klass))");

    query.bindValue(":subjectName", subject.subjectName());
    query.bindValue(":subjectCoef", subject.subjectCoef());
    query.bindValue(":f_klass", classID);

    if (query.exec())
    {
        int subjectID = query.lastInsertId().toInt();
        subject.setSubjectId(subjectID);
    }

    else
    {
        setErrorMessage(QString("Error while Adding new Subject to the database. Subject name: %1\n%2")
                            .arg(subject.subjectName()).arg(query.lastError().text()));
    }
}

void DatabaseAccess::studentGrades(int klassID,
                                   int trimester,
                                   std::vector<StudentGrade> &grades,
                                   std::vector<Student> &students,
                                   std::vector<Subject> &subjects)
{
    QSqlQuery query(database);
    query.prepare(R"(
        SELECT student.id, name, subjectName, subject.id AS subjectID ,grade,
        grade.id AS grade_id, skip ,grade20, f_trimester, grade.f_student, klassName, subjectCoef
        FROM grade
        INNER JOIN student on student.id = grade.f_student
        INNER JOIN subject on subject.id = f_subject
        INNER JOIN trimester on trimester.id = f_trimester
        INNER JOIN klass on klass.id = student.f_klass
        WHERE klass.id = :klassID AND f_trimester  = :trimester
    )");

    query.bindValue(":klassID", klassID);
    query.bindValue(":trimester", trimester);

    if (!query.exec())
    {
        setErrorMessage(QString("Error while reading students grade from the database. KlassID: %1, Trimester: %2\n%3")
                            .arg(klassID).arg(trimester).arg(query.lastError().text()));
        return;
    }

    while (query.next())
    {
        int studentID = query.value("id").toInt();
        int subjectID = query.value("subjectID").toInt();

        auto studentIndex = DatabaseAccess::indexOF(students, studentID);
        auto subjectIndex = DatabaseAccess::indexOF(subjects, subjectID);

        try {
            StudentGrade studentGrade(students.at(studentIndex), subjects.at(subjectIndex), trimester);
            studentGrade.setGradeID(query.value("grade_id").toInt());
            studentGrade.setGrade(query.value("grade").toDouble());
            studentGrade.setGrade20(query.value("grade20").toDouble());
            studentGrade.setSkip(query.value("skip").toBool());
            studentGrade.setCoef(query.value("subjectCoef").toInt());
            grades.push_back(studentGrade);
        } catch (std::exception &e)
        {
            setErrorMessage(QString("Exception was raised while reading/seting studentgrade: %1").arg(e.what()));
        }

    }
}

std::size_t DatabaseAccess::indexOF(std::vector<Student> &students, int id)
{
    std::size_t index = -1;

    for (std::size_t i = 0; i < students.size(); i++)
    {
        if (students.at(i).id() == id)
        {
            index = i;
            break;
        }
    }
    return index;
}

std::size_t DatabaseAccess::indexOF(std::vector<Subject> &subjects, int id)
{
    std::size_t index = -1;

    for (std::size_t i = 0; i < subjects.size(); i++)
    {
        if (subjects.at(i).subjectId() == id)
        {
            index = i;
            break;
        }
    }


    return index;
}

void DatabaseAccess::updateStudentGrade(const GradeMetaData &gradeMeta)
{
    QSqlQuery query;
    query.prepare(R"(
        UPDATE grade
        SET grade = :grade, grade20 = :grade20, skip = :skip
        WHERE id = :id
    )");

    query.bindValue(":grade", gradeMeta.grade);
    query.bindValue(":grade20", gradeMeta.grade20);
    query.bindValue(":skip",gradeMeta.skip);
    query.bindValue(":id", gradeMeta.id);

    if (!query.exec())
        setErrorMessage(QString("Error while updating student grade. GradeID: %1, SubjectName: %2\n%3")
                            .arg(gradeMeta.id).arg(gradeMeta.subjectName).arg(query.lastError().text()));
}

void DatabaseAccess::addGrade(GradeMetaData &gradeMeta, int trimesterID, int studentID, int subjectID)
{
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO grade(grade, grade20, f_subject, f_student, f_trimester, skip)
        VALUES (:grade, :grade20, :f_subject, :f_student, :f_trimester, :skip)
    )");

    query.bindValue(":grade", gradeMeta.grade);
    query.bindValue(":grade20", gradeMeta.grade20);
    query.bindValue(":f_subject", subjectID);
    query.bindValue(":f_student", studentID);
    query.bindValue(":f_trimester", trimesterID);
    query.bindValue(":skip", gradeMeta.skip);

    if (query.exec())
        gradeMeta.id = query.lastInsertId().toInt();

    else
    {
        setErrorMessage(QString("Error while Adding new student grade to the database. Subject name: %1\n%2")
                            .arg(gradeMeta.subjectName).arg(query.lastError().text()));
    }
}

void DatabaseAccess::getTrimesterAverages(std::vector<TrimesterAVG> &averages, int classID, int trimesterID)
{
    QSqlQuery query;
    query.prepare(R"(
        SELECT trimavg.id as trimAVGid, avg, rank, f_trimester, f_student, total
        FROM trimavg
        INNER JOIN student
        ON student.id  = trimavg.f_student
        WHERE student.f_klass = :klassID  AND f_trimester = :trimester
    )");

    query.bindValue(":klassID", classID);
    query.bindValue(":trimester", trimesterID);

    if (!query.exec())
    {
        setErrorMessage(QString("Error while reading students trimester averages from the database. KlassID: %1, Trimester: %2\n%3")
                            .arg(classID).arg(trimesterID).arg(query.lastError().text()));
        return;
    }

    while (query.next())
    {
        TrimesterAVG trimAVG;
        trimAVG.id = query.value("trimAVGid").toInt();
        trimAVG.avg = query.value("avg").toDouble();
        trimAVG.rank = query.value("rank").toInt();
        trimAVG.trimester = query.value("f_trimester").toInt();
        trimAVG.studentid = query.value("f_student").toInt();
        trimAVG.total = query.value("total").toDouble();

        averages.push_back(trimAVG);
    }
}

void DatabaseAccess::addTrimesterAVG(TrimesterAVG &trimAVG)
{
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO trimAVG(avg, rank, f_trimester, f_student, total)
        VALUES (:avg, :rank, :trimester, :f_student, :total)
    )");

    query.bindValue(":avg", trimAVG.avg);
    query.bindValue(":rank", trimAVG.rank);
    query.bindValue(":trimester", trimAVG.trimester);
    query.bindValue(":f_student", trimAVG.studentid);
    query.bindValue(":total", trimAVG.total);

    if (query.exec())
    {
        trimAVG.id = query.lastInsertId().toInt();
        return;
    }

    setErrorMessage(QString("Error while Adding new student trimester average to the database.\n%2")
                       .arg(query.lastError().text()));
    qDebug() << errorMessage;
}

void DatabaseAccess::updateTrimAVG(const TrimesterAVG &trimAVG)
{
    QSqlQuery query;
    query.prepare(R"(
        UPDATE trimavg
        SET avg = :avg, rank = :rank, total = :total
        WHERE id = :id
    )");

    query.bindValue(":avg", trimAVG.avg);
    query.bindValue(":rank", trimAVG.rank);
    //query.bindValue(":trimester", trimAVG.trimester);
    //query.bindValue(":f_student", trimAVG.studentid);
    query.bindValue(":total", trimAVG.total);
    query.bindValue(":id", trimAVG.id);

    if (!query.exec())
    {
        setErrorMessage(QString("Trimester average update error: %1").arg(query.lastError().text()));
        qDebug() << errorMessage;
    }
}

void DatabaseAccess::updateSubject(const Subject &subject)
{
    QSqlQuery query;
    query.prepare(R"(
        UPDATE subject
        SET subjectName = :subjectName, subjectCoef = :subjectCoef
        WHERE id = :id
    )");

    query.bindValue(":subjectName", subject.subjectName());
    query.bindValue(":subjectCoef", subject.subjectCoef());
    query.bindValue(":id", subject.subjectId());

    if (!query.exec())
    {
        setErrorMessage(QString("Error while Updating subject: %1.\n%2")
                            .arg(subject.subjectName()).arg(query.lastError().text()));
        qDebug() << errorMessage;
    }
}

void DatabaseAccess::removeSubject(const Subject &subject)
{
    QSqlQuery query;
    query.prepare(R"(
        DELETE FROM subject WHERE id = :id
    )");

    query.bindValue(":id", subject.subjectId());

    if (!query.exec())
    {
        setErrorMessage(QString("Error while removing subject: %1.\n%2")
                            .arg(subject.subjectName()).arg(query.lastError().text()));
        qDebug() << errorMessage;
    }
}

void DatabaseAccess::getGrade(GradeMetaData &grade, int studentID, int subjectID, int trimester)
{
    QSqlQuery query;
    query.prepare(R"(
        SELECT  *
        FROM grade
        WHERE f_student = :f_student
        AND  f_subject  = :f_subject
        AND f_trimester  = :f_trimester
    )");

    query.bindValue(":f_student", studentID);
    query.bindValue(":f_subject", subjectID);
    query.bindValue(":f_trimester", trimester);

    if (query.exec())
    {
        if (query.next())
        {
            grade.id = query.value("id").toInt();
            grade.skip = query.value("skip").toBool();
        }
    }
    else {
        setErrorMessage(QString("Grade reading error.%2")
                            .arg(query.lastError().text()));
        qDebug() << errorMessage;
    }
}

void DatabaseAccess::trimAVG(TrimesterAVG &avg)
{
    QSqlQuery query;
    query.prepare(R"(
        SELECT id
        FROM trimavg
        WHERE f_student = :f_student
        AND f_trimester = :trimester
    )");

    query.bindValue(":f_student", avg.studentid);
    query.bindValue(":trimester", avg.trimester);
    if (query.exec())
    {
        if (query.next())
        {
            avg.id = query.value("id").toInt();
        }
    }
    else
    {
        setErrorMessage(QString("Trimester average reading error.%2")
                            .arg(query.lastError().text()));
        qDebug() << errorMessage;
    }
}

void DatabaseAccess::updateGrade20(int gradeID, double grade20)
{
    QSqlQuery query;
    query.prepare(R"(
        UPDATE grade
        SET grade20 = :grade20
        WHERE id = :id
    )");

    query.bindValue(":grade20", grade20);
    query.bindValue(":id", gradeID);

    if (!query.exec())
    {
        setErrorMessage(QString("Grade20 update error %1").arg(query.lastError().text()));
        qDebug() << errorMessage;
    }
}

std::vector<FinalAVG> DatabaseAccess::getFinalAVGs(int classID)
{
    std::vector<FinalAVG> avgs;
    QSqlQuery query(database);
    query.prepare(R"(
        SELECT avg_id, avg, rank, f_student
        FROM final_avg
        INNER JOIN student
        ON student.id = f_student
        WHERE student.f_klass = :klassID
    )");

    query.bindValue(":klassID", classID);

    if (!query.exec())
    {
        setErrorMessage(QString("Final AVGs reading error update error %1").arg(query.lastError().text()));
        qDebug() << errorMessage;
        return avgs;
    }

    while (query.next())
    {
        FinalAVG avg;
        avg.setId(query.value("avg_id").toInt());
        avg.setAvg(query.value("avg").toDouble());
        avg.setRank(query.value("rank").toInt());
        avg.setStudentId(query.value("f_student").toInt());

        avgs.push_back(avg);
    }
    return avgs;
}

std::vector<TrimesterAVG> DatabaseAccess::getTrimesterAVGs(int trimester, int klassID)
{
    std::vector<TrimesterAVG> avgs;

    QSqlQuery query(database);
    query.prepare(R"(
        SELECT trimavg.id AS trimavg_id, avg, rank, f_trimester, f_student, total
        FROM trimavg
        INNER JOIN student
        ON student.id = f_student
        WHERE f_trimester = :trimester AND f_klass = :klassID
    )");

    query.bindValue(":trimester", trimester);
    query.bindValue(":klassID", klassID);

    if (!query.exec())
    {
        setErrorMessage(QString("Trimester AVGs reading error %1").arg(query.lastError().text()));
        qDebug() << errorMessage;
        return avgs;
    }

    while (query.next())
    {
        TrimesterAVG avg;
        avg.trimester = trimester;
        avg.avg = query.value("avg").toDouble();
        avg.id = query.value("trimavg_id").toInt();
        avg.total = query.value("total").toDouble();
        avg.rank = query.value("rank").toInt();
        avg.studentid = query.value("f_student").toInt();
        avgs.push_back(avg);
    }

    return avgs;
}

void DatabaseAccess::addFinalAverage(FinalAVG &final)
{
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO final_avg(avg, rank, f_student)
        VALUES (:final, :rank, :f_student)
    )");
    query.bindValue(":final", final.avg());
    query.bindValue(":rank", final.rank());
    query.bindValue(":f_student", final.studentId());

    if (query.exec())
        final.setId(query.lastInsertId().toInt());

    else
    {
        setErrorMessage(QString("Final average insert error %1").arg(query.lastError().text()));
        qDebug() << errorMessage;
    }
}

void DatabaseAccess::updateFinalAverage(const FinalAVG &final)
{
    QSqlQuery query;
    query.prepare(R"(
        UPDATE final_avg
        SET avg = :final, rank = :rank
        WHERE avg_id = :id
    )");
    query.bindValue(":final", final.avg());
    query.bindValue(":rank", final.rank());
    query.bindValue(":id", final.id());

    if (!query.exec())
    {
        setErrorMessage(QString("Final average UPDATE error %1").arg(query.lastError().text()));
        qDebug() << errorMessage;
    }
}

Klass DatabaseAccess::classByID(int klassID)
{
    Klass klass;
    QSqlQuery query(database);
    query.prepare(R"(
        SELECT * FROM klass WHERE id = :id
    )");

    query.bindValue(":id", klassID);

    if (query.exec())
    {
        if (query.next())
        {
            klass.setClassId(klassID);
            klass.setClassName(query.value("klassName").toString());
        }
    }

    else
    {
        setErrorMessage(QString("Class by ID reading error %1").arg(query.lastError().text()));
        qDebug() << errorMessage;
    }

    return klass;

}

int DatabaseAccess::getStudentCount(const QString &schoolYear)
{
    QSqlQuery query;
    query.prepare(R"(
        SELECT COUNT(student.id)
        FROM student
        INNER JOIN klass ON klass.id = student.f_klass
        INNER JOIN school_year ON f_schoolYear  = school_year .id
        WHERE school_year.schoolYear  = :schoolYear;
    )");

    query.bindValue(":schoolYear", schoolYear);
    if (query.exec())
    {
        if (query.next())
            return query.value(0).toInt();
    }

    else
    {
        setErrorMessage(QString("Student count reading error %1").arg(query.lastError().text()));
        qDebug() << errorMessage;
    }

    return 0;
}

int DatabaseAccess::getSexeCount(const QString &schoolYear, const QString &sexe)
{
    QSqlQuery query;
    query.prepare(R"(
        SELECT COUNT(student.id) FROM student
        INNER JOIN klass ON klass.id = student.f_klass
        INNER JOIN school_year ON f_schoolYear  = school_year .id
        WHERE school_year.schoolYear  = :schoolYear
        AND student.sexe = :sexe

    )");

    query.bindValue(":schoolYear", schoolYear);
    query.bindValue(":sexe", sexe);
    if (query.exec())
    {
        if (query.next())
            return query.value(0).toInt();
    }

    else
    {
        setErrorMessage(QString("Student gender count reading error %1").arg(query.lastError().text()));
        qDebug() << errorMessage;
    }

    return 0;
}

int DatabaseAccess::studentCountByClass(int klassID)
{
    int count = 0;
    QSqlQuery query(database);
    query.prepare(R"(
        SELECT COUNT(student.id)
        FROM student
        INNER JOIN klass ON klass.id = student.f_klass
        WHERE klass.id = :klassID
    )");

    query.bindValue(":klassID", klassID);
    if (query.exec())
    {
        if (query.next())
            count = query.value(0).toInt();
    }

    else
    {
        setErrorMessage(QString("Student count reading error %1").arg(query.lastError().text()));
        qDebug() << errorMessage;
    }

    return count;
}
