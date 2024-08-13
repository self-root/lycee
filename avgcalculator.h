#ifndef AVGCALCULATOR_H
#define AVGCALCULATOR_H

#include "studentgrade.h"
#include "trimesteravg.h"

#include <vector>

class AVGCalculator
{
public:
    AVGCalculator();

    static std::vector<TrimesterAVG> computeTrimesterAverage(std::vector<StudentGrade> &grades,
                                        const std::vector<Student> &students,
                                        int trimester);

    static TrimesterAVG computeStudentAverage(const std::vector<GradeMetaData> &gradeMeta, int studentID, int trimester);

    static void sortAVG(std::vector<TrimesterAVG> &trimAVGs);
};

#endif // AVGCALCULATOR_H
