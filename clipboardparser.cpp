#include "clipboardparser.h"

#include <QDate>
#include <QLocale>
#include <QRegularExpression>

ClipBoardParser::ClipBoardParser() {}

GradeClipboardParseResult ClipBoardParser::parseGradeClipboard(const QString &data)
{
    GradeClipboardParseResult result;
    QStringList rows = data.split(QRegularExpression("[\\r\\n]+"), Qt::SkipEmptyParts);
    const QLocale locale;

    if (rows.isEmpty())
    {
        result.errors << "Le presse-papiers est vide.";
        return result;
    }

    const QStringList header = rows.takeFirst().split('\t');
    if (header.size() != 2 || header.at(1).trimmed().isEmpty())
    {
        result.errors << "La premiere ligne doit contenir deux colonnes et le nom de la matiere.";
        return result;
    }
    result.subjectName = header.at(1).trimmed();

    int line = 2;
    for (const QString &row : rows)
    {
        const QStringList values = row.split('\t');
        if (values.size() != 2 || values.at(0).trimmed().isEmpty())
        {
            result.errors << QString("Ligne %1: deux colonnes (eleve et note) sont requises.").arg(line++);
            continue;
        }

        bool ok = false;
        double value = values.at(1).trimmed().toDouble(&ok);
        if (!ok)
            value = locale.toDouble(values.at(1).trimmed(), &ok);
        if (!ok)
        {
            QString normalized = values.at(1).trimmed();
            normalized.replace(',', '.');
            value = normalized.toDouble(&ok);
        }
        if (!ok || value < 0.0)
        {
            result.errors << QString("Ligne %1: la note '%2' est invalide.")
                                 .arg(line++)
                                 .arg(values.at(1).trimmed());
            continue;
        }

        ClipboardGrade grade;
        grade.studentName = values.at(0).trimmed();
        grade.grade = value;
        result.grades.push_back(grade);
        ++line;
    }
    return result;
}

StudentClipboardParseResult ClipBoardParser::parseStudentsClipboard(const QString &data)
{
    StudentClipboardParseResult result;
    const QStringList rows = data.split(QRegularExpression("[\\r\\n]+"), Qt::SkipEmptyParts);
    int line = 1;

    for (const QString &row : rows)
    {
        const QStringList values = row.split('\t');
        if (values.size() != 5 && values.size() != 6)
        {
            result.errors << QString("Ligne %1: 5 ou 6 colonnes sont requises.").arg(line++);
            continue;
        }

        bool numberOk = false;
        const int number = values.at(0).trimmed().toInt(&numberOk);
        const QString name = values.at(1).trimmed();
        const QString sexe = values.at(2).trimmed().toUpper();
        const QString situation = values.at(3).trimmed().toUpper();
        QDate birthDate = QDate::fromString(values.at(4).trimmed(), "dd/MM/yyyy");
        if (!birthDate.isValid())
            birthDate = QDate::fromString(values.at(4).trimmed(), "d/M/yyyy");

        QStringList lineErrors;
        if (!numberOk || number < 1)
            lineErrors << "numero invalide";
        if (name.isEmpty())
            lineErrors << "nom vide";
        if (sexe != "M" && sexe != "F")
            lineErrors << "sexe attendu M ou F";
        if (situation != "P" && situation != "R")
            lineErrors << "situation attendue P ou R";
        if (!birthDate.isValid())
            lineErrors << "date attendue jj/mm/aaaa";

        if (!lineErrors.isEmpty())
        {
            result.errors << QString("Ligne %1: %2.").arg(line++).arg(lineErrors.join(", "));
            continue;
        }

        Student student;
        student.setNumber(number);
        student.setName(name);
        student.setSexe(sexe == "M" ? "Garçon" : "Fille");
        student.setSituation(situation == "P" ? "Passant(e)" : "Redoublant(e)");
        student.setBirthDay(birthDate.toString("dd-MM-yyyy"));
        if (values.size() == 6)
            student.setMatricule(values.at(5).trimmed());
        result.students.push_back(student);
        ++line;
    }

    if (rows.isEmpty())
        result.errors << "Le presse-papiers est vide.";
    return result;
}
