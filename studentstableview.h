#ifndef STUDENTSTABLEVIEW_H
#define STUDENTSTABLEVIEW_H

#include <QTableView>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>

#include "studentlistmodel.h"

class StudentsTableView : public QTableView
{
    Q_OBJECT
public:
    StudentsTableView(QTableView *parent=nullptr);

    void loadStudents(int id);

    Student selectedStudent();

    void deleteStudent();

    void dropEvent(QDropEvent *event) override;

public slots:
    void onSaveStudent(Student student);
    void onUpdateStudent(Student student);
private:
    StudentListModel *model;
    QSortFilterProxyModel *proxyModel;
};

#endif // STUDENTSTABLEVIEW_H
