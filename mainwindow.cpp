#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "dialog.h"
#include "preferences.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QCoreApplication::setOrganizationName("Future");
    QCoreApplication::setOrganizationDomain("future.com");
    QCoreApplication::setApplicationName("TextImageViewer");
    QString filename = QCoreApplication::applicationDirPath() + "/settings.ini";

    QSettings settings(filename, QSettings::Format::IniFormat,this);
    QMap extractedSettings = loadSettings(settings);
    auto themename = extractedSettings.find("Theme:");

    QString theme = ":/themes/themes/"+ *themename+ ".css";
    QFile style(theme);
    if(style.open(QIODevice::ReadOnly)){
        QString sheetStyle = style.readAll();
        style.close();
        setStyleSheet(sheetStyle);
      }
    else
        qDebug()<<"Closed file";


    QPixmap mainIcon(":/images/slideshow.png");
    setWindowTitle("Notepad and Image Editor");
    setWindowIcon(QIcon(mainIcon));

    //set navigation button names
    ui->notePadNav->setObjectName("notepadBtn");
    ui->imageViewerNav->setObjectName("imageBtn");

    ui->stackedWidget->setCurrentIndex(0);

    activeStyle =  R"(QToolButton{
                border:0px;
                padding:5px;
                background-color:rgb(194, 194, 194);
            }
            QToolButton:hover{
                    background-color:rgb(194, 194, 194);
            }
            QToolButton:pressed{
                background-color: rgb(90, 90, 90);
            })";
    inactiveStyle =  R"(QToolButton{
            border:0px;
            padding:5px;
        }
        QToolButton:hover{
                background-color:rgb(194, 194, 194);
        }
        QToolButton:pressed{
            background-color: rgb(90, 90, 90);
        })";
    ui->notePadNav->setStyleSheet(activeStyle);

    // Navbar animation

    animation.setTargetObject(ui->SideNavBar);
    animation.setPropertyName("minimumWidth");
    animation.setDuration(300);
    animation.setEasingCurve(QEasingCurve::InOutQuart);
    ui->SideNavBar->setMaximumWidth(40);

    // create timer for slide show
    pixMapTimer = new QTimer(this);

    signalSlotConnections();
//    pixMapTimer->start(2000);

   }

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::openFile()
{
    // Get the path to the file
    QString file = QFileDialog::getOpenFileName(this,tr("Open File"),
                                                           "C:/Documents",tr("Text files(*txt)"));

    // Open the file and extract the contents
    QFile filename(file);
    QDir filePath(file);
    filePath.cdUp();

    if(!filename.open(QIODevice::ReadWrite))
    {
        QMessageBox::warning(this,"Warning","Unable to open file",QMessageBox::StandardButton::Abort);
        return;
    }
    QString data = filename.readAll();
    ui->textEdit->setText(data);

}

void MainWindow::saveNote()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                 "/home/jana/untitled.png",
                                 tr("Text files (*.txt)"));
    QFile file(fileName);

    if(!file.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(this,"Warning","Unable to open file",QMessageBox::StandardButton::Abort);
            return;
    }
    QByteArray data = ui->textEdit->toPlainText().toUtf8();
    file.write(data);
    file.close();
}

void MainWindow::expandContract()
{
    static bool open {true};

    if (open) {
        qDebug()<<"Opening. width"<<ui->SideNavBar->minimumWidth();
        defaultSize = ui->QuickButtons->frameSize();
        animation.setStartValue(40);
        animation.setEndValue(100);
        animation.start();
        open = false;
        QIcon close(":/images/left.png");
        ui->resizeNavBtn->setIcon(close);
    }
    else{
        animation.setStartValue(100);
        animation.setEndValue(40);
        animation.start();
        qDebug()<<"closing. width"<<ui->SideNavBar->minimumWidth();
        QIcon close(":/images/swipe-right.png");
        ui->resizeNavBtn->setIcon(close);
        open = true;
    }


}

void MainWindow::showModalView()
{
    Dialog *fileShow = new Dialog(this);
    fileShow->exec();
}

void MainWindow::showPreferences()
{
    Preferences *preferencesShow = new Preferences(this);
    int ret = preferencesShow->exec();
    if(ret==QDialog::Accepted)
    {
        //extract the settings and apply to application
        QString filename = ":/themes/themes/"+preferencesShow->getTheme()+".css";

        QFile style(filename);
        if(style.open(QIODevice::ReadOnly)){
            QString sheetStyle = style.readAll();
            style.close();
            setStyleSheet(sheetStyle);

            //settings values to save
            QString filename = QCoreApplication::applicationDirPath() + "/settings.ini";

            QSettings settings(filename, QSettings::Format::IniFormat,this);
            saveSettings(settings, preferencesShow->getFontName(),preferencesShow->getFontSize(),
                         preferencesShow->getTheme(),"John Stanley",true);
        }
        else
            qDebug()<<"Closed file";
    }
}

QMap<QString, QString> MainWindow::loadSettings(QSettings &settingsName)
{
    // create container to store settings
   QMap<QString, QString> returnValue;

   // retrieve the keys
    QStringList keys=settingsName.allKeys();
    foreach(auto key,keys)
        returnValue[key] = settingsName.value(key).toString();

    qDebug()<<returnValue;
    return  returnValue;
}

void MainWindow::saveSettings(QSettings &settingsName, QString font, QString fSize, QString theme, QString username, bool first)
{
    settingsName.setValue("User:",QVariant(username));
    settingsName.setValue("Font:",QVariant(font));
    settingsName.setValue("FontSize:",QVariant(fSize));
    settingsName.setValue("Theme:",QVariant(theme));
    int result = settingsName.status();
    qInfo()<<username<<font<<fSize<<theme;
    qInfo()<<settingsName.fileName();
    if (result ==QSettings::NoError && !first )
    {
        QMessageBox::information(this,"Success","Saved Settings");
        if (result !=QSettings::NoError)
            QMessageBox::warning(this,"Failure","Failed to Save Settings");
    }
}


