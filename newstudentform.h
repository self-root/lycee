#ifndef NEWSTUDENTFORM_H
#define NEWSTUDENTFORM_H

#include <QWidget>
#include "student.h"

namespace Ui {
class NewStudentForm;
}

class NewStudentForm : public QWidget
{
    Q_OBJECT

public:
    enum Intent{
        Save,
        Update
    };
    explicit NewStudentForm(QWidget *parent = nullptr);
    ~NewStudentForm();

    Intent getIntent() const;
    void setIntent(Intent newIntent);

    Student getEditingStudent() const;
    void setEditingStudent(const Student &newEditingStudent);

    void display(Intent _intent);

private slots:
    void on_saveBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::NewStudentForm *ui;
    Intent intent;
    Student editingStudent;

signals:
    void saveStudent(Student student);
    void updateStudent(Student student);
};

#endif // NEWSTUDENTFORM_H
