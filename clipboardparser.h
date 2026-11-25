#ifndef CLIPBOARDPARSER_H
#define CLIPBOARDPARSER_H

#include <QString>
#include <QStringList>
#include <vector>

#include "student.h"

struct ClipboardGrade
{
    QString studentName;
    double grade = 0.0;
};

struct GradeClipboardParseResult
{
    QString subjectName;
    std::vector<ClipboardGrade> grades;
    QStringList errors;
};

struct StudentClipboardParseResult
{
    std::vector<Student> students;
    QStringList errors;
};

class ClipBoardParser
{
public:
    ClipBoardParser();
    static GradeClipboardParseResult parseGradeClipboard(const QString &data);
    static StudentClipboardParseResult parseStudentsClipboard(const QString &data);
};

#endif // CLIPBOARDPARSER_H
