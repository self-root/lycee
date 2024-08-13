#include "gradelistmodel.h"
#include "databaseaccess.h"
#include "avgcalculator.h"

GradeListModel::GradeListModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

QVariant GradeListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        switch (orientation) {
        case Qt::Horizontal:{
            return _header.at(section);
            break;
        }
        default:
            break;
        }
        break;
    default:
        break;
    }

    return QVariant();
}

int GradeListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return students.size();
}

int GradeListModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return _header.length();
}

QVariant GradeListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Student currentStudent = students.at(index.row());
    TrimesterAVG avg = trimesterAvgFor(currentStudent.id());
    int col = index.column();
    switch (role) {
    case Qt::DisplayRole:{

        if (col == 0)
            return currentStudent.name();
        else if (col > _header.size() - 4)
        {
            if (col == _header.size() - 3)
                return avg.total;
            else if (col == _header.size() - 2)
                return avg.avg;
            else {
                return (avg.rank > 0 ? avg.rank : QVariant());
            }
        }
        else {
            double grade = gradeForSubject(currentStudent, subjects.at(col-1));
            if (grade != -1)
                return grade;
        }
        break;
    }
    default:
        break;
    }
    return QVariant();
}

void GradeListModel::loadModelData(int trimester, int classID)
{
    beginResetModel();
    currentClass = classID;
    currentTrimester = trimester;
    subjects = DatabaseAccess::instance()->getSubjectByClass(classID);
    students = DatabaseAccess::instance()->loadStudentsByClass(classID);
    studentsGrade.clear();
    trimesterAVG.clear();
    DatabaseAccess::instance()->studentGrades(classID, trimester, studentsGrade, students, subjects);
    DatabaseAccess::instance()->getTrimesterAverages(trimesterAVG, currentClass, currentTrimester);
    makeHeader();
    endResetModel();
}

void GradeListModel::subjectsList(QStringList &list)
{
    for (const Subject &sub : subjects)
        list << sub.subjectName();
}

double GradeListModel::gradeForSubject(const Student &student, const Subject &subject) const
{
    double __grade = -1;
    for (const StudentGrade &grade : studentsGrade)
    {
        if (grade.mStudent.name() == student.name() && grade.mSubject.subjectName() == subject.subjectName())
        {
            if (grade.skip())
                __grade = -2;
            else
                __grade = grade.grade();
        }
    }
    return __grade;
}

GradeMetaData GradeListModel::gradeFor(const QString &studentName, const QString &subjectName) const
{
    GradeMetaData gradeMeta;
    for (const StudentGrade &grade : studentsGrade)
    {
        if (grade.mStudent.name() == studentName && grade.mSubject.subjectName() == subjectName)
        {
            gradeMeta.subjectName = grade.mSubject.subjectName();
            gradeMeta.grade = grade.grade();
            gradeMeta.skip = grade.skip();
            gradeMeta.id = grade.getGradeID();
            break;
        }
    }

    return gradeMeta;
}

Student GradeListModel::studentFromName(const QString &studentName)
{
    Student _student;
    for (Student &_s : students)
    {
        if (_s.name() ==  studentName)
        {
            _student = _s;
            break;
        }
    }
    return _student;
}

Subject GradeListModel::subjectFromName(const QString &subjectName)
{
    Subject subject;
    for (Subject &_subject : subjects)
    {
        if (_subject.subjectName() == subjectName)
            subject = _subject;
    }

    return subject;
}

void GradeListModel::updateGrade(const GradeMetaData &gradeMeta)
{
    DatabaseAccess::instance()->updateStudentGrade(gradeMeta);
    loadModelData(currentTrimester, currentClass);
}

TrimesterAVG GradeListModel::trimesterAvgFor(int studentID) const
{
    TrimesterAVG avg;
    for (const TrimesterAVG &trimAVG : trimesterAVG)
    {
        if (trimAVG.studentid == studentID)
        {
            avg = trimAVG;
            break;
        }
    }

    return avg;
}

void GradeListModel::computeAVG()
{
    trimesterAVG = AVGCalculator::computeTrimesterAverage(studentsGrade, students, currentTrimester);
    for (const TrimesterAVG &t : trimesterAVG)
    {
        qDebug() << "id: " << t.id << " total: " << t.total << " avg: " << t.avg;
    }

    //TODO: Get rank
    setRanks(trimesterAVG);
    //TODO: add/update trimAVG in the database
    //TODO resetModel
}

void GradeListModel::setRanks(std::vector<TrimesterAVG> &avgs)
{
    for (std::size_t i = 0; i < avgs.size(); i++)
    {
        avgs.at(i).rank = i + 1;
    }
}

void GradeListModel::addGrade(GradeMetaData &gradeMeta, const QString &studentName)
{
    Subject subject = subjectFromName(gradeMeta.subjectName);
    int trimester = currentTrimester;
    Student student = studentFromName(studentName);
    DatabaseAccess::instance()->addGrade(gradeMeta, trimester, student.id(), subject.subjectId());
    loadModelData(currentTrimester, currentClass);
}

void GradeListModel::makeHeader()
{
    _header.clear();
    if (students.size() > 0)
    {
        _header << "Nom";
        subjectsList(_header);
        _header << "Total" << "Moyenne" << "Rang";
    }

}
