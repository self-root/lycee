#include "studentstableview.h"
#include "controller.h"
#include "clipboardparser.h"

#include <QDropEvent>
#include <QMimeData>
#include <QClipboard>
#include <QApplication>

StudentsTableView::StudentsTableView(QTableView *parent) : QTableView(parent)
{
    setSortingEnabled(true);
    proxyModel = new QSortFilterProxyModel;
    model = new StudentListModel;
    proxyModel->setSourceModel(model);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setModel(proxyModel);
    this->setupMenu();
}

void StudentsTableView::loadStudents(int id)
{
    model->loadStudents(id);
}

Student StudentsTableView::selectedStudent()
{
    Student student;
    if (currentIndex().row() >= 0)
    {
        QModelIndex index = currentIndex().siblingAtColumn(1);

        qDebug() << "Current student: " << proxyModel->data(index);
        student = model->studentByName(proxyModel->data(index).toString());
    }
    return student;
}

void StudentsTableView::deleteStudent()
{
    Student student = selectedStudent();
    model->deleteStudent(student);
    Controller::instance()->checkDbError();
}

void StudentsTableView::dropEvent(QDropEvent *event)
{
    qDebug() << "Drop event";
    qDebug() << event->mimeData();
}

void StudentsTableView::contextMenuEvent(QContextMenuEvent *event)
{
    menu->exec(event->globalPos());
}

void StudentsTableView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)
    QString currentStudentName = proxyModel->data(current.sibling(current.row(), 1)).toString();
    qDebug() <<  currentStudentName;
}

void StudentsTableView::onSaveStudent(Student student)
{
    model->addStudent(student);
    Controller::instance()->checkDbError();
}

void StudentsTableView::onUpdateStudent(Student student)
{
    qDebug() << "Updating student: " << student.name() << "ID: " << student.id();
    model->updateStudent(student);
    Controller::instance()->checkDbError();
}

void StudentsTableView::onPasteAction(bool _)
{
    Q_UNUSED(_);
    QClipboard *clipboard = QApplication::clipboard();

    model->addStudentsFromClipboard(ClipBoardParser::parseStudentsClipboard(clipboard->text()));
    Controller::instance()->checkDbError();

}

void StudentsTableView::setupMenu()
{
    menu = new QMenu(this);
    addAction = new QAction("Ajouter");
    editAction = new QAction("Editer");
    removeAction = new QAction("Supprimer");
    pasteAction = new QAction(QIcon(":/icons/images/paste.png"), "Coller");

    menu->addAction(addAction);
    menu->addAction(editAction);
    menu->addAction(removeAction);
    menu->addSeparator();
    menu->addAction(pasteAction);

    QObject::connect(pasteAction, &QAction::triggered, this, &StudentsTableView::onPasteAction);
}
