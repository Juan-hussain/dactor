#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QFileDialog>
#include <QSettings>


namespace Ui {
class settings;
}

class settings : public QDialog
{
    Q_OBJECT

public:
    explicit settings(QWidget *parent = nullptr);
    ~settings();

    QString getSelectedAudioInput();
    QString getSelectedText();

private slots:


    void on_save_accepted();
    void on_browseButton_clicked();
//    void closeEvent(QCloseEvent *event);


private:
    Ui::settings *ui;
    QString selectedText;
};

#endif // SETTINGS_H
