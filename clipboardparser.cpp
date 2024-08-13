#include "clipboardparser.h"
#include <QStringList>
#include <QDebug>

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
