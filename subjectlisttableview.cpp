#include "subjectlisttableview.h"
#include "controller.h"
#include <QMessageBox>


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
        const int answer = QMessageBox::warning(
            this,
            "Supprimer la matiere",
            QString("Supprimer '%1' ? Toutes les notes associees seront supprimees. Une sauvegarde sera creee avant l'operation.")
                .arg(subject.subjectName()),
            QMessageBox::Cancel | QMessageBox::Yes,
            QMessageBox::Cancel);
        if (answer == QMessageBox::Yes)
        {
            model->removeSubject(subject);
            Controller::instance()->checkDbError();
        }
    }

}
