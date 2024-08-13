#include "avgcalculator.h"


AVGCalculator::AVGCalculator() {}

std::vector<TrimesterAVG> AVGCalculator::computeTrimesterAverage(std::vector<StudentGrade> &grades,
                                            const std::vector<Student> &students,
                                            int trimester)
{
    std::vector<TrimesterAVG> avgs;

    for (const Student &student : students)
    {
        std::vector<GradeMetaData> _grades;

        for (StudentGrade &s_grade: grades)
        {
            if (s_grade.mStudent.id() == student.id())
            {
                GradeMetaData meta;
                meta.grade = s_grade.grade();
                meta.grade20 = s_grade.grade20();
                meta.skip = s_grade.skip();
                meta.subjectName = s_grade.mSubject.subjectName();
                meta.coef = s_grade.coef();
                s_grade.setGrade20(s_grade.grade20() / s_grade.coef());
                _grades.push_back(meta);

            }
        }

        TrimesterAVG avg = computeStudentAverage(_grades, student.id(), trimester);
        avgs.push_back(avg);
    }

    sortAVG(avgs);
    return avgs;
}

TrimesterAVG AVGCalculator::computeStudentAverage(const std::vector<GradeMetaData> &gradeMeta, int studentID, int trimester)
{
    int totalCoef = 0;
    double totalScore = 0.0;
    double average = 0.0;

    for (const GradeMetaData &grade : gradeMeta)
    {
        if (!grade.skip)
        {
            totalCoef += grade.coef;
            totalScore += grade.grade;
        }
    }

    if (totalCoef > 0)
        average = totalScore / totalCoef;

    TrimesterAVG trimAVG;
    trimAVG.avg = average;
    trimAVG.total = totalScore;
    trimAVG.studentid = studentID;
    trimAVG.trimester = trimester;

    return trimAVG;
}

void AVGCalculator::sortAVG(std::vector<TrimesterAVG> &trimAVGs)
{
    std::sort(std::begin(trimAVGs), std::end(trimAVGs), [](const TrimesterAVG &a, const TrimesterAVG &b){return a.avg > b.avg;});
}
