#include "subjectlisttableview.h"
#include "controller.h"


SubjectListTableView::SubjectListTableView(QTableView *parent) : QTableView(parent)
{
    model = new SubjectListModel;
    setModel(model);
}

void SubjectListTableView::onGetSubjectsFor(const Klass &klass)
{
    //Call subjectList model to load subject lists
    qDebug() << "Notifying model to get subjects for the class: " << klass.className();

    model->loadSubjectLists(klass.classId());
    Controller::instance()->checkDbError();
}

void SubjectListTableView::addSubject(const QString &subjectName, int subjectCoef)
{
    model->addSubject(subjectName, subjectCoef);
    Controller::instance()->checkDbError();
}
