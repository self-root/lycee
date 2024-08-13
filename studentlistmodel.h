#ifndef STUDENTLISTMODEL_H
#define STUDENTLISTMODEL_H

#include <QAbstractTableModel>
#include "student.h"

class StudentListModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit StudentListModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void loadStudents(int klassId);

    void addStudent(Student student);

    void updateStudent(Student student);

    void deleteStudent(const Student &student);

    Student studentByName(const QString &name);

private:
    std::vector<Student> students;
    int klassID;
};

#endif // STUDENTLISTMODEL_H
