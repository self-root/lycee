#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QMouseEvent>
#include <QSettings>
#include <QMessageBox>
#include "controller.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    Controller::instance()->init();
    //this->setMouseTracking(true);
    QObject::connect(Controller::instance(), &Controller::databaseError, this, &MainWindow::onDatabaseError);
    //this->setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
    setWindowTitle("Lycee");
    QPixmap img = QPixmap(":/icons/images/whale_letter2.png");
    img = img.scaledToWidth(250);
    ui->imgLabel->setPixmap(img);
    this->setCentralWidget(ui->horizontalLayoutWidget);
    homePage = new Home;
    studentPage = new StudentPage;
    klassPage = new KlassesPage;
    notePage = new NotesPage;
    settingsPage = new SettingsPage;
    stackedLayout = new QStackedLayout;
    stackedLayout->addWidget(homePage);
    stackedLayout->addWidget(studentPage);
    stackedLayout->addWidget(klassPage);
    stackedLayout->addWidget(notePage);
    stackedLayout->addWidget(settingsPage);
    ui->mainWidget->setLayout(stackedLayout);
    getScreenSize();

    QObject::connect(klassPage, &KlassesPage::schoolYearAded, studentPage, &StudentPage::onSchoolYearAdded);
    QObject::connect(klassPage, &KlassesPage::schoolYearAded, notePage, &NotesPage::loadSchoolYear);
    QObject::connect(klassPage, &KlassesPage::schoolYearRemoved, studentPage, &StudentPage::onSchoolYearAdded);
    QObject::connect(klassPage, &KlassesPage::schoolYearRemoved, notePage, &NotesPage::loadSchoolYear);
    QObject::connect(klassPage, &KlassesPage::schoolYearRemoved, homePage, &Home::onSchoolYearsChanged);
    QObject::connect(klassPage, &KlassesPage::schoolYearAded, homePage, &Home::onSchoolYearsChanged);
}

MainWindow::~MainWindow()
{
    saveScreenSize();
    delete ui;
}

/*void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    //setCursor(Qt::SizeAllCursor);
    if (previousPos_y < 20)
    {
        //QCursor curs(Qt::SizeAllCursor);
        //setCursor(curs);
        move(e->globalPosition().x() - previousPos_x, e->globalPosition().y() - previousPos_y);
    }
    //qDebug() << "Mouse";
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    previousPos_x = e->position().x();
    previousPos_y = e->position().y();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->position().x() < 20 && e->button() == Qt::LeftButton)
        setCursor(Qt::ArrowCursor);
}*/

void MainWindow::on_exitButton_clicked()
{
    this->close();
}

void MainWindow::saveScreenSize()
{
    QSettings setting;
    setting.beginGroup("screenSize");
    setting.setValue("size", this->geometry());
    setting.endGroup();
}

void MainWindow::getScreenSize()
{
    QSettings setting;
    QRect screenSize;
    setting.beginGroup("screenSize");
    screenSize  = setting.value("size", QRect(50, 50, 800, 600)).toRect();
    this->setGeometry(screenSize);
    setting.endGroup();
}

void MainWindow::on_homeButton_clicked()
{
    if (stackedLayout->currentIndex() != 0)
        stackedLayout->setCurrentIndex(0);
}


void MainWindow::on_etudiantsBtn_clicked()
{
    if (stackedLayout->currentIndex() != 1)
        stackedLayout->setCurrentIndex(1);
}

void MainWindow::onDatabaseError(const QString &errorMessage)
{
    QMessageBox::warning(this, "Database error", errorMessage);
}


void MainWindow::on_klasseBtn_clicked()
{
    if (stackedLayout->currentIndex() != 2)
        stackedLayout->setCurrentIndex(2);
}


void MainWindow::on_notesBtn_clicked()
{
    if (stackedLayout->currentIndex() != 3)
        stackedLayout->setCurrentIndex(3);
}


void MainWindow::on_settingsButton_clicked()
{
    if (stackedLayout->currentIndex() != 4)
        stackedLayout->setCurrentIndex(4);
}

