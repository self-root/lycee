#include "pdfcreator.h"
#include <QTextDocument>
#include <QPrinter>
#include <QPainter>
#include <QFile>
#include <QDir>

#include "databaseaccess.h"
#include "student.h"
#include "trimesteravg.h"
#include "studentgrade.h"
#include "subject.h"
#include "finalavg.h"

PdfCreator::PdfCreator(QObject *parent)
    : QObject{parent}
{
    QLocale::setDefault(QLocale::French);
}

void PdfCreator::createTranscript(int classID, int trimester, QString out, QMap<QString, QString> schoolInfo_, const QString &schoolYear)
{
    qDebug() << "Creating PDF...";
    qDebug() << "ClassID: " << classID;
    qDebug() << "trimester: " << trimester;
    std::vector<Student> students = DatabaseAccess::instance()->loadStudentsByClass(classID);
    std::vector<TrimesterAVG> trimesterAVGs = DatabaseAccess::instance()->getTrimesterAVGs(trimester, classID);
    std::vector<Subject> subjects = DatabaseAccess::instance()->getSubjectByClass(classID);
    std::vector<StudentGrade> grades;
    qDebug() << "Trimavgs size: " << trimesterAVGs.size() << "Students: " << students.size();

    Klass klass = DatabaseAccess::instance()->classByID(classID);
    DatabaseAccess::instance()->studentGrades(classID, trimester, grades, students, subjects);

    double classAVG = classAverage(trimesterAVGs);
    QString htmlText;

    htmlText += R"(<table width=100% height=100% style="border: 1px solid black; border-collapse: collapse; width: 100%;">)";

    for (const TrimesterAVG &trimAvg : trimesterAVGs)
    {
        Student student = studentFor(trimAvg, students);
        htmlText += R"(
            <tr>
        )";

        for (int i = 0; i < 2; i++)
        {
            htmlText += "<td style='width: 100%;'>";
            htmlText += transcriptHeader.arg(trimester)
                            .arg(schoolInfo_.value("school_name"))
                            .arg(schoolYear)
                            .arg(schoolInfo_.value("code"))
                            .arg(student.number())
                            .arg(student.name())
                            .arg(klass.className())
                            .arg(student.matricule())
                            .arg(student.situation());

            htmlText += R"(<table style="border: 1px solid black; border-collapse: collapse; width: 100%;">)";
            htmlText += tableHeader;
            int totalCoef = 0;
            double grade20_total = 0.0;
            for (const Subject &subject : subjects)
            {
                GradeMetaData grade = gradeFor(student, subject, grades);
                if (grade.skip)
                    continue;
                htmlText += subjectRows
                .arg(subject.subjectName())
                                .arg(locale.toString(grade.grade20, 'g', 4))
                .arg(grade.coef)
                                .arg(locale.toString(grade.grade, 'g', 4))
                .arg(appreciation(grade.grade20));

                totalCoef += grade.coef;
                grade20_total += grade.grade20;
            }

            htmlText += tootal_rank_part
                            .arg(locale.toString(grade20_total, 'g', 5))
                .arg(totalCoef)
                            .arg(locale.toString(trimAvg.total, 'g', 5))
                            .arg(locale.toString(trimAvg.avg, 'g', 4))
                            .arg(locale.toString(classAVG, 'g', 4))
                .arg(trimAvg.rank)
                .arg(students.size());



            htmlText += "</table>";

            htmlText += footer.arg(schoolInfo_.value("place")).arg(schoolInfo_.value("principal"));

            htmlText += "</td>";

        }
        htmlText += "</tr>";
        htmlText += "<div style='page-break-before: always;'></div>";
    }

    htmlText += "</table>";

    QString htmlPath = writeHtml(htmlText);
    qDebug() << "HTML saved at: " << htmlPath;

    //QString cmd = QString("wkhtmltopdf -O %1").arg(htmlPath);
    //QDir dir(filePath);
    //QString path = dir.absoluteFilePath(filePath);
    //system(cmd.arg(fi));

    QTextDocument doc;
    doc.setHtml(htmlText);
    //doc.setDocumentMargin(0);

    QPrinter printer(QPrinter::HighResolution);
    printer.setPageMargins(QMarginsF(0,0,0,0), QPageLayout::Millimeter);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setOutputFileName(out);

    //QPainter painter(&printer);

    doc.print(&printer);
    //painter.end();
    emit pdfCreated();
}

