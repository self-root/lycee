#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedLayout>
#include <QPixmap>

#include "home.h"
#include "studentpage.h"
#include "klassespage.h"
#include "notespage.h"
#include "settingspage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //void mouseMoveEvent(QMouseEvent *e) override;
    //void mousePressEvent(QMouseEvent *e) override;
    //void mouseReleaseEvent(QMouseEvent *e) override;

private slots:

    void on_exitButton_clicked();

    void on_homeButton_clicked();

    void on_etudiantsBtn_clicked();

    void onDatabaseError(const QString &errorMessage);

    void on_klasseBtn_clicked();

    void on_notesBtn_clicked();

    void on_settingsButton_clicked();

private:
    Ui::MainWindow *ui = nullptr;
    Home *homePage = nullptr;
    StudentPage *studentPage = nullptr;
    KlassesPage *klassPage = nullptr;
    NotesPage *notePage = nullptr;
    SettingsPage *settingsPage = nullptr;
    QStackedLayout *stackedLayout = nullptr;
    //Controller *controller = nullptr;
    //MemberPage *memberPage = nullptr;
    int previousPos_x;
    int previousPos_y;

    void saveScreenSize();
    void getScreenSize();

};
#endif // MAINWINDOW_H
