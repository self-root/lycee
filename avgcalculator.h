#ifndef AVGCALCULATOR_H
#define AVGCALCULATOR_H

#include "studentgrade.h"
#include "trimesteravg.h"
#include "finalavg.h"

#include <vector>

class AVGCalculator
{
public:
    AVGCalculator();

    static std::vector<TrimesterAVG> computeTrimesterAverage(std::vector<StudentGrade> &grades,
                                        const std::vector<Student> &students,
                                        int trimester);

    static TrimesterAVG computeStudentAverage(const std::vector<GradeMetaData> &gradeMeta, int studentID, int trimester);

    static void sortAVG(std::vector<TrimesterAVG> &trimAVGs, bool ascending=true);

    static std::vector<FinalAVG> computeFinalAVG(
                                    const std::vector<TrimesterAVG> &trimAVG_1,
                                    const std::vector<TrimesterAVG> &trimAVG_2,
                                    const std::vector<TrimesterAVG> &trimAVG_3,
                                    const std::vector<Student> &students);

    static TrimesterAVG trimAVGFor(int studentID, const std::vector<TrimesterAVG> &trimAVG);
    static void sortFinalAVG(std::vector<FinalAVG> &finalAVG);
    static void setFinalRanks(std::vector<FinalAVG> &finalAVG);
};

#endif // AVGCALCULATOR_H
