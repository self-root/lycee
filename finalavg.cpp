#include "finalavg.h"

FinalAVG::FinalAVG() {}

int FinalAVG::id() const
{
    return mId;
}

void FinalAVG::setId(int newId)
{
    mId = newId;
}

int FinalAVG::studentId() const
{
    return mStudentId;
}

void FinalAVG::setStudentId(int newStudentId)
{
    mStudentId = newStudentId;
}

int FinalAVG::rank() const
{
    return mRank;
}

void FinalAVG::setRank(int newRank)
{
    mRank = newRank;
}

double FinalAVG::avg() const
{
    return mAvg;
}

void FinalAVG::setAvg(double newAvg)
{
    mAvg = newAvg;
}
