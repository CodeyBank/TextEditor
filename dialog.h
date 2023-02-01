#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileSystemModel>
#include <QDir>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;


    QFileSystemModel *folderView= new QFileSystemModel(this);
    QFileSystemModel *fileView= new QFileSystemModel(this);

};

#endif // DIALOG_H
