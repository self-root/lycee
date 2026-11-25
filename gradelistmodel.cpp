#include "gradelistmodel.h"
#include "databaseaccess.h"
#include "avgcalculator.h"
#include "controller.h"

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
        case Qt::Vertical:
        {
            Student currentStudent = students.at(section);
            return currentStudent.number();
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
    case Qt::EditRole:{
        if (col <= 0 || col > static_cast<int>(subjects.size()))
            break;
        double grade = gradeForSubject(currentStudent, subjects.at(col-1));
        if (grade != -1)
            return grade;
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
        if (grade.mStudent.id() == student.id() && grade.mSubject.subjectId() == subject.subjectId())
        {
            if (grade.skip())
                __grade = -2;
            else
                __grade = grade.grade();
        }
    }
    return __grade;
}

GradeMetaData GradeListModel::gradeFor(const Student &student, const Subject &subject) const
{
    GradeMetaData gradeMeta;
    for (const StudentGrade &grade : studentsGrade)
    {
        if (grade.mStudent.id() == student.id() && grade.mSubject.subjectId() == subject.subjectId())
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

Student GradeListModel::studentAt(int row) const
{
    if (row < 0 || row >= static_cast<int>(students.size()))
        return Student();
    return students.at(row);
}

Subject GradeListModel::subjectFromID(int subjectID) const
{
    for (const Subject &subject : subjects)
    {
        if (subject.subjectId() == subjectID)
            return subject;
    }
    return Subject();
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
    std::vector<TrimesterAVG> trimAVGs = AVGCalculator::computeTrimesterAverage(studentsGrade, students, currentTrimester);

    //TODO: Get rank
    setRanks(trimAVGs);
    updateGrade20();
    //TODO: add/update trimAVG in the database
    for (TrimesterAVG &avg : trimAVGs)
    {
        DatabaseAccess::instance()->trimAVG(avg);
        if (avg.id > 0)
        {
            qDebug() << "Updating: " << avg.id << "s: " << avg.studentid << " t: " << avg.trimester << " total: " << avg.total;
            DatabaseAccess::instance()->updateTrimAVG(avg);
        }
        else
        {
            qDebug() << "Adding new: " << avg.id << "s: " << avg.studentid << " t: " << avg.trimester << " total: " << avg.total;
            DatabaseAccess::instance()->addTrimesterAVG(avg);
        }
    }
    Controller::instance()->checkDbError();
    //TODO resetModel
    loadModelData(currentTrimester, currentClass);
}

void GradeListModel::setRanks(std::vector<TrimesterAVG> &avgs)
{
    int i = 1;
    for (TrimesterAVG &avg : avgs)
    {
        if (avg.avg <= 0)
            continue;
        avg.rank = i;
        i++;
    }
    /*for (std::size_t i = 0; i < avgs.size(); i++)
    {
        if (avgs[i].avg <= 0)
            continue;
        avgs.at(i).rank = i + 1;
    }*/
}

int GradeListModel::importGrades(const GradeClipboardParseResult &result, QStringList &rejectedRows)
{
    if (currentClass < 1 || currentTrimester < 1 || currentTrimester > 3)
    {
        rejectedRows << "Aucune classe ou trimestre valide n'est selectionne.";
        return 0;
    }

    std::vector<Subject> matchingSubjects;
    for (const Subject &subject : subjects)
    {
        if (subject.subjectName().compare(result.subjectName, Qt::CaseInsensitive) == 0)
            matchingSubjects.push_back(subject);
    }
    if (matchingSubjects.size() != 1)
    {
        rejectedRows << QString("La matiere '%1' est absente ou ambigue dans cette classe.")
                            .arg(result.subjectName);
        return 0;
    }

    std::vector<GradeImportRecord> records;
    for (const ClipboardGrade &clipboardGrade : result.grades)
    {
        std::vector<Student> matches;
        for (const Student &student : students)
        {
            if (student.name().compare(clipboardGrade.studentName, Qt::CaseInsensitive) == 0)
                matches.push_back(student);
        }
        if (matches.size() != 1)
        {
            rejectedRows << QString("Eleve '%1' absent ou nom en double.").arg(clipboardGrade.studentName);
            continue;
        }

        GradeImportRecord record;
        record.studentID = matches.front().id();
        record.subjectID = matchingSubjects.front().subjectId();
        record.trimesterID = currentTrimester;
        record.grade = clipboardGrade.grade;
        records.push_back(record);
    }

    int imported = 0;
    if (!records.empty() && DatabaseAccess::instance()->importGrades(records, &imported))
        loadModelData(currentTrimester, currentClass);
    return imported;
}

void GradeListModel::updateGrade20()
{
    for (const StudentGrade &grade : studentsGrade)
    {
        DatabaseAccess::instance()->updateGrade20(grade.getGradeID(), grade.grade20());
    }

    Controller::instance()->checkDbError();
}

Qt::ItemFlags GradeListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    if (index.column() <= 0 || index.column() >= _header.size() - 3)
        return QAbstractTableModel::flags(index);

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool GradeListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        bool converted = false;
        double newGrade = 0.0;
        newGrade = value.toDouble(&converted);
        if (!converted)
            return false;

        if (index.column() < 1 || index.column() > static_cast<int>(subjects.size()))
            return false;
        Student student = studentAt(index.row());
        Subject subject = subjects.at(index.column() - 1);
        GradeMetaData grade = gradeFor(student, subject);
        qDebug() << "Data editing: " << student.name() << " - " << subject.subjectName() << " -> " << newGrade;
        if (grade.id > 0)
        {
            if (newGrade != grade.grade)
            {
                grade.grade = newGrade;
                updateGrade(grade);
                return true;
            }

            return false;
        }
        else
        {
            grade.grade = newGrade;
            grade.coef = subject.subjectCoef();
            grade.subjectName = subject.subjectName();
            addGrade(grade, student, subject);
            return true;
        }
        return true;
    }
    return false;
}

int GradeListModel::getCurrentClass()
{
    return currentClass;
}

void GradeListModel::addGrade(GradeMetaData &gradeMeta, const Student &student, const Subject &subject)
{
    if (student.id() < 1 || subject.subjectId() < 1 || currentTrimester < 1)
        return;
    gradeMeta.subjectName = subject.subjectName();
    if (DatabaseAccess::instance()->addGrade(gradeMeta, currentTrimester, student.id(), subject.subjectId()))
        loadModelData(currentTrimester, currentClass);
}

void GradeListModel::makeHeader()
{
    _header.clear();
    if (students.size() > 0)
    {
        _header << "Nom et Prénom";
        subjectsList(_header);
        _header << "Total" << "Moyenne" << "Rang";
    }

}
