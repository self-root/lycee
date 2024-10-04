#ifndef GRADELISTMODEL_H
#define GRADELISTMODEL_H

#include <QAbstractTableModel>
#include <QLocale>

#include <vector>

#include "student.h"
#include "subject.h"
#include "studentgrade.h"
#include "trimesteravg.h"


class GradeListModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit GradeListModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void loadModelData(int trimester, int classID);

    void subjectsList(QStringList &list);

    double gradeForSubject(const Student &student, const Subject &subject) const;

    GradeMetaData gradeFor(const QString &studentName, const QString &subjectName) const;

    Student studentFromName(const QString &studentName);

    Subject subjectFromName(const QString &subjectName);

    void addGrade(GradeMetaData &gradeMeta, const QString &studentName);

    void updateGrade(const GradeMetaData &gradeMeta);

    TrimesterAVG trimesterAvgFor(int studentID) const;

    void computeAVG();

    void setRanks(std::vector<TrimesterAVG> &avgs);

    void studentGradeFromClipboard(GradeMetaData &grade, const QString &studentName);

    void updateGrade20();

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    int getCurrentClass();

private:
    std::vector<Student> students;
    std::vector<Subject> subjects;
    std::vector<StudentGrade> studentsGrade;
    std::vector<TrimesterAVG> trimesterAVG;
    QStringList _header;
    int currentClass = -1;
    int currentTrimester = -1;
    void makeHeader();
    QLocale locale;
};

#endif // GRADELISTMODEL_H
