#include "pdfcreator.h"
#include <QTextDocument>
#include <QPrinter>
#include <QPainter>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QDate>

#include "controller.h"
#include "avgcalculator.h"

PdfCreator::PdfCreator(QObject *parent)
    : QObject{parent}
{
    dbAccess = new DatabaseAccess;
}

void PdfCreator::createTranscript(int classID, int trimester, QString out, const QString &schoolYear)
{
    qDebug() << "Creating PDF...";
    qDebug() << "ClassID: " << classID;
    qDebug() << "trimester: " << trimester;
    QMap<QString, QString> schoolInfo_ = Controller::instance()->getSchoolSettings();
    std::vector<Student> students = dbAccess->loadStudentsByClass(classID);
    std::vector<TrimesterAVG> trimesterAVGs = dbAccess->getTrimesterAVGs(trimester, classID);
    AVGCalculator::sortAVG(trimesterAVGs);
    std::vector<Subject> subjects = dbAccess->getSubjectByClass(classID);
    std::vector<StudentGrade> grades;
    qDebug() << "Trimavgs size: " << trimesterAVGs.size() << "Students: " << students.size();

    Klass klass = dbAccess->classByID(classID);
    dbAccess->studentGrades(classID, trimester, grades, students, subjects);

    double classAVG = classAverage(trimesterAVGs);
    QString htmlText;
    QString currentDate = locale.toString(QDate::currentDate(), "dd MMM yyyy");

    htmlText += R"(<table width=100% height=100% style="border: 1px solid black; border-collapse: collapse; width: 100%;">)";

    for (const TrimesterAVG &trimAvg : trimesterAVGs)
    {
        Student student = Utils::studentFor(trimAvg, students);
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
                GradeMetaData grade = Utils::gradeFor(student, subject, grades);
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

            htmlText += footer
                    .arg(schoolInfo_.value("place"))
                    .arg(currentDate)
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

void PdfCreator::createFinalTranscipt(int classID, QString out, const QString &schoolYear)
{
    auto schoolInfo_ = Controller::instance()->getSchoolSettings();
    std::vector<Student> students = dbAccess->loadStudentsByClass(classID);
    std::vector<TrimesterAVG> trimesterAVGs_1 = dbAccess->getTrimesterAVGs(1, classID);
    std::vector<TrimesterAVG> trimesterAVGs_2 = dbAccess->getTrimesterAVGs(2, classID);
    std::vector<TrimesterAVG> trimesterAVGs_3 = dbAccess->getTrimesterAVGs(3, classID);
    std::vector<Subject> subjects = dbAccess->getSubjectByClass(classID);
    std::vector<StudentGrade> grades;
    qDebug() << "Trimavgs size: " << trimesterAVGs_1.size() << "Students: " << students.size();

    Klass klass = dbAccess->classByID(classID);
    dbAccess->studentGrades(classID, 3, grades, students, subjects);
    QString currentDate = locale.toString(QDate::currentDate(), "dd MMM yyyy");

    double classAVG = classAverage(trimesterAVGs_3);
    QString htmlText;

    htmlText += R"(<table width=100% height=100% style="border: 1px solid black; border-collapse: collapse; width: 100%;">)";

    std::vector<FinalAVG> finals = dbAccess->getFinalAVGs(classID);
    AVGCalculator::sortFinalAVG(finals);

    for (const FinalAVG &finalAvg : finals)
    {
        Student student = Utils::studentForFinal(finalAvg, students);
        TrimesterAVG trimAvg = Utils::trimAVGFor(student, trimesterAVGs_3);
        TrimesterAVG trimAvg_1 = Utils::trimAVGFor(student, trimesterAVGs_1);
        TrimesterAVG trimAvg_2 = Utils::trimAVGFor(student, trimesterAVGs_2);

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
                GradeMetaData grade = Utils::gradeFor(student, subject, grades);
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
                            .arg(currentDate)
                            .arg(schoolInfo_.value("principal"));

            htmlText += "</td>";
        }
        htmlText += "</tr>";
        htmlText += "<div style='page-break-before: always;'></div>";
    }

    htmlText += "</table>";

    QString htmlPath = writeHtml(htmlText);
    qDebug() << "HTML saved at: " << htmlPath;

    QTextDocument doc;
    doc.setHtml(htmlText);
    //doc.setDocumentMargin(0);

    QPrinter printer(QPrinter::HighResolution);
    printer.setPageMargins(QMarginsF(0,0,0,0), QPageLayout::Millimeter);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setOutputFileName(out);

    doc.print(&printer);
    //painter.end();
    emit pdfCreated();
}

void PdfCreator::createTotalisationPDF(int classID,
                                       int trimester,
                                       QString out,
                                       const QString &schoolYear,
                                       Order by,
                                       FilterBy filter)
{
    QMap<QString, QString> schoolInfo_ = Controller::instance()->getSchoolSettings();
    std::vector<Student> students = dbAccess->loadStudentsByClass(classID);
    std::vector<TrimesterAVG> trimesterAVGs = dbAccess->getTrimesterAVGs(trimester, classID);
    std::vector<Subject> subjects = dbAccess->getSubjectByClass(classID);
    std::vector<StudentGrade> grades;
    qDebug() << "Trimavgs size: " << trimesterAVGs.size() << "Students: " << students.size();

    Klass klass = dbAccess->classByID(classID);
    dbAccess->studentGrades(classID, trimester, grades, students, subjects);

    QString htmlText;
    htmlText += totalisation_header
            .arg(trimester)
            .arg(schoolInfo_.value("school_name"))
            .arg(schoolYear)
            .arg(schoolInfo_.value("code"))
            .arg(klass.className());
    htmlText += "<table style='border: 1px solid black; border-collapse: collapse; width: 100%;'>";

    QStringList header;

    Utils::makeTotalisationHeader(header, subjects);

    htmlText += "<tr>";
    for (const QString &h : header)
    {
        htmlText += QString("<td style='border: 1px solid black;'>%1</td>").arg(h);
    }
    htmlText += "</tr>";

    if (filter == FilterBy::Number)
    {
        if (by == Order::Ask)
        {
            std::sort(std::begin(students), std::end(students), [](const Student &a, const Student &b){
                return a.number() < b.number();
            });
        }
        else
        {
            std::sort(std::begin(students), std::end(students), [](const Student &a, const Student &b){
                return a.number() > b.number();
            });
        }


        for (const Student &student : students)
        {
            htmlText += "<tr>";
            htmlText += QString(R"(
                <td style='border: 1px solid black;'>%1</td>
                <td style='border: 1px solid black;'>%2</td>
            )").arg(student.number()).arg(student.name());

            for (const Subject &subject : subjects)
            {
                GradeMetaData grade = Utils::gradeFor(student, subject, grades);
                if (grade.skip)
                    htmlText += QString("<td style='border: 1px solid black;'>NC</td>");
                else
                    htmlText += QString("<td style='border: 1px solid black;'>%1</td>").arg(locale.toString(grade.grade, 'g', 4));
            }

            TrimesterAVG trimAVG = Utils::trimAVGFor(student, trimesterAVGs);
            htmlText += QString(R"(
                <td style='border: 1px solid black;'>%1</td>
                <td style='border: 1px solid black;'>%2</td>
                <td style='border: 1px solid black;'>%3</td>
            )").arg(locale.toString(trimAVG.total, 'g', 5)).arg(locale.toString(trimAVG.avg, 'g', 4)).arg(trimAVG.rank);

            htmlText += "</tr>";
        }


    }
    else // filter by rank/avg
    {
        if (by == Order::Ask)
        {
            AVGCalculator::sortAVG(trimesterAVGs);
        }

        else
        {
            AVGCalculator::sortAVG(trimesterAVGs, false);
        }

        for (const TrimesterAVG &trimAVG : trimesterAVGs)
        {
            Student student = Utils::studentFor(trimAVG, students);
            htmlText += "<tr>";
            htmlText += QString(R"(
                <td style='border: 1px solid black;'>%1</td>
                <td style='border: 1px solid black;'>%2</td>
            )").arg(student.number()).arg(student.name());

            for (const Subject &subject : subjects)
            {
                GradeMetaData grade = Utils::gradeFor(student, subject, grades);
                if (grade.skip)
                    htmlText += QString("<td style='border: 1px solid black;'>NC</td>");
                else
                    htmlText += QString("<td style='border: 1px solid black;'>%1</td>").arg(locale.toString(grade.grade, 'g', 4));
            }
            htmlText += QString(R"(
                <td style='border: 1px solid black;'>%1</td>
                <td style='border: 1px solid black;'>%2</td>
                <td style='border: 1px solid black;'>%3</td>
            )").arg(locale.toString(trimAVG.total, 'g', 5)).arg(locale.toString(trimAVG.avg, 'g', 4)).arg(trimAVG.rank);

            htmlText += "</tr>";
        }
    }

    htmlText += "</table>";

    QTextDocument doc;
    doc.setHtml(htmlText);
    qDebug() << htmlText;

    QPrinter printer(QPrinter::HighResolution);
    printer.setPageMargins(QMarginsF(0,0,0,0), QPageLayout::Millimeter);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setOutputFileName(out);

    doc.print(&printer);

    emit totalisationPDFCreated(out);
}

void PdfCreator::createFinalTotalisationPDF(int classID, QString out, const QString &schoolYear, Order order, FilterBy by)
{
    auto schoolInfo_ = Controller::instance()->getSchoolSettings();
    std::vector<Student> students = dbAccess->loadStudentsByClass(classID);
    std::vector<TrimesterAVG> trimesterAVGs_1 = dbAccess->getTrimesterAVGs(1, classID);
    std::vector<TrimesterAVG> trimesterAVGs_2 = dbAccess->getTrimesterAVGs(2, classID);
    std::vector<TrimesterAVG> trimesterAVGs_3 = dbAccess->getTrimesterAVGs(3, classID);
    std::vector<Subject> subjects = dbAccess->getSubjectByClass(classID);
    std::vector<StudentGrade> grades;
    qDebug() << "Trimavgs size: " << trimesterAVGs_1.size() << "Students: " << students.size();
    dbAccess->studentGrades(classID, 3, grades, students, subjects);
    Klass klass = dbAccess->classByID(classID);

    std::vector<FinalAVG> finals = dbAccess->getFinalAVGs(classID);
    AVGCalculator::sortFinalAVG(finals);

    QString htmlText;
    htmlText += totalisation_header
            .arg(3)
            .arg(schoolInfo_.value("school_name"))
            .arg(schoolYear)
            .arg(schoolInfo_.value("code"))
            .arg(klass.className());
    htmlText += "<table style='border: 1px solid black; border-collapse: collapse; width: 100%;'>";

    QStringList header;

    Utils::makeFinalTotalisationHeader(header, subjects);

    htmlText += "<tr>";
    for (const QString &h : header)
    {
        htmlText += QString("<td style='border: 1px solid black;'>%1</td>").arg(h);
    }
    htmlText += "</tr>";

    if (by == FilterBy::Number)
    {
        if (order == Order::Ask)
        {
            std::sort(std::begin(students), std::end(students), [](const Student &a, const Student &b){
                return a.number() < b.number();
            });
        }
        else
        {
            std::sort(std::begin(students), std::end(students), [](const Student &a, const Student &b){
                return a.number() > b.number();
            });
        }


        for (const Student &student : students)
        {
            htmlText += "<tr>";
            htmlText += QString(R"(
                <td style='border: 1px solid black;'>%1</td>
                <td style='border: 1px solid black;'>%2</td>
            )").arg(student.number()).arg(student.name());

            for (const Subject &subject : subjects)
            {
                GradeMetaData grade = Utils::gradeFor(student, subject, grades);
                if (grade.skip)
                    htmlText += QString("<td style='border: 1px solid black;'>NC</td>");
                else
                    htmlText += QString("<td style='border: 1px solid black;'>%1</td>").arg(locale.toString(grade.grade, 'g', 4));
            }
            FinalAVG final = Utils::finalAVGFor(student, finals);
            TrimesterAVG trimAVG = Utils::trimAVGFor(student, trimesterAVGs_3);
            htmlText += QString(R"(
                <td style='border: 1px solid black;'>%1</td>
                <td style='border: 1px solid black;'>%2</td>
                <td style='border: 1px solid black;'>%3</td>
                <td style='border: 1px solid black;'>%4</td>
            )").arg(locale.toString(trimAVG.total, 'g', 5))
                .arg(locale.toString(trimAVG.avg, 'g', 4))
                .arg(locale.toString(final.avg(), 'g', 4))
                .arg(final.rank());

            htmlText += "</tr>";
        }


    }
    else // filter by rank/avg
    {
        if (order == Order::Ask)
        {
            AVGCalculator::sortFinalAVG(finals);
        }

        else
        {
            AVGCalculator::sortFinalAVG(finals, false);
        }

        for (const FinalAVG &final : finals)
        {
            Student student = Utils::studentForFinal(final, students);
            htmlText += "<tr>";
            htmlText += QString(R"(
                <td style='border: 1px solid black;'>%1</td>
                <td style='border: 1px solid black;'>%2</td>
            )").arg(student.number()).arg(student.name());

            for (const Subject &subject : subjects)
            {
                GradeMetaData grade = Utils::gradeFor(student, subject, grades);
                if (grade.skip)
                    htmlText += QString("<td style='border: 1px solid black;'>NC</td>");
                else
                    htmlText += QString("<td style='border: 1px solid black;'>%1</td>").arg(locale.toString(grade.grade, 'g', 4));
            }
            TrimesterAVG trimAVG = Utils::trimAVGFor(student, trimesterAVGs_3);
            htmlText += QString(R"(
                <td style='border: 1px solid black;'>%1</td>
                <td style='border: 1px solid black;'>%2</td>
                <td style='border: 1px solid black;'>%3</td>
                <td style='border: 1px solid black;'>%4</td>
            )").arg(locale.toString(trimAVG.total, 'g', 5))
                .arg(locale.toString(trimAVG.avg, 'g', 4))
                .arg(locale.toString(final.avg(), 'g', 4))
                .arg(final.rank());

            htmlText += "</tr>";
        }
    }

    htmlText += "</table>";

    QTextDocument doc;
    doc.setHtml(htmlText);
    qDebug() << htmlText;

    QPrinter printer(QPrinter::HighResolution);
    printer.setPageMargins(QMarginsF(0,0,0,0), QPageLayout::Millimeter);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setOutputFileName(out);

    doc.print(&printer);

    emit finalTotalisationExcelCreated(out);

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
