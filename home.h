#ifndef HOME_H
#define HOME_H

#include <QWidget>

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

private:
    Ui::Home *ui;
    HomeController *homeController = nullptr;
};

#endif // HOME_H
