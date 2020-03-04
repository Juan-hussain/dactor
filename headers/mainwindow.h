#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextBrowser>
#include <QKeyEvent>
#include <QFile>
#include <QTextStream>
#include <QtGlobal>
#include <QLineEdit>
#include <QTextEdit>
#include <QScrollBar>
#include <QCursor>
#include <QAudioRecorder>
#include <QTime>
#include <QMessageBox>
#include <QDir>
#include <QSettings>
#include <QTimer>
#include <QRegExp>
#include <QStateMachine>
#include<QLoggingCategory>

#include "speakerprofileform.h"
#include "loginform.h"
#include "settings.h"
#include "audiorecorder.h"
#include "stmmanager.h"
#include "recstatemachine.h"
#include "options.h"
#include "user.h"
#include "userRecords.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //QStateMachine* machine1;
    RecStateMachine *recStateMachine;
    QString txtFileName;

private slots:
    void on_text_plus_clicked();

    void on_text_minus_clicked();

    void updateTextSize(int value);

    void on_settings_clicked();
    void closeEvent( QCloseEvent *event);

    void initWindow(User* user,QString path_to_text);

    void on_add_user_clicked();

    User* log_in(bool isInitial);

    bool end_of_table(bool look_upwards);
    void key_space_pressed();
    void key_enter_pressed();

private:
    Ui::MainWindow *ui;
    User* user;
    int last_row;
    QShortcut *key_right;
    QShortcut *key_down;
    QShortcut *key_left;
    QShortcut *key_up;
    QShortcut *key_space;
    QShortcut *key_enter;

    // define the minimal and maximal allowed font sizes
    const int min_font_size = 10;
    const int max_font_size = 34;
    QFont tableFont;
    // define settings file
    //QSettings *savedSettings;

    // define an STMManager
    StmManager* stm;
    LogInForm *logInForm;
    UserRecords* userRecords;

//    bool recOddClick = true;
    bool playOddClick = true;
    void prepareTable();
    void postpareTable();
    QString select_text();
    QString init_stm(QString path_to_text);

public:

    //static QAudioRecorder *getRecorder();
    static bool isInitial();

signals:
//    void record_button_clicked();
private slots:

    void key_next();
    void key_previous();
//    void on_rec_clicked();
    void on_play_clicked();
    void on_ltr_clicked();
    void on_rtl_clicked();
};


#endif // MAINWINDOW_H
