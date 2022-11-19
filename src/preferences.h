#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>

namespace Ui {
class Preferences;
}

class Preferences : public QDialog
{
    Q_OBJECT

public:
    explicit Preferences(QWidget *parent = nullptr);
    ~Preferences();

private:
    Ui::Preferences *ui;
    void loadFromSettings();

private slots:
    void writeSettings();

private slots:
    void resetSettingsToDefaults();
};

#endif // PREFERENCES_H
