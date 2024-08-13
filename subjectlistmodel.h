#ifndef SUBJECTLISTMODEL_H
#define SUBJECTLISTMODEL_H

#include <QAbstractTableModel>

#include <vector>

#include "subject.h"

class SubjectListModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit SubjectListModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void loadSubjectLists(int klassID);

    void addSubject(const QString &subjectName, int subjectCoef);

    Subject subjectAt(int index) const;

    void updateSubject(const Subject &subject);

    void removeSubject(const Subject &subject);

private:
    std::vector<Subject> subjects;
    int currentClasss = 0;
};

#endif // SUBJECTLISTMODEL_H