void PdfCreator::createFinalTranscipt(int classID, QString out, QMap<QString, QString> schoolInfo_, const QString &schoolYear)
{
    std::vector<Student> students = DatabaseAccess::instance()->loadStudentsByClass(classID);
    std::vector<TrimesterAVG> trimesterAVGs_1 = DatabaseAccess::instance()->getTrimesterAVGs(1, classID);
    std::vector<TrimesterAVG> trimesterAVGs_2 = DatabaseAccess::instance()->getTrimesterAVGs(2, classID);
    std::vector<TrimesterAVG> trimesterAVGs_3 = DatabaseAccess::instance()->getTrimesterAVGs(3, classID);
    std::vector<Subject> subjects = DatabaseAccess::instance()->getSubjectByClass(classID);
    std::vector<StudentGrade> grades;
    qDebug() << "Trimavgs size: " << trimesterAVGs_1.size() << "Students: " << students.size();

    Klass klass = DatabaseAccess::instance()->classByID(classID);
    DatabaseAccess::instance()->studentGrades(classID, 3, grades, students, subjects);

    double classAVG = classAverage(trimesterAVGs_3);
    QString htmlText;

    htmlText += R"(<table width=100% height=100% style="border: 1px solid black; border-collapse: collapse; width: 100%;">)";

    std::vector<FinalAVG> finals = DatabaseAccess::instance()->getFinalAVGs(classID);

    for (const FinalAVG &finalAvg : finals)
    {
        Student student = studentForFinal(finalAvg, students);
        TrimesterAVG trimAvg = trimAVGFor(student, trimesterAVGs_3);
        TrimesterAVG trimAvg_1 = trimAVGFor(student, trimesterAVGs_1);
        TrimesterAVG trimAvg_2 = trimAVGFor(student, trimesterAVGs_2);

        htmlText += R"(
            <tr>
        )";

        for (int i = 0; i < 2; i++)
        {
            htmlText += "<td style='width: 100%;'>";
            htmlText += transcriptHeader.arg(3)
                            .arg(schoolInfo_.value("school_name"))
                            .arg(schoolYear)
                            .arg(schoolInfo_.value("code"))
                            .arg(student.number())
                            .arg(student.name())
                            .arg(klass.className())
                            .arg(student.matricule())
                            .arg(student.situation());

            htmlText += R"(<table style="border: 1px solid black; border-collapse: collapse; width: 100%;">)";
            htmlText += tableHeader;
            int totalCoef = 0;
            double grade20_total = 0.0;
            for (const Subject &subject : subjects)
            {
                GradeMetaData grade = gradeFor(student, subject, grades);
                if (grade.skip)
                    continue;
                htmlText += subjectRows
                                .arg(subject.subjectName())
                                .arg(locale.toString(grade.grade20, 'g', 4))
                                .arg(grade.coef)
                                .arg(locale.toString(grade.grade, 'g', 4))
                                .arg(appreciation(grade.grade20));

                totalCoef += grade.coef;
                grade20_total += grade.grade20;
            }

            htmlText += final_total_rank_part
                            .arg(locale.toString(grade20_total, 'g', 5))
                            .arg(totalCoef)
                            .arg(locale.toString(trimAvg.total, 'g', 5))
                            .arg(locale.toString(trimAvg.avg, 'g', 4))
                            .arg(locale.toString(classAVG, 'g', 4))
                            .arg(trimAvg.rank)
                            .arg(students.size())
                            .arg(finalAvg.rank())
                            .arg(finals.size());



            htmlText += "</table>";

            htmlText += final_footer
                            .arg(locale.toString(trimAvg_1.avg, 'g', 4))
                            .arg(locale.toString(trimAvg_2.avg, 'g', 4))
                            .arg(locale.toString(finalAvg.avg(), 'g', 4))
                            .arg(schoolInfo_.value("place"))
                            .arg(schoolInfo_.value("principal"));

            htmlText += "</td>";
        }
        htmlText += "</tr>";
        htmlText += "<div style='page-break-before: always;'></div>";
    }

    htmlText += "</table>";

    QString htmlPath = writeHtml(htmlText);
    qDebug() << "HTML saved at: " << htmlPath;

    //QString cmd = QString("wkhtmltopdf -O %1").arg(htmlPath);
    //QDir dir(filePath);
    //QString path = dir.absoluteFilePath(filePath);
    //system(cmd.arg(fi));

    QTextDocument doc;
    doc.setHtml(htmlText);
    //doc.setDocumentMargin(0);

    QPrinter printer(QPrinter::HighResolution);
    printer.setPageMargins(QMarginsF(0,0,0,0), QPageLayout::Millimeter);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setOutputFileName(out);

    //QPainter painter(&printer);

    doc.print(&printer);
    //painter.end();
    emit pdfCreated();
}

