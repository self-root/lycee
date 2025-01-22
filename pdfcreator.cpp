#include "pdfcreator.h"
#include <QTextDocument>
#include <QPrinter>
#include <QPainter>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QDate>
#include <QTemporaryFile>
#include <QProcess>

#include "controller.h"
#include "avgcalculator.h"
#include "utils.h"

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
    trimesterAVGs.erase(std::remove_if(trimesterAVGs.begin(), trimesterAVGs.end(), [](TrimesterAVG avg){
        return avg.avg <= 0;
    }), trimesterAVGs.end());
    qDebug() << "Trimavgs size: " << trimesterAVGs.size() << "Students: " << students.size();

    Klass klass = dbAccess->classByID(classID);
    dbAccess->studentGrades(classID, trimester, grades, students, subjects);

    double classAVG = classAverage(trimesterAVGs);
    QString htmlText;
    QString currentDate = locale.toString(QDate::currentDate(), "dd MMM yyyy");

    htmlText += R"(<table width=100% height=100% style="border-collapse: collapse; width: 100%;">)";

    for (const TrimesterAVG &trimAvg : trimesterAVGs)
    {
        Student student = Utils::studentFor(trimAvg, students);
        htmlText += R"(
            <tr>
        )";

        for (int i = 0; i < 2; i++)
        {
            if (i == 0)
                htmlText += "<td style='border-right: 1px solid black; padding: 4mm;'>";
            else
                htmlText += "<td style='border-left: 1px solid black; padding: 4mm;'>";

            htmlText += R"(<table style="border-collapse: collapse; width: 100%;">)";
            htmlText += QStringView(transcriptHeader).arg(QString::number(trimester),
                                             schoolInfo_.value("school_name"),
                                             schoolYear,
                                             schoolInfo_.value("code"),
                                             QString::number(student.number()),
                                             student.name(),
                                             klass.className(),
                                             student.matricule(),
                                             student.situation());

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
                                .arg(Utils::toString(grade.grade20))
                                .arg(grade.coef)
                                .arg(Utils::toString(grade.grade))
                                .arg(appreciation(grade.grade20));

                totalCoef += grade.coef;
                grade20_total += grade.grade20;
            }

            htmlText += tootal_rank_part
                            .arg(Utils::toString(grade20_total))
                            .arg(totalCoef)
                            .arg(Utils::toString(trimAvg.total))
                            .arg(Utils::toString(trimAvg.avg))
                            .arg(locale.toString(classAVG, 'f', 2))
                            .arg(trimAvg.rank)
                            .arg(trimesterAVGs.size());


            htmlText += footer
                            .arg(schoolInfo_.value("place"))
                            .arg(currentDate)
                            .arg(schoolInfo_.value("principal"));
            htmlText += "</table>";



            htmlText += "</td>";

        }
        htmlText += "</tr>";
        htmlText += "<div style='page-break-before: always;'></div>";
    }

    htmlText += "</table>";

    QString html_body = html_template.arg(getCss(Controller::instance()->getTranscriptFormatSettings()))
                            .arg(htmlText);

    write(html_body, out);
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

    htmlText += R"(<table width=100% height=100% style="border-collapse: collapse; width: 100%;">)";

    std::vector<FinalAVG> finals = dbAccess->getFinalAVGs(classID);
    AVGCalculator::sortFinalAVG(finals);
    finals.erase(std::remove_if(finals.begin(), finals.end(), [](FinalAVG avg){
                            return avg.avg() <= 0;
                        }), finals.end());

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
            if (i == 0)
                htmlText += "<td style='border-right: 1px solid black; padding: 4mm;'>";
            else
                htmlText += "<td style='border-left: 1px solid black; padding: 4mm;'>";


            htmlText += R"(<table style="border-collapse: collapse; width: 100%;">)";
            htmlText += transcriptHeader.arg(3)
                            .arg(schoolInfo_.value("school_name"))
                            .arg(schoolYear)
                            .arg(schoolInfo_.value("code"))
                            .arg(student.number())
                            .arg(student.name())
                            .arg(klass.className())
                            .arg(student.matricule())
                            .arg(student.situation());
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
                                .arg(Utils::toString(grade.grade20))
                                .arg(grade.coef)
                                .arg(Utils::toString(grade.grade))
                                .arg(appreciation(grade.grade20));

                totalCoef += grade.coef;
                grade20_total += grade.grade20;
            }

            htmlText += final_total_rank_part
                            .arg(Utils::toString(grade20_total))
                            .arg(totalCoef)
                            .arg(Utils::toString(trimAvg.total))
                            .arg(Utils::toString(trimAvg.avg))
                            .arg(locale.toString(classAVG, 'f', 2))
                            .arg(trimAvg.rank)
                            .arg(students.size())
                            .arg(finalAvg.rank())
                            .arg(finals.size());

            htmlText += final_footer
                            .arg(locale.toString(trimAvg_1.avg, 'f', 2))
                            .arg(locale.toString(trimAvg_2.avg, 'f', 2))
                            .arg(locale.toString(finalAvg.avg(), 'f', 2))
                            .arg(schoolInfo_.value("place"))
                            .arg(currentDate)
                            .arg(schoolInfo_.value("principal"));

            htmlText += "</table>";



            htmlText += "</td>";
        }
        htmlText += "</tr>";
        htmlText += "<div style='page-break-before: always;'></div>";
    }

    htmlText += "</table>";

    QString html_body = html_template.arg(getCss(Controller::instance()->getTranscriptFormatSettings()))
                            .arg(htmlText);

    write(html_body, out);
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

    htmlText += "<thead><tr>";
    for (const QString &h : header)
    {
        htmlText += QString("<th style='border: 1px solid black;'>%1</th>").arg(h);
    }
    htmlText += "</tr></thead>";
    htmlText += "<tbody>";

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
                <td class="student-name" style='border: 1px solid black;'>%2</td>
            )").arg(student.number()).arg(student.name());

            for (const Subject &subject : subjects)
            {
                GradeMetaData grade = Utils::gradeFor(student, subject, grades);
                if (grade.skip)
                    htmlText += QString("<td class='notes' style='border: 1px solid black;'>NC</td>");
                else
                    htmlText += QString("<td class='notes' style='border: 1px solid black;'>%1</td>").arg(Utils::toString(grade.grade));
            }

            TrimesterAVG trimAVG = Utils::trimAVGFor(student, trimesterAVGs);
            htmlText += QString(R"(
                <td class="notes" style='border: 1px solid black;'>%1</td>
                <td class="notes" style='border: 1px solid black;'>%2</td>
                <td class="notes" style='border: 1px solid black;'>%3</td>
            )").arg(Utils::toString(trimAVG.total)).arg(Utils::toString(trimAVG.avg)).arg(trimAVG.rank);

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
                <td class="student-name" style='border: 1px solid black;'>%2</td>
            )").arg(student.number()).arg(student.name());

            for (const Subject &subject : subjects)
            {
                GradeMetaData grade = Utils::gradeFor(student, subject, grades);
                if (grade.skip)
                    htmlText += QString("<td class='notes' style='border: 1px solid black;'>NC</td>");
                else
                    htmlText += QString("<td class='notes' style='border: 1px solid black;'>%1</td>").arg(Utils::toString(grade.grade));
            }
            htmlText += QString(R"(
                <td class="notes" style='border: 1px solid black;'>%1</td>
                <td class="notes" style='border: 1px solid black;'>%2</td>
                <td class="notes" style='border: 1px solid black;'>%3</td>
            )").arg(Utils::toString(trimAVG.total)).arg(Utils::toString(trimAVG.avg)).arg(trimAVG.rank);

            htmlText += "</tr>";
        }
    }
    htmlText += "</tbody>";

    htmlText += "</table>";

    QString html_body = html_template.arg(getCss(Controller::instance()->getTotalizationFormatSettings()))
                            .arg(htmlText);

    write(html_body, out);

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

    htmlText += "<thead><tr>";
    for (const QString &h : header)
    {
        htmlText += QString("<th style='border: 1px solid black;'>%1</th>").arg(h);
    }
    htmlText += "</tr></thead>";
    htmlText += "<tbody>";

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
                <td class="student-name" style='border: 1px solid black;'>%2</td>
            )").arg(student.number()).arg(student.name());

            for (const Subject &subject : subjects)
            {
                GradeMetaData grade = Utils::gradeFor(student, subject, grades);
                if (grade.skip)
                    htmlText += QString("<td class='notes' style='border: 1px solid black;'>NC</td>");
                else
                    htmlText += QString("<td class='notes' style='border: 1px solid black;'>%1</td>").arg(Utils::toString(grade.grade));
            }
            FinalAVG final = Utils::finalAVGFor(student, finals);
            TrimesterAVG trimAVG = Utils::trimAVGFor(student, trimesterAVGs_3);
            htmlText += QString(R"(
                <td class="notes" style='border: 1px solid black;'>%1</td>
                <td class="notes" style='border: 1px solid black;'>%2</td>
                <td class="notes" style='border: 1px solid black;'>%3</td>
                <td class="notes" style='border: 1px solid black;'>%4</td>
            )").arg(Utils::toString(trimAVG.total))
                .arg(Utils::toString(trimAVG.avg))
                .arg(Utils::toString(final.avg()))
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
                <td class="student-name" style='border: 1px solid black;'>%2</td>
            )").arg(student.number()).arg(student.name());

            for (const Subject &subject : subjects)
            {
                GradeMetaData grade = Utils::gradeFor(student, subject, grades);
                if (grade.skip)
                    htmlText += QString("<td class='notes' style='border: 1px solid black;'>NC</td>");
                else
                    htmlText += QString("<td class='notes' style='border: 1px solid black;'>%1</td>").arg(Utils::toString(grade.grade));
            }
            TrimesterAVG trimAVG = Utils::trimAVGFor(student, trimesterAVGs_3);
            htmlText += QString(R"(
                <td class="notes" style='border: 1px solid black;'>%1</td>
                <td class="notes" style='border: 1px solid black;'>%2</td>
                <td class="notes" style='border: 1px solid black;'>%3</td>
                <td class="notes" style='border: 1px solid black;'>%4</td>
            )").arg(Utils::toString(trimAVG.total))
                .arg(Utils::toString(trimAVG.avg))
                .arg(Utils::toString(final.avg()))
                .arg(final.rank());

            htmlText += "</tr>";
        }
    }
    htmlText += "</tbody>";
    htmlText += "</table>";

    QString html_body = html_template.arg(getCss(Controller::instance()->getTotalizationFormatSettings()))
                            .arg(htmlText);

    write(html_body, out);

    emit finalTotalisationExcelCreated(out);

}

