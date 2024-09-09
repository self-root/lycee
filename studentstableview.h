#ifndef STUDENTSTABLEVIEW_H
#define STUDENTSTABLEVIEW_H

#include <QTableView>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>

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

    void contextMenuEvent(QContextMenuEvent *event) override;

    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

public slots:
    void onSaveStudent(Student student);
    void onUpdateStudent(Student student);
    void onDeleteAction(bool triggered);
    void onAddAction(bool _);
    void onUpdateAction(bool _);
    void onPasteAction(bool _);
private:
    StudentListModel *model;
    QSortFilterProxyModel *proxyModel;
    QMenu *menu = nullptr;
    QAction *addAction = nullptr;
    QAction *editAction = nullptr;
    QAction *removeAction = nullptr;
    QAction *pasteAction = nullptr;

    void setupMenu();

signals:
    void addStudent();
    void updateStudent();
};

#endif // STUDENTSTABLEVIEW_H