GradeMetaData PdfCreator::gradeFor(const Student &student, const Subject &subjct, std::vector<StudentGrade> &grades)
{
    GradeMetaData gradeMeta;
    for (const StudentGrade &grade : grades)
    {
        if (grade.mStudent.id() == student.id() && subjct.subjectId() == grade.mSubject.subjectId())
        {
            gradeMeta.grade = grade.grade();
            gradeMeta.grade20 = grade.grade20();
            gradeMeta.coef = grade.coef();
            gradeMeta.id = grade.getGradeID();
            gradeMeta.skip = grade.skip();
            break;
        }
    }
    return gradeMeta;
}

Student PdfCreator::studentFor(const TrimesterAVG &trimAVG, const std::vector<Student> &students)
{
    Student student;
    for (const Student &s : students)
    {
        if (trimAVG.studentid == s.id())
        {
            student = s;
            break;
        }
    }
    return student;
}

Student PdfCreator::studentForFinal(const FinalAVG &final, const std::vector<Student> &students)
{
    Student student;
    for (const Student &s : students)
    {
        if (s.id() == final.studentId())
        {
            student = s;
            break;
        }
    }

    return student;
}

TrimesterAVG PdfCreator::trimAVGFor(const Student &student, const std::vector<TrimesterAVG> &avgs)
{
    TrimesterAVG avg;
    for (const TrimesterAVG &_avg : avgs)
    {
        if (student.id() == _avg.studentid)
        {
            avg = _avg;
            break;
        }
    }
    return avg;
}

QString PdfCreator::appreciation(double grade20)
{
    if (grade20>19.99)
        return QString("Excellent");
    else if (grade20>17.99)
        return QString("Honorable");
    else if (grade20>15.99)
        return QString("Très-bien");
    else if (grade20>13.99)
        return QString("Bien");
    else if (grade20>11.99)
        return QString("Assez-Bien");
    else if (grade20>9.99)
        return QString("Passable");
    else if (grade20>6.99)
        return QString("Insuffisante");
    else if (grade20>4.99)
        return QString("Faible");
    else
        return QString("Très-Faible");
}

double PdfCreator::classAverage(const std::vector<TrimesterAVG> &avgs)
{
    double total = 0.0;

    for (const TrimesterAVG &trimavg : avgs)
    {
        total += trimavg.avg;
    }

    return total / avgs.size();
}

QString PdfCreator::writeHtml(const QString &html)
{
    QDir dir(filePath);
    QString path = dir.absoluteFilePath(filePath);
    QString htmlPath = QDir::cleanPath(path + QDir::separator() + "htmltemp.html");
    QFile file(htmlPath);
    if (file.open(QIODevice::Text))
    {
        file.write(html.toStdString().c_str());
    }
    return htmlPath;
}
