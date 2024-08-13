#include "home.h"
#include "ui_home.h"
#include <QUrl>
#include <QQmlContext>

Home::Home(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Home)
{
    ui->setupUi(this);
    homeController = new HomeController;
    QQmlContext *context = ui->quickWidget->rootContext();
    context->setContextProperty("homeController", homeController);
    ui->quickWidget->setSource(QUrl("qrc:/HomeUi.qml"));
}

Home::~Home()
{
    delete ui;
}
