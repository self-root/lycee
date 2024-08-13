#include "clipboardparser.h"
#include <QStringList>
#include <QDebug>
#include <QDate>

ClipBoardParser::ClipBoardParser() {}

std::vector<ClipboardGrade> ClipBoardParser::parseGradeClipboard(const QString &data)
{
    std::vector<ClipboardGrade> grades;
    QStringList rows = data.split("\n");

    try {
        ClipboardGrade header;
        QString firstRow = rows.at(0);
        auto pair = firstRow.split("\t");
        if (pair.length() == 2)
        {
            header.name = pair.at(1);
            grades.push_back(header);
            rows.removeFirst();
        }

    } catch (...) {
        return grades;
    }

    for (auto &row : rows)
    {
        ClipboardGrade grade;
        try {
            auto nameGradePair = row.split("\t");
            if (nameGradePair.length() == 2)
            {
                grade.studentName = nameGradePair.at(0);
                grade.grade = nameGradePair.at(1).toDouble();
                grades.push_back(grade);
            }

        } catch (...) {
            return grades;
        }
    }

    return grades;
}

std::vector<Student> ClipBoardParser::parseStudentsClipboard(const QString &data)
{
    std::vector<Student> students;

    QStringList rows = data.split("\n");

    for (auto &row : rows)
    {
        Student student;
        QStringList rowValues = row.split("\t");
        if (rowValues.length() == 6)
        {
            student.setNumber(rowValues.at(0).toInt());
            student.setName(rowValues.at(1));
            QString sexe = rowValues.at(2);
            if (sexe == "M")
                student.setSexe("Garçon");
            else
            {
                student.setSexe("Fille");
            }

            QString situation = rowValues.at(3);
            if (situation == "P")
                student.setSituation("Passant(e)");
            else
            {
                student.setSituation("Redoublant(e)");
            }

            QDate dateNsiss = QDate::fromString(rowValues.at(4), "dd/MM/yyyy");
            student.setBirthDay(dateNsiss.toString("dd-MM-yyyy"));
            student.setMatricule(rowValues.at(5));
            students.push_back(student);
        }

        else if (rowValues.length() == 5)
        {
            student.setNumber(rowValues.at(0).toInt());
            student.setName(rowValues.at(1));
            QString sexe = rowValues.at(2);
            if (sexe == "M")
                student.setSexe("Garçon");
            else
            {
                student.setSexe("Fille");
            }

            QString situation = rowValues.at(3);
            if (situation == "P")
                student.setSituation("Passant(e)");
            else
            {
                student.setSituation("Redoublant(e)");
            }

            QDate dateNsiss = QDate::fromString(rowValues.at(4), "dd/MM/yyyy");
            student.setBirthDay(dateNsiss.toString("dd-MM-yyyy"));
        }
    }

    return students;
}
