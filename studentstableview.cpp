#include "studentstableview.h"
#include "controller.h"
#include "clipboardparser.h"
#include "databaseaccess.h"

#include <QDropEvent>
#include <QMimeData>
#include <QClipboard>
#include <QApplication>
#include <QMessageBox>

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
    currentClassID = id;
    model->loadStudents(id);
}

Student StudentsTableView::selectedStudent()
{
    Student student;
    if (currentIndex().row() >= 0)
    {
        const QModelIndex sourceIndex = proxyModel->mapToSource(currentIndex());
        student = model->studentAt(sourceIndex.row());
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
    const bool hasClass = currentClassID > 0;
    addAction->setEnabled(hasClass);
    pasteAction->setEnabled(hasClass);
    editAction->setEnabled(hasClass && currentIndex().isValid());
    removeAction->setEnabled(hasClass && currentIndex().isValid());
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

void StudentsTableView::onDeleteAction(bool triggered)
{
    Q_UNUSED(triggered)
    if (!currentIndex().isValid())
        return;

    int ret = QMessageBox::warning(this, "Effacer un/une élève", QString("Supprimer %1 de la base de données?").arg(selectedStudent().name()), QMessageBox::Button::Cancel | QMessageBox::Button::Ok);
    if (ret == QMessageBox::Button::Ok)
        deleteStudent();
    else {
        return;
    }
}

void StudentsTableView::onAddAction(bool _)
{
    Q_UNUSED(_)
    emit addStudent();
}

void StudentsTableView::onUpdateAction(bool _)
{
    Q_UNUSED(_);
    emit updateStudent();
}

void StudentsTableView::onPasteAction(bool _)
{
    Q_UNUSED(_);
    if (currentClassID < 1)
    {
        QMessageBox::warning(this, "Classe requise", "Selectionnez une classe avant d'importer des eleves.");
        return;
    }
    QClipboard *clipboard = QApplication::clipboard();
    const StudentClipboardParseResult result = ClipBoardParser::parseStudentsClipboard(clipboard->text());
    if (result.students.empty())
    {
        QMessageBox::warning(this, "Import impossible", result.errors.join("\n"));
        return;
    }

    QString preview = QString("Classe: %1\nLignes valides: %2\nLignes ignorees: %3")
                          .arg(DatabaseAccess::instance()->classByID(currentClassID).className())
                          .arg(result.students.size())
                          .arg(result.errors.size());
    if (!result.errors.isEmpty())
        preview += "\n\n" + result.errors.mid(0, 5).join("\n");
    int ret = QMessageBox::question(this, "Apercu de l'import", preview + "\n\nContinuer ?",
                                    QMessageBox::Ok | QMessageBox::Cancel);
    switch (ret)
    {
    case QMessageBox::Ok:{
        const int inserted = model->addStudentsFromClipboard(result.students);
        Controller::instance()->checkDbError();
        QMessageBox::information(this, "Import termine",
                                 QString("%1 eleve(s) ajoute(s), %2 doublon(s) ignore(s).")
                                     .arg(inserted)
                                     .arg(static_cast<int>(result.students.size()) - inserted));
        break;
    }
    case QMessageBox::Cancel:
    default:
        break;
    }


}

void StudentsTableView::setupMenu()
{
    menu = new QMenu(this);
    addAction = new QAction(QIcon(":/icons/images/add_student.png"), "Ajouter");
    editAction = new QAction(QIcon(":/icons/images/edit-profile.png"), "Editer");
    removeAction = new QAction(QIcon(":/icons/images/delete.png"), "Supprimer");
    pasteAction = new QAction(QIcon(":/icons/images/paste.png"), "Coller");

    menu->addAction(addAction);
    menu->addAction(editAction);
    menu->addAction(removeAction);
    menu->addSeparator();
    menu->addAction(pasteAction);

    QObject::connect(removeAction, &QAction::triggered, this, &StudentsTableView::onDeleteAction);
    QObject::connect(pasteAction, &QAction::triggered, this, &StudentsTableView::onPasteAction);
    QObject::connect(addAction, &QAction::triggered, this, &StudentsTableView::onAddAction);
    QObject::connect(editAction, &QAction::triggered, this, &StudentsTableView::onUpdateAction);
}
