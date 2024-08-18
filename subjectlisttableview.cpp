#include "subjectlisttableview.h"
#include "controller.h"


SubjectListTableView::SubjectListTableView(QTableView *parent) : QTableView(parent)
{
    setSelectionBehavior(QTableView::SelectRows);
    model = new SubjectListModel;
    setModel(model);
    contextMenu = new QMenu;
    contextMenu->addAction("Supprimer", this, &SubjectListTableView::onDeleteAction);
}

void SubjectListTableView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)
    if (!current.isValid())
        return;

    emit selectedSubject(model->subjectAt(current.row()));
}

void SubjectListTableView::contextMenuEvent(QContextMenuEvent *event)
{
    contextMenu->exec(event->globalPos());
}

void SubjectListTableView::onGetSubjectsFor(const Klass &klass)
{
    model->loadSubjectLists(klass.classId());
    Controller::instance()->checkDbError();
}

void SubjectListTableView::addSubject(const QString &subjectName, int subjectCoef)
{
    model->addSubject(subjectName, subjectCoef);
    Controller::instance()->checkDbError();
}

void SubjectListTableView::onDeleteAction()
{
    qDebug() << "Delete: " << currentIndex();
    if (currentIndex().isValid())
    {
        Subject subject = model->subjectAt(currentIndex().row());
        model->removeSubject(subject);
    }

}
