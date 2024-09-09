#include "klasslistview.h"
#include "controller.h"
#include <QMessageBox>

KlassListView::KlassListView(QListView *parent) : QListView(parent)
{
    //proxyModel = new QSortFilterProxyModel;
    model = new KlassListModel;
    //proxyModel->setSourceModel(model);
    this->setModel(model);
    setupMenu();
}

void KlassListView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);

    int currentRow = current.row();
    if (currentRow >= 0)
    {
        emit getSubjectListsFor(model->klassAt(currentRow));
    }
}

void KlassListView::contextMenuEvent(QContextMenuEvent *event)
{
    menu->exec(event->globalPos());
}

void KlassListView::setupMenu()
{
    menu = new QMenu(this);
    deleteAction = new QAction("Supprimer");
    menu->addAction(deleteAction);

    QObject::connect(deleteAction, &QAction::triggered, this, &KlassListView::onDelete);
}

void KlassListView::loadKlasses(const QString &schoolYear)
{
    model->loadData(schoolYear);
    Controller::instance()->checkDbError();
}

void KlassListView::addClass(const QString &className)
{
    model->addClass(className);
}

void KlassListView::onDelete(bool triggered)
{
    Q_UNUSED(triggered)

    QModelIndex current = currentIndex();
    if (!current.isValid())
        return;
    Klass currentKlass = model->klassAt(current.row());
    QString klassName = currentKlass.className();
    int ret = QMessageBox::warning(this,
                         "Supprimer une classe",
                         QString("voulez-vous supprimer <b>%1</b> et tous les élèves qui lui sont associés?").arg(klassName),
                         QMessageBox::Ok | QMessageBox::Cancel);

    if (ret == QMessageBox::Ok)
        model->removeKlass(currentKlass);
}

