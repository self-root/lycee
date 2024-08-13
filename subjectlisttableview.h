#ifndef SUBJECTLISTTABLEVIEW_H
#define SUBJECTLISTTABLEVIEW_H

#include <QTableView>

#include "klass.h"

#include "subjectlistmodel.h"

class SubjectListTableView : public QTableView
{
    Q_OBJECT
public:
    SubjectListTableView(QTableView *parent=nullptr);

public slots:
    void onGetSubjectsFor(const Klass &klass);

    void addSubject(const QString &subjectName, int subjectCoef);

private:
    SubjectListModel *model = nullptr;
};

#endif // SUBJECTLISTTABLEVIEW_H
