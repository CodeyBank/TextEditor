#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#include <QDir>
#include <QFileInfoList>
#include <QDirIterator>
#include <QMessageBox>

namespace Ui {
class Preferences;
}

class Preferences : public QDialog
{
    Q_OBJECT

public:
    explicit Preferences(QWidget *parent = nullptr);
    ~Preferences();
    QString getTheme() const;
    void setTheme(const QString &newTheme);

    QString getFontSize() const;
    void setFontSize(const QString &newFontSize);

    QString getFontName() const;
    void setFontName(const QString &newFontName);

private slots:
    void apply();
    void cancel();

private:
    Ui::Preferences *ui;
    QString user, fontName, fontSize, theme;
    void populateThemeList();
};

#endif // PREFERENCES_H
