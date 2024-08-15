#ifndef FINALAVG_H
#define FINALAVG_H

class FinalAVG
{
public:
    FinalAVG();

    int id() const;
    void setId(int newId);

    int studentId() const;
    void setStudentId(int newStudentId);

    int rank() const;
    void setRank(int newRank);

    double avg() const;
    void setAvg(double newAvg);

private:
    int mId = -1;
    int mStudentId = -1;
    int mRank = -1;
    double mAvg = 0.0;
};

#endif // FINALAVG_H
