#include "settingspage.h"
#include "ui_settingspage.h"
#include <QMap>
#include "controller.h"

SettingsPage::SettingsPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingsPage)
{
    ui->setupUi(this);
    setSettingFormValues();
}

SettingsPage::~SettingsPage()
{
    delete ui;
}

void SettingsPage::on_saveSettingsBtn_clicked()
{
    QMap<QString, QString> settings;
    settings["principal"] = ui->principalEdit->text();
    settings["school_name"] = ui->schoolNameEdit->text();
    settings["code"] = ui->codeEdit->text();
    settings["place"] = ui->schoolLocationEdit->text();

    Controller::instance()->saveSchoolSettings(settings);
}

void SettingsPage::setSettingFormValues()
{
    QMap<QString, QString> schoolSettings = Controller::instance()->getSchoolSettings();
    ui->principalEdit->setText(schoolSettings.value("principal"));
    ui->schoolNameEdit->setText(schoolSettings.value("school_name"));
    ui->codeEdit->setText(schoolSettings.value("code"));
    ui->schoolLocationEdit->setText(schoolSettings.value("place"));
}