void PdfCreator::createFicheDeNote(int classID, const QString &out, const QString &schoolYear, int trimester)
{
    auto schoolInfo_ = Controller::instance()->getSchoolSettings();
    std::vector<Student> students = dbAccess->loadStudentsByClass(classID);
    Klass klass = dbAccess->classByID(classID);

    QString html = "<table style='width: 100%'>";

    html += QString(R"(
    <tr>
        <td>%1</td>
    </tr>
    <tr>
        <td>Code: %2</td>
    </tr>
    <tr>
        <td colspan="3"  align="center">Fiche de Notes du trimestre %3</td>
    </tr>
    <tr>
        <td colspan="3">Nom du professeur: </td>
    </tr>
    <tr>
        <td>Matière: </td>
        <td align="center">Classe: %4 </td>
        <td align="right">Année Scholaire: %5</td>
    </tr>
    </table>
    )").arg(schoolInfo_.value("school_name"))
        .arg(schoolInfo_.value("code"))
        .arg(trimester)
        .arg(klass.className())
        .arg(schoolYear);

    html += "<table style='width: 100%; border-collapse: collapse;'>";
    html += R"(
    <thead>
    <tr>
        <th style='border: 1px solid black;'>Num</th>
        <th style='border: 1px solid black;'>Nom et Prénom</th>
        <th style='border: 1px solid black;'>JRN 1</th>
        <th style='border: 1px solid black;'>JRN 2</th>
        <th style='border: 1px solid black;'>Moyenne J</th>
        <th style='border: 1px solid black;'>Composition</th>
        <th style='border: 1px solid black;'>Moyenne G</th>
        <th style='border: 1px solid black;'>Coef</th>
        <th style='border: 1px solid black;'>Note Def</th>
    </tr>
    </thead>
    )";

    html += "<tbody>";

    for (const Student &student : students)
    {
        html += QString(R"(
        <tr>
            <td style='border: 1px solid black;'>%1</td>
            <td class="student-name" style='border: 1px solid black;'>%2</td>
            <td style='border: 1px solid black;'></td>
            <td style='border: 1px solid black;'></td>
            <td style='border: 1px solid black;'></td>
            <td style='border: 1px solid black;'></td>
            <td style='border: 1px solid black;'></td>
            <td style='border: 1px solid black;'></td>
            <td style='border: 1px solid black;'></td>
        </tr>
        )").arg(student.number()).arg(student.name());
    }
    html += "</tbody>";
    html += "</table>";

    QString html_body = html_template.arg(getCss(Controller::instance()->getTotalizationFormatSettings()))
                            .arg(html);


    write(html_body, out);

    emit ficheDeNoteCreated(out);
    qDebug() << "Fiche de notes created...";
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

