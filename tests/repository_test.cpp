#include <QtTest>
#include <QTemporaryDir>

#include "avgcalculator.h"
#include "clipboardparser.h"
#include "databaseaccess.h"

class RepositoryTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void calculatesAverages();
    void performsCrudById();
    void rollsBackInvalidBulkImport();
    void validatesClipboardRows();

private:
    QTemporaryDir temporaryDirectory;
    DatabaseAccess *database = nullptr;
    int classID = -1;
};

void RepositoryTest::initTestCase()
{
    QVERIFY(temporaryDirectory.isValid());
    database = new DatabaseAccess(temporaryDirectory.filePath("lycee-test.sqlite"));
    QVERIFY2(!database->hasDbError(), qPrintable(database->getErrorMessage()));

    QVERIFY(database->addSchoolYear("2026-2027"));
    const int yearID = database->schoolYearID("2026-2027");
    QVERIFY(yearID > 0);
    QVERIFY(database->addClass("Seconde A", yearID));
    const std::vector<Klass> classes = database->gestClasses("2026-2027");
    QCOMPARE(classes.size(), std::size_t(1));
    classID = classes.front().classId();
}

void RepositoryTest::cleanupTestCase()
{
    delete database;
    database = nullptr;
}

void RepositoryTest::calculatesAverages()
{
    GradeMetaData math;
    math.grade = 32.0;
    math.coef = 2;
    GradeMetaData french;
    french.grade = 14.0;
    french.coef = 1;
    const TrimesterAVG average = AVGCalculator::computeStudentAverage({math, french}, 42, 1);
    QCOMPARE(average.total, 46.0);
    QCOMPARE(average.avg, 46.0 / 3.0);

    std::vector<double> trimesters = {12.0, 14.0, 16.0};
    QCOMPARE(AVGCalculator::computeFinal(trimesters), 14.5);
}

void RepositoryTest::performsCrudById()
{
    Student student;
    student.setName("Alex Martin");
    student.setNumber(1);
    student.setSexe("Garcon");
    student.setSituation("Passant(e)");
    QVERIFY(database->addStudent(student, classID));
    QVERIFY(student.id() > 0);

    student.setName("Alex Renomme");
    QVERIFY(database->updateStudent(student, classID));
    std::vector<Student> students = database->loadStudentsByClass(classID);
    QCOMPARE(students.size(), std::size_t(1));
    QCOMPARE(students.front().id(), student.id());
    QCOMPARE(students.front().name(), QString("Alex Renomme"));

    Subject subject;
    subject.setSubjectName("Mathematiques");
    subject.setSubjectCoef(2);
    QVERIFY(database->addSubject(subject, classID));

    GradeMetaData grade;
    grade.grade = 30.0;
    grade.subjectName = subject.subjectName();
    QVERIFY(database->addGrade(grade, 1, student.id(), subject.subjectId()));
    QVERIFY(grade.id > 0);

    grade.grade = 34.0;
    QVERIFY(database->updateStudentGrade(grade));
    GradeMetaData stored;
    database->getGrade(stored, student.id(), subject.subjectId(), 1);
    QCOMPARE(stored.id, grade.id);

    QVERIFY(database->deleteStudent(student, classID));
    QVERIFY(database->loadStudentsByClass(classID).empty());
}

void RepositoryTest::rollsBackInvalidBulkImport()
{
    Student student;
    student.setName("Import Test");
    student.setNumber(2);
    student.setSexe("Fille");
    student.setSituation("Passant(e)");
    QVERIFY(database->addStudent(student, classID));

    const Subject subject = database->getSubjectByClass(classID).front();
    GradeImportRecord valid;
    valid.studentID = student.id();
    valid.subjectID = subject.subjectId();
    valid.trimesterID = 2;
    valid.grade = 15.0;
    GradeImportRecord invalid = valid;
    invalid.studentID = -1;

    QVERIFY(!database->importGrades({valid, invalid}));
    database->getErrorMessage();
    GradeMetaData stored;
    database->getGrade(stored, student.id(), subject.subjectId(), 2);
    QCOMPARE(stored.id, -1);
}

void RepositoryTest::validatesClipboardRows()
{
    const GradeClipboardParseResult grades =
        ClipBoardParser::parseGradeClipboard("Eleve\tMathematiques\nAlex\t12,5\nSam\tn/a");
    QCOMPARE(grades.subjectName, QString("Mathematiques"));
    QCOMPARE(grades.grades.size(), std::size_t(1));
    QCOMPARE(grades.errors.size(), 1);

    const StudentClipboardParseResult students =
        ClipBoardParser::parseStudentsClipboard("1\tAlex\tX\tP\t01/01/2010");
    QVERIFY(students.students.empty());
    QCOMPARE(students.errors.size(), 1);
}

QTEST_MAIN(RepositoryTest)
#include "repository_test.moc"
