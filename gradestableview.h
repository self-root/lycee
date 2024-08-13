#ifndef GRADESTABLEVIEW_H
#define GRADESTABLEVIEW_H

#include <QTableView>
#include <QSortFilterProxyModel>

#include "gradelistmodel.h"


class GradesTableView : public QTableView
{
    Q_OBJECT
public:
    GradesTableView(QTableView *parent=nullptr);

    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

    GradeListModel *model = nullptr;

public slots:
    void loadStudentGrades(int klassID, int trimestre);

private:
    QSortFilterProxyModel *proxyModel = nullptr;


signals:
    void studentSelected(const Student &student);
};

#endif // GRADESTABLEVIEW_H
