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

public slots:
    void onSaveStudent(Student student);
    void onUpdateStudent(Student student);
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
};

#endif // STUDENTSTABLEVIEW_H
