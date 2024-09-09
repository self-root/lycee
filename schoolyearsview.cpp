#include "schoolyearsview.h"
#include <QMessageBox>

SchoolYearsView::SchoolYearsView(QListView *parent)
    : QListView(parent), model(new SchoolYearsModel)
{
    setModel(model);
    model->loadSchoolYears();
    setupMenu();
}

void SchoolYearsView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)
    if (current.isValid())
        emit currentSchoolYearChanged(model->data(current).toString());
}

void SchoolYearsView::contextMenuEvent(QContextMenuEvent *event)
{
    menu->exec(event->globalPos());
}

void SchoolYearsView::setupMenu()
{
    menu = new QMenu(this);
    deleteMenu = new QAction("Supprimer");
    menu->addAction(deleteMenu);
    QObject::connect(deleteMenu, &QAction::triggered, this, &SchoolYearsView::onDelete);
}

void SchoolYearsView::onDelete(bool triggered)
{
    Q_UNUSED(triggered)
    QModelIndex current = currentIndex();
    if (current.isValid())
    {
        QString selectedScholYear = current.data().toString();
        qDebug() << "Delete schoolyear: " << selectedScholYear;
        int ret = QMessageBox::warning(this,
                             "Supprimer une année scolaire",
                             QString("voulez-vous supprimer l'année scolaire <b>%1</b>?").arg(selectedScholYear),
                             QMessageBox::Ok | QMessageBox::Cancel);

        if (ret == QMessageBox::Ok)
        {
            model->removeSchoolYear(selectedScholYear);
            emit schoolYearDeleted();
        }
    }

}
