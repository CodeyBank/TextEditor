#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QFile>
#include <QIODevice>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QSettings>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void populateFileTree(QDir &rootFolder);

public slots:
    void openFile();
    void saveNote();
//    void saveAsPDF();
    void expandContract();
    void showModalView();

private:
    Ui::MainWindow *ui;
    QTimer *pixMapTimer;

    void signalSlotConnections();
    void slideShowTimer();
    QPropertyAnimation animation ;
    QSize defaultSize;
    bool active{false};
    bool firstLoadSettings {false};
    void setActiveButton();

    QString inactiveStyle;
    QString activeStyle;
    void showPreferences();
    QMap<QString, QString> loadSettings(QSettings &settingsName);
    void saveSettings(QSettings &settingsName,  QString font,
                      QString fSize, QString theme, QString username="John Doe", bool first=true);
};



#endif // MAINWINDOW_H
