#include "preferences.h"
#include "ui_preferences.h"

Preferences::Preferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preferences)
{
    ui->setupUi(this);
    QPixmap icon(":/images/preference.png");
    this->setWindowIcon(QIcon(icon));
    setWindowTitle("Preference setting");

    //signal slot connections
    connect(ui->applyBtn,&QPushButton::clicked,this,&Preferences::apply);
    connect(ui->cancelBtn,&QPushButton::clicked,this,&Preferences::cancel);
    populateThemeList();
}

Preferences::~Preferences()
{
    delete ui;
}

void Preferences::apply()
{
    fontName = ui->fontComboBox_2->currentFont().family();
    fontSize = ui->comboBox_5->currentText();
    theme = ui->themeList->currentItem()->text();
    accept();
}

QString Preferences::getFontName() const
{
    return fontName;
}

void Preferences::setFontName(const QString &newFontName)
{
    fontName = newFontName;
}

QString Preferences::getFontSize() const
{
    return fontSize;
}

void Preferences::setFontSize(const QString &newFontSize)
{
    fontSize = newFontSize;
}

QString Preferences::getTheme() const
{
    return theme;
}

void Preferences::setTheme(const QString &newTheme)
{
    theme = newTheme;
}

void Preferences::cancel()
{
    QMessageBox::warning(this,"Failed","No settings applied");
    reject();
}

void Preferences::populateThemeList()
{
    // Define the root folder
    QDir root(":/themes/themes");

    foreach(QFileInfo fi, root.entryInfoList(QDir::Files))
    {
        qInfo()<<fi.baseName();
        ui->themeList->addItem(fi.baseName());
    }

    //set a default index(theme)
    ui->themeList->setCurrentRow(0);

}
