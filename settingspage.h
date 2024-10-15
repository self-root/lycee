#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>
#include <QTextBrowser>

namespace Ui {
class SettingsPage;
}

class SettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPage(QWidget *parent = nullptr);
    ~SettingsPage();

private slots:
    void on_saveSettingsBtn_clicked();

    void on_bulletinFontCombo_currentFontChanged(const QFont &f);

    void on_bulletinFontSize_valueChanged(int arg1);

    void on_bulletinPadding_valueChanged(int arg1);

    void on_ficheFontConbo_currentFontChanged(const QFont &f);

    void on_ficheFontSize_valueChanged(int arg1);

    void on_fichePaddings_valueChanged(int arg1);

private:
    Ui::SettingsPage *ui;
    void setSettingFormValues();
    void updateBulletinStyleSheet(const QMap<QString, QString> &setting);
    void updateFicheStyleSheet(const QMap<QString, QString> &setting);
    QString cssTemplate = R"(
        *{
            font-family: '%1';
            font-size: %2pt;
        }
        td{
            padding: %3px;
        }
    )";

    QString readTemplate(const QString &filaPath);
    QMap<QString, QString> bulletinFormatSettingFromUi();
    QMap<QString, QString> ficheSettingFromUi();
};

#endif // SETTINGSPAGE_H
