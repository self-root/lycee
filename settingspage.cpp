#include "settingspage.h"
#include "ui_settingspage.h"
#include <QMap>
#include "controller.h"
#include <QFont>
#include <QFile>

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

    Controller::instance()->saveTranscriptFormatSettings(bulletinFormatSettingFromUi());

    Controller::instance()->saveTotalizationFormatSettings(ficheSettingFromUi());
}

void SettingsPage::setSettingFormValues()
{
    QMap<QString, QString> schoolSettings = Controller::instance()->getSchoolSettings();
    ui->principalEdit->setText(schoolSettings.value("principal"));
    ui->schoolNameEdit->setText(schoolSettings.value("school_name"));
    ui->codeEdit->setText(schoolSettings.value("code"));
    ui->schoolLocationEdit->setText(schoolSettings.value("place"));

    QMap<QString, QString> transcriptSetting = Controller::instance()->getTranscriptFormatSettings();
    ui->bulletinFontCombo->setCurrentFont(QFont(transcriptSetting.value("font")));
    ui->bulletinFontSize->setValue(transcriptSetting.value("fontsize").toInt());
    ui->bulletinPadding->setValue(transcriptSetting.value("paddings").toInt());
    updateBulletinStyleSheet(transcriptSetting);

    QMap<QString, QString> totalizationSetting = Controller::instance()->getTotalizationFormatSettings();
    ui->ficheFontConbo->setCurrentFont(QFont(totalizationSetting.value("font")));
    ui->ficheFontSize->setValue(totalizationSetting.value("fontsize").toInt());
    ui->fichePaddings->setValue(totalizationSetting.value("paddings").toInt());
}

void SettingsPage::updateBulletinStyleSheet(const QMap<QString, QString> &setting)
{
    ui->bulletinSampleText->document()->setDefaultStyleSheet(
        cssTemplate
            .arg(setting.value("font"))
            .arg(setting.value("fontsize"))
            .arg(setting.value("paddings"))
        );

    ui->bulletinSampleText->document()->setHtml(readTemplate(":/html/html/bulletin.html"));

}

void SettingsPage::updateFicheStyleSheet(const QMap<QString, QString> &setting)
{
    ui->ficheSampleText->document()->setDefaultStyleSheet(
        cssTemplate
            .arg(setting.value("font"))
            .arg(setting.value("fontsize"))
            .arg(setting.value("paddings"))
        );

    ui->ficheSampleText->document()->setHtml(readTemplate(":/html/html/fiche.html"));
}

QString SettingsPage::readTemplate(const QString &filaPath)
{
    QFile file(filaPath);
    if (file.open(QIODevice::ReadOnly))
    {
        return QString(file.readAll());
    }
    return QString();
}

QMap<QString, QString> SettingsPage::bulletinFormatSettingFromUi()
{
    QMap<QString, QString> bulletinSettings;
    bulletinSettings["font"] = ui->bulletinFontCombo->currentFont().family();
    bulletinSettings["fontsize"] = QString::number(ui->bulletinFontSize->value());
    bulletinSettings["paddings"] = QString::number(ui->bulletinPadding->value());
    return bulletinSettings;
}

QMap<QString, QString> SettingsPage::ficheSettingFromUi()
{
    QMap<QString, QString> totalizationSettings;
    totalizationSettings["font"] = ui->ficheFontConbo->currentFont().family();
    totalizationSettings["fontsize"] = QString::number(ui->ficheFontSize->value());
    totalizationSettings["paddings"] = QString::number(ui->fichePaddings->value());
    return totalizationSettings;
}



void SettingsPage::on_bulletinFontCombo_currentFontChanged(const QFont &f)
{
    Q_UNUSED(f)
    updateBulletinStyleSheet(bulletinFormatSettingFromUi());
    //qDebug() << "CSS Updated. FOnt: " << bulletinSettings["font"];
}


void SettingsPage::on_bulletinFontSize_valueChanged(int arg1)
{
    Q_UNUSED(arg1)
    updateBulletinStyleSheet(bulletinFormatSettingFromUi());
}


void SettingsPage::on_bulletinPadding_valueChanged(int arg1)
{
    Q_UNUSED(arg1)
    updateBulletinStyleSheet(bulletinFormatSettingFromUi());
}


void SettingsPage::on_ficheFontConbo_currentFontChanged(const QFont &f)
{
    Q_UNUSED(f)
    updateFicheStyleSheet(ficheSettingFromUi());
}


void SettingsPage::on_ficheFontSize_valueChanged(int arg1)
{
    Q_UNUSED(arg1)
    updateFicheStyleSheet(ficheSettingFromUi());
}


void SettingsPage::on_fichePaddings_valueChanged(int arg1)
{
    Q_UNUSED(arg1)
    updateFicheStyleSheet(ficheSettingFromUi());
}

