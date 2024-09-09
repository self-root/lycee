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
                s_grade.setGrade20(meta.grade / s_grade.coef());
                _grades.push_back(meta);

            }
        }

        TrimesterAVG avg = computeStudentAverage(_grades, student.id(), trimester);
        avgs.push_back(avg);
    }

    sortAVG(avgs);
    return avgs;
}

TrimesterAVG AVGCalculator::computeStudentAverage(const std::vector<GradeMetaData> &gradeMeta,
                                                  int studentID,
                                                  int trimester)
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

void AVGCalculator::sortAVG(std::vector<TrimesterAVG> &trimAVGs, bool ascending)
{
    if (ascending)
        std::sort(std::begin(trimAVGs), std::end(trimAVGs), [](const TrimesterAVG &a, const TrimesterAVG &b){return a.avg > b.avg;});
    else
        std::sort(std::begin(trimAVGs), std::end(trimAVGs), [](const TrimesterAVG &a, const TrimesterAVG &b){return a.avg < b.avg;});
}

std::vector<FinalAVG> AVGCalculator::computeFinalAVG(const std::vector<TrimesterAVG> &trimAVG_1,
                                                     const std::vector<TrimesterAVG> &trimAVG_2,
                                                     const std::vector<TrimesterAVG> &trimAVG_3,
                                                     const std::vector<Student> &students)
{
    std::vector<FinalAVG> avgs;
    for (const Student &student : students)
    {
        TrimesterAVG avg_1 = trimAVGFor(student.id(), trimAVG_1);
        TrimesterAVG avg_2 = trimAVGFor(student.id(), trimAVG_2);
        TrimesterAVG avg_3 = trimAVGFor(student.id(), trimAVG_3);

        double final_ = (avg_1.avg + avg_2.avg + (avg_3.avg * 2)) / 4;
        FinalAVG finalAVG;
        finalAVG.setAvg(final_);
        finalAVG.setStudentId(student.id());
        avgs.push_back(finalAVG);
    }

    sortFinalAVG(avgs);
    setFinalRanks(avgs);
    return avgs;
}

TrimesterAVG AVGCalculator::trimAVGFor(int studentID, const std::vector<TrimesterAVG> &trimAVG)
{
    TrimesterAVG avg;
    for (const TrimesterAVG &a : trimAVG)
    {
        if (a.studentid == studentID)
        {
            avg = a;
            break;
        }
    }

    return avg;
}

void AVGCalculator::sortFinalAVG(std::vector<FinalAVG> &finalAVG, bool ask)
{
    if (ask)
    {
        std::sort(std::begin(finalAVG), std::end(finalAVG), [](const FinalAVG &a, const FinalAVG &b){
            return a.avg() > b.avg();
        });
    }
    else
    {
        std::sort(std::begin(finalAVG), std::end(finalAVG), [](const FinalAVG &a, const FinalAVG &b){
            return a.avg() < b.avg();
        });
    }

}

void AVGCalculator::setFinalRanks( std::vector<FinalAVG> &finalAVG)
{
    for (std::size_t i = 0; i < finalAVG.size(); i++)
    {
        finalAVG.at(i).setRank(i + 1);
    }
}
