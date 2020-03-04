#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>
#include <QDir>
#include <QMessageBox>
#include <QCloseEvent>
#include "settings.h"
#include "userRecords.h"
#include "util.h"
#include "speakerprofileform.h"
#include "options.h"
#include "user.h"

namespace Ui {
class LogInForm;
}

class LogInForm : public QDialog
{
    Q_OBJECT

public:
    explicit LogInForm(QWidget *parent = nullptr);
    ~LogInForm();

    void setCloseIfCancel(bool value);

    User* getUser() const;
    void setUser(const User &value);

private slots:

    void on_login_btn_clicked();
    void on_register_btn_clicked();
    void closeEvent(QCloseEvent *event);



private:
    UserRecords* userRecords = UserRecords::getInstance();
    Ui::LogInForm *ui;
    int user_index;
    QString stmFile;
    bool closeIfCancel;
    QString username;
    QString userWorkingDir;
    User *user = nullptr;

    void select_text();
    void assignUserStm(int user_index,QString stmFilename);

};

#endif // LOGINFORM_H
