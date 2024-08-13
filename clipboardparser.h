#ifndef CLIPBOARDPARSER_H
#define CLIPBOARDPARSER_H

#include <QString>

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
};

#endif // CLIPBOARDPARSER_H
