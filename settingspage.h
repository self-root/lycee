#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>

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

private:
    Ui::SettingsPage *ui;
    void setSettingFormValues();
};

#endif // SETTINGSPAGE_H
