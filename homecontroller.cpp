#include "homecontroller.h"

HomeController::HomeController(QObject *parent)
    : QObject{parent}
{}

QString HomeController::getSchoolName() const
{
    return schoolName;
}