void MainWindow::signalSlotConnections()
{
    connect(ui->actionOpenFolder, &QAction::triggered, this, [=](){
        QString folderPath = QFileDialog::getExistingDirectory(this,tr("Open Folder"),
                                                               "C:/Documents",QFileDialog::ShowDirsOnly);
        QDir path(folderPath);
//        populateFileTree(path);
    });

    //Open file and display in text editor
    connect(ui->actionOpen_file,&QAction::triggered, this, &MainWindow::openFile);

    //Save file
    connect(ui->actionSave_Note, &QAction::triggered, this, &MainWindow::saveNote);

    // Text controls - cut,copy, paste, redo, undo
    //QTextEdit Controls - Copy,Cut,Paste and undo
    connect(ui->actionCut,&QAction::triggered,this,[=](){
        ui->textEdit->cut();
    });
    connect(ui->actionPaste,&QAction::triggered,this,[=](){
        ui->textEdit->paste();
    });
    connect(ui->actionUndo,&QAction::triggered,this,[=](){
        ui->textEdit->undo();
    });
    connect(ui->actionCopy,&QAction::triggered,this,[=](){
        ui->textEdit->copy();
    });
    connect(ui->actionRedo,&QAction::triggered,this,[=](){
        ui->textEdit->redo();
    });

    // Quit Application
    connect(ui->actionQuit,&QAction::triggered,this, [=](){
        int ret=QMessageBox::information(this,"Close file","Are you sure?",
                                 QMessageBox::StandardButton::Cancel|QMessageBox::StandardButton::Ok
                                 ,QMessageBox::StandardButton::Cancel);


        if (ret==QMessageBox::Cancel) return;
        if (ret==QMessageBox::Ok) QApplication::quit();
    });

    //Help pop ups
    connect(ui->actionAbout,&QAction::triggered,this,[=](){
        QMessageBox::about(this, "About Editor",
                           "This is a simple image and text viewer created to illustrate"
                           "Several aspects of qt's animation and widget featutres");
    });

    connect(ui->actionAbout_Qt,&QAction::triggered,this, [=](){QApplication::aboutQt();});

    //Animation
    connect(ui->resizeNavBtn, &QToolButton::clicked, this, &MainWindow::expandContract);

    //show modal windows
    connect(ui->actionOpen_view_dialog, &QAction::triggered, this, &MainWindow::showModalView);
    connect(ui->preferencesBtn, &QPushButton::clicked, this, &MainWindow::showPreferences);
    connect(ui->actionPreferences, &QAction::triggered,this, &MainWindow::showPreferences);

    //Change stacked widget - notepad
    connect(ui->notePadNav,&QPushButton::clicked,this, [=](){
        ui->stackedWidget->setCurrentIndex(0);
        setActiveButton();
    });

    //Change stacked widget - notepad
    connect(ui->imageViewerNav,&QPushButton::clicked,this, [=](){
        ui->stackedWidget->setCurrentIndex(1);
        setActiveButton();
    });

    //Slideshow
    connect(pixMapTimer,&QTimer::timeout,this, [=](){
        //get the list of files in the resource files
        QList<QString> images;
        static int counter{0};
        images = QDir(":/slideshowImages/slideshow/").entryList();
        QPixmap image(":/slideshowImages/slideshow/" + images[counter]);

        ui->pictureLabel->setPixmap(image);
        (counter>=images.length()-1) ? counter=0 :counter++;
    });

    //slideshow button
    connect(ui->slideshowBtn, &QPushButton::clicked,this,[=](){
        static bool onSlideShowMode {false};
        if (!onSlideShowMode){
            onSlideShowMode = true;
            pixMapTimer->start(3000);

        }
        else{
            onSlideShowMode =false;
            pixMapTimer->stop();
        }
    });

    //slideshow next button
    connect(ui->nextImageBtn,&QPushButton::clicked,this,[=](){
        //get the list of files in the resource files
        QList<QString> images;
        static int counter{0};
        images = QDir(":/slideshowImages/slideshow/").entryList();
        QPixmap image(":/slideshowImages/slideshow/" + images[counter]);

        ui->pictureLabel->setPixmap(image);
        (counter>=images.length()-1) ? counter=0 :counter++;
    });
    connect(ui->prevImageBtn,&QPushButton::clicked,this,[=](){
        //get the list of files in the resource files
        QList<QString> images;
        static int counter{0};
        images = QDir(":/slideshowImages/slideshow/").entryList();
        QPixmap image(":/slideshowImages/slideshow/" + images[counter]);

        ui->pictureLabel->setPixmap(image);
        (counter<=0) ? counter=images.length()-1 :counter--;
    });

}

void MainWindow::slideShowTimer()
{
    // create timer for slide show
    pixMapTimer = new QTimer(this);

    signalSlotConnections();
    pixMapTimer->start(2000);
}

void MainWindow::setActiveButton()
{
    if (sender()->objectName()=="notepadBtn"){
        active = true;
        ui->notePadNav->setStyleSheet(activeStyle);
        ui->imageViewerNav->setStyleSheet(inactiveStyle);
    }
    if (sender()->objectName()=="imageBtn"){
        active = false;
        ui->notePadNav->setStyleSheet(inactiveStyle);
        ui->imageViewerNav->setStyleSheet(activeStyle);
    }
}


