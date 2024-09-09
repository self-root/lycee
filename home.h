#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QQuickWidget>
#include <QShowEvent>
#include <QHideEvent>

#include "homecontroller.h"

namespace Ui {
class Home;
}

class Home : public QWidget
{
    Q_OBJECT

public:
    explicit Home(QWidget *parent = nullptr);
    ~Home();

    void showEvent(QShowEvent *event) override;

    void hideEvent(QHideEvent *event) override;

public slots:
    void onSchoolYearsChanged();

private:
    Ui::Home *ui;
    HomeController homeController;
    QQuickWidget *quickWidget = nullptr;

};

#endif // HOME_H
