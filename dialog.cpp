#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //tree view setting
    fileView->setParent(ui->documentView);
    fileView->setReadOnly(false);
    fileView->setFilter(QDir::NoDotAndDotDot|QDir::NoSymLinks|QDir::Files);

    fileView->setRootPath(QDir::currentPath());
    ui->documentView->setModel(fileView);
    ui->documentView->setRootIndex(fileView->index(QDir::currentPath()));
    ui->documentView->resizeColumnToContents(0);

    // folder view
    folderView->setParent(ui->folderView);
    folderView->setReadOnly(false);
    folderView->setFilter(QDir::NoDotAndDotDot|QDir::Dirs);

    folderView->setRootPath(QDir::currentPath());
    ui->folderView->setModel(folderView);
    ui->folderView->setRootIndex(folderView->index(QDir::currentPath()));
    ui->folderView->resizeColumnToContents(1);
}

Dialog::~Dialog()
{
    delete ui;
}
