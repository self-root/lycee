#ifndef CLIPBOARDPARSER_H
#define CLIPBOARDPARSER_H

#include <QString>
#include "student.h"

struct Header
{
    Header() {}
    QString name;
};

struct ClipboardGrade : public Header
{
    ClipboardGrade() {}
    QString studentName;
    double grade;
};


class ClipBoardParser
{
public:
    ClipBoardParser();
    static std::vector<ClipboardGrade> parseGradeClipboard(const QString &data);
    static std::vector<Student> parseStudentsClipboard(const QString &data);
};

#endif // CLIPBOARDPARSER_H
