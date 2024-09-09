#ifndef UTILS_H
#define UTILS_H

#include <QMetaType>
#include <QStringList>
#include <QDebug>

#include "subject.h"
#include "student.h"
#include "studentgrade.h"
#include "trimesteravg.h"
#include "finalavg.h"

enum Order{
    Ask,
    Desc
};
Q_DECLARE_METATYPE(Order);
enum FilterBy{
    Number,
    Rank
};
Q_DECLARE_METATYPE(FilterBy);
enum Target{
    PDF,
    Excel
};
Q_DECLARE_METATYPE(Target);

class Utils
{
public:
    static void makeTotalisationHeader(QStringList &header, const std::vector<Subject> &subjects)
    {
        header << "Num" << "Nom";
        for (const Subject &subject : subjects)
        {
            QString subjectName = subject.subjectName();
            subjectName.truncate(3);
            header << subjectName;
        }

        header << "Total" << "Moyenne" << "Rang";
    }

    static void makeFinalTotalisationHeader(QStringList &header, const std::vector<Subject> &subjects)
    {
        header << "Num" << "Nom";
        for (const Subject &subject : subjects)
        {
            QString subjectName = subject.subjectName();
            subjectName.truncate(3);
            header << subjectName;
        }

        header << "Total" << "Moyenne" << "Moyenne G" << "Rang";
    }

    static GradeMetaData gradeFor(const Student &student, const Subject &subjct, std::vector<StudentGrade> &grades)
    {
        GradeMetaData gradeMeta;
        for (const StudentGrade &grade : grades)
        {
            //qDebug() << "Student id in grade: " << grade.mStudent.id() << " current student: " << student.id() << " subjectID: " << subjct.subjectId() << " grade subject ID: " << grade.mSubject.subjectId();
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

    static Student studentFor(const TrimesterAVG &trimAVG, const std::vector<Student> &students)
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

    static TrimesterAVG trimAVGFor(const Student &student, const std::vector<TrimesterAVG> &avgs)
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

    static FinalAVG finalAVGFor(const Student &student, const std::vector<FinalAVG> &finals)
    {
        FinalAVG final;
        for (const FinalAVG &f : finals)
        {
            if (f.studentId() == student.id())
            {
                final = f;
                break;
            }
        }

        return final;
    }

    static Student studentForFinal(const FinalAVG &final, const std::vector<Student> &students)
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
};



#endif // UTILS_H
