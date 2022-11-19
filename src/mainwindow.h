#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "scribblearea.h"
#include <QMainWindow>
#include "preferences.h"

namespace Ui {
class MainWindow;
}

#include <QList>
#include <QMainWindow>

class ScribbleArea;

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    bool openQImage(const QImage & inputImage) { return scribbleArea->openQImage(inputImage);}

    virtual ~MainWindow();

signals:
    void expandToolbar(bool);


protected:
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void open();
    void openRecent();
    void save();
    void penColor();
    void brushColor();
    void penWidth();
    void setFillEnabled(bool b);
    void setFloodFillEnabled(bool b);
    void openSettings();
    void showStatusMessage(QString str);

private:
    void createActions();
    void createMenus();
    bool maybeSave();
    bool saveFile(const QByteArray &fileFormat);

    Ui::MainWindow *ui;


    ScribbleArea *scribbleArea;

    Preferences * preferences_;

    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QMenu *helpMenu;

    QAction *openAct;
    QAction *openRecentAct;
    QList<QAction *> saveAsActs;
    QAction *exitAct;
    QAction *penColorAct;
     QAction *brushColorAct;
    QAction *penWidthAct;
    QAction *printAct;

};
//! [0]

#endif // MAINWINDOW_H