void PdfCreator::setCSS(const QMap<QString, QString> &settings, QTextDocument &textDoc)
{
    textDoc.setDefaultStyleSheet(QString(R"(
        *{
            font-family: '%1';
            font-size: %2pt;
        }
        td{
            padding: %3px;
        }
    )").arg(settings.value("font")).arg(settings.value("fontsize")).arg(settings.value("paddings")));
}

QString PdfCreator::getCss(const QMap<QString, QString> &settings)
{
    QString css = QString(R"(
        *{
            font-family: '%1';
            font-size: %2pt;
        }
        td{
            padding: %3px;
        }
        .notes{
            text-align: center;
        }
        .student-name{
            max-width: 400px;
            white-space: nowrap;
            overflow: hidden;
            text-overflow: ellipsis;
        }
    )").arg(settings.value("font")).arg(settings.value("fontsize")).arg(settings.value("paddings"));

    return css;
}

void PdfCreator::write(const QString &htmlBody, const QString &path)
{
    QString temp_html_path = QDir::cleanPath(QDir::tempPath() + QDir::separator() + "XXXXXX.html");

    qDebug() << "Temporary file template: " << temp_html_path;

    QTemporaryFile html_temp(temp_html_path);

    if (html_temp.open())
    {
        qDebug() << "Temporary file name: " << html_temp.fileName();
        html_temp.write(htmlBody.toStdString().c_str());
        QProcess process;
        QString program = "wkhtmltopdf.exe";
        QStringList arguments;
        arguments << "--orientation" << "Landscape";
        arguments << "--margin-top" << "4mm";
        arguments << "--margin-right" << "4mm";
        arguments << "--margin-bottom" << "4mm";
        arguments << "--margin-left" << "4mm";
        arguments << html_temp.fileName() << path;

        process.start(program, arguments);
        if (process.waitForStarted())
        {
            qDebug() << "Creating PDF using wkhtmltopdf.exe";
            if (process.waitForFinished())
                qDebug() << "wkhtmltopdf.exe finished successfully";
            else
            {
                qDebug() << "wkhtmltopdf.exe process ended with error";
            }
        }
        else
            qDebug() << "Could not create pdf using: wkhtmltopdf.exe";
    }
    else {
        qDebug() << "could not open temporary html file";
    }
}
