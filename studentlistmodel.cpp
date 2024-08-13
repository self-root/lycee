#include "studentlistmodel.h"
#include <QColor>
#include <QDate>
#include "databaseaccess.h"

StudentListModel::StudentListModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

QVariant StudentListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal)
    {
        switch (section) {
        case 0:{
            switch (role) {
            case Qt::DisplayRole:
                return "Numero";
                break;
            case Qt::ForegroundRole:
                //return QColor(255, 255, 255);
                break;
            default:
                return QVariant();
            }
        }
        case 1:{
            switch (role) {
            case Qt::DisplayRole:
                return "Nom et Prenom";
                break;
            case Qt::ForegroundRole:
                //return QColor(0, 0, 0);
                break;
            default:
                return QVariant();
            }
        }
        case 2:{
            switch (role) {
            case Qt::DisplayRole:
                return "Matricule";
                break;
            case Qt::ForegroundRole:
                //return QColor(0, 0, 0);
                break;
            default:
                return QVariant();
            }
        }
        case 3:{
            switch (role) {
            case Qt::DisplayRole:
                return "Sexe";
                break;
            case Qt::ForegroundRole:
                //return QColor(0, 0, 0);
                break;
            default:
                return QVariant();
            }
        }
        case 4:{
            switch (role) {
            case Qt::DisplayRole:
                return "Situation";
                break;
            case Qt::ForegroundRole:
                //return QColor(0, 0, 0);
                break;
            default:
                return QVariant();
            }
        }
        case 5:{
            switch (role) {
            case Qt::DisplayRole:
                return "Date de Naissance";
                break;
            case Qt::ForegroundRole:
                //return QColor(0, 0, 0);
                break;
            default:
                return QVariant();
            }
        }

        default:
            return QVariant();
            break;
        }
    }

    else if(orientation == Qt::Vertical)
    {
        switch (role) {
        case Qt::DisplayRole:
            return students.at(section).id();
            break;
        default:
            return QVariant();
            break;
        }
    }

    return QVariant();
}

int StudentListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return students.size();
}

int StudentListModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 6;
}

QVariant StudentListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Student student = students.at(index.row());
    switch (index.column()) {
    case 0:
        switch (role) {
        case Qt::DisplayRole:
            return student.number();
            break;
        default:
            break;
        }
        break;
    case 1:
        switch (role) {
        case Qt::DisplayRole:
            return student.name();
            break;
        default:
            break;
        }
        break;
    case 2:
        switch (role) {
        case Qt::DisplayRole:
            return student.matricule();
            break;
        default:
            break;
        }
        break;
    case 3:
        switch (role) {
        case Qt::DisplayRole:
            return student.sexe();
            break;
        default:
            break;
        }
        break;
    case 4:
        switch (role) {
        case Qt::DisplayRole:
            return student.situation();
            break;
        default:
            break;
        }
        break;
    case 5:
        switch (role) {
        case Qt::DisplayRole:
            return QDate::fromString(student.birthDay(), "dd-MM-yyyy");
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return QVariant();
}

void StudentListModel::loadStudents(int klassId)
{
    beginResetModel();
    students.clear();
    students = DatabaseAccess::instance()->loadStudentsByClass(klassId);
    klassID = klassId;
    endResetModel();

}

void StudentListModel::addStudent(Student student)
{
    beginResetModel();
    Student _student = student;
    DatabaseAccess::instance()->addStudent(_student, klassID);
    students.push_back(_student);
    endResetModel();
}

void StudentListModel::updateStudent(Student student)
{
    beginResetModel();
    DatabaseAccess::instance()->updateStudent(student, klassID);
    for (std::size_t i = 0; i < students.size(); i++)
    {
        if (students.at(i).name() == student.name())
        {
            students[i] = student;
            break;
        }
    }
    endResetModel();
}

void StudentListModel::deleteStudent(const Student &student)
{
    beginResetModel();
    DatabaseAccess::instance()->deleteStudent(student, klassID);
    for (std::size_t i = 0; i < students.size(); i++)
    {
        if (students.at(i).name() == student.name())
        {
            students.erase((std::begin(students) + i));
            break;
        }
    }
    endResetModel();
}

Student StudentListModel::studentByName(const QString &name)
{
    Student student;
    for (const Student &_student : students)
    {
        if (_student.name() == name)
        {
            student = _student;
            break;
        }
    }
    return student;
}

void StudentListModel::addStudentsFromClipboard(const std::vector<Student> &students)
{
    if (klassID < 0)
        return;
    for (const Student &s : students)
    {
        if (s.name() == studentByName(s.name()).name())
            continue;
        addStudent(s);
    }
}
