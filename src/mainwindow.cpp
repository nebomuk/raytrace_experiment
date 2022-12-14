#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scribblearea.h"

#include <QApplication>
#include <QColorDialog>
#include <QFileDialog>
#include <QImageWriter>
#include <QInputDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QCloseEvent>
#include <QCursor>
#include <QSettings>
#include <QFileInfo>


MainWindow::~MainWindow()
{
    delete ui;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),  ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    this->scribbleArea =  ui->scribbleArea;


    createActions();
    createMenus();

    setWindowTitle(tr("Krita Plugin Experiment"));

    QSettings settings;
   restoreGeometry(settings.value("geometry").toByteArray());
   restoreState(settings.value("windowState").toByteArray());

   connect(scribbleArea,SIGNAL(statusMessage(QString)),this,SLOT(showStatusMessage(QString)));

    //resize(500, 500);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
        event->accept();
    else
        event->ignore();

    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    QMainWindow::closeEvent(event);
}

void MainWindow::open()
{
    if (maybeSave()) {


        QString recent = QSettings().value("recent_file",QString()).toString();


        QString path = QDir::currentPath();
        if(!recent.isNull())
        {
            QFileInfo info(recent);
            path = info.path();
        }

        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open File"), path);
        if (!fileName.isEmpty())
        {
            QSettings().setValue("recent_file",fileName);
            scribbleArea->openImage(fileName);
        }
    }
}

void MainWindow::openRecent()
{
    QString recentFile = QSettings().value("recent_file",QString()).toString();
    if(QFileInfo::exists(recentFile))
    {
        scribbleArea->openImage(recentFile);
    }
}

void MainWindow::save()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void MainWindow::penColor()
{
    QColor newColor = QColorDialog::getColor(scribbleArea->penColor());
    if (newColor.isValid())
        scribbleArea->setPenColor(newColor);
}

void MainWindow::brushColor()
{
    QColor newColor = QColorDialog::getColor(scribbleArea->brushColor());
    if (newColor.isValid())
        scribbleArea->setBrushColor(newColor);
}

void MainWindow::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Scribble"),
                                        tr("Select pen width:"),
                                        scribbleArea->penWidth(),
                                        1, 50, 1, &ok);
    if (ok)
        scribbleArea->setPenWidth(newWidth);
}

void MainWindow::setFillEnabled(bool b)
{
    if(b)
    {
        this->setCursor(QCursor(QPixmap(":ic_bucket_fill"),2,16));
        ui->actionFlood_Fill->setChecked(false);
        scribbleArea->setFloodFillEnabled(false);
    }
    else
    {
        this->setCursor(QCursor());
    }
    scribbleArea->setFillEnabled(b);
}

void MainWindow::setFloodFillEnabled(bool b)
{
    if(b)
    {
        this->setCursor(QCursor(QPixmap(":ic_bucket_fill"),2,16));
        ui->actionFill->setChecked(false);
        scribbleArea->setFillEnabled(false);
    }
    else
    {
        this->setCursor(QCursor());
    }
    scribbleArea->setFloodFillEnabled(b);
}

void MainWindow::openSettings()
{
    preferences_->show();
}

void MainWindow::showStatusMessage(QString str)
{
    ui->statusbar->showMessage(str,5000);
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    openRecentAct = new QAction(tr("Open Recent"), this);
    openRecentAct->setShortcuts(QKeySequence::Open);
    connect(openRecentAct, &QAction::triggered, this, &MainWindow::openRecent);

    const QList<QByteArray> imageFormats = QImageWriter::supportedImageFormats();
    for (const QByteArray &format : imageFormats) {
        QString text = tr("%1...").arg(QString::fromLatin1(format).toUpper());

        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, &QAction::triggered, this, &MainWindow::save);
        saveAsActs.append(action);
    }

    printAct = new QAction(tr("&Print..."), this);
    connect(printAct, &QAction::triggered, scribbleArea, &ScribbleArea::print);

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, &QAction::triggered, this, &MainWindow::close);

    penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, &QAction::triggered, this, &MainWindow::penColor);

    brushColorAct = new QAction(tr("&Brush Color..."), this);
    connect(brushColorAct, &QAction::triggered, this, &MainWindow::brushColor);

    penWidthAct = new QAction(tr("Pen &Width..."), this);
    connect(penWidthAct, &QAction::triggered, this, &MainWindow::penWidth);

    ui->actionClear_Screen->setShortcut(tr("Ctrl+L"));
    connect(ui->actionClear_Screen, &QAction::triggered,
            scribbleArea, &ScribbleArea::clearImage);

    connect(ui->actionClear_DebugDraw,&QAction::triggered,scribbleArea,&ScribbleArea::clearDebugDraw);

    connect(ui->actionFill,&QAction::toggled,this,&MainWindow::setFillEnabled);

    connect(ui->actionFlood_Fill,&QAction::toggled,this,&MainWindow::setFloodFillEnabled);


    connect(ui->actionReset_Zoom,&QAction::triggered,scribbleArea,&ScribbleArea::resetZoom);

    connect(ui->actionPreferences,&QAction::triggered,this,&MainWindow::openSettings);

    // based on https://stackoverflow.com/questions/55930004/keep-qtoolbar-showing-always-all-items
#ifdef Q_OS_ANDROID
    // expand layout (for Android)
    QLayout* lay = ui->toolBar->findChild<QLayout*>();

   connect(this,SIGNAL(expandToolbar(bool)),lay,SLOT(setExpanded(bool)));
   emit expandToolbar(true);

   // install an event fitler that filters out the event that causes the toolbar to close automatically
   ui->toolBar->installEventFilter(this);
#endif

}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    #ifdef Q_OS_ANDROID
    if (obj == ui->toolBar) {
            if (event->type() == QEvent::Leave) {
                // do nothing
                return true;
            } else {
                return false;
            }
        } else {
            return QMainWindow::eventFilter(obj, event);
    }
#else
    return QMainWindow::eventFilter(obj, event);
#endif

}

void MainWindow::createMenus()
{
    preferences_ = new Preferences(this);

    saveAsMenu = new QMenu(tr("&Save As"), this);
    for (QAction *action : qAsConst(saveAsActs))
        saveAsMenu->addAction(action);

    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addAction(openRecentAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(brushColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();

    helpMenu = new QMenu(tr("&Help"), this);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
}

bool MainWindow::maybeSave()
{
    if (scribbleArea->isModified()) {
       QMessageBox::StandardButton ret;
       ret = QMessageBox::warning(this, tr("Scribble"),
                          tr("The image has been modified.\n"
                             "Do you want to save your changes?"),
                          QMessageBox::Save | QMessageBox::Discard
                          | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return saveFile("png");
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(QString::fromLatin1(fileFormat.toUpper()))
                               .arg(QString::fromLatin1(fileFormat)));
    if (fileName.isEmpty())
        return false;
    return scribbleArea->saveImage(fileName, fileFormat.constData());
}

