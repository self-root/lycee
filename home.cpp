#include "home.h"
#include "ui_home.h"
#include <QUrl>
#include <QQmlContext>


Home::Home(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Home)
{
    ui->setupUi(this);

    quickWidget = new QQuickWidget;

    quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    QQmlContext *context = quickWidget->rootContext();
    context->setContextProperty("homeController", &homeController);
    quickWidget->setSource(QUrl("qrc:/HomeUi.qml"));
    ui->mainLayout->addWidget(quickWidget);
}

Home::~Home()
{
    delete ui;
}

void Home::showEvent(QShowEvent *event)
{
    /*if (quickWidget == nullptr)
    {
        quickWidget = new QQuickWidget;
        QQmlContext *context = quickWidget->rootContext();
        context->setContextProperty("homeController", &homeController);
        quickWidget->setSource(QUrl("qrc:/HomeUi.qml"));
        quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);


        ui->mainLayout->addWidget(quickWidget);
    }*/
}

void Home::hideEvent(QHideEvent *event)
{
    /*if (quickWidget != nullptr)
    {
        delete quickWidget;
        quickWidget = nullptr;
    }*/
}

void Home::onSchoolYearsChanged()
{
    homeController.onSchoolYearsChanged();
}
