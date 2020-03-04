#ifndef SPEAKERPROFILEFORM_H
#define SPEAKERPROFILEFORM_H

#include <QMessageBox>
//#include <QTextStream>
#include <QStandardPaths>
#include <QDate>
#include <QFile>
//#include <iostream>
#include<QDir>
#include<QDebug>
#include <QTime>
#include"options.h"
#include"userRecords.h"
//#include "mainwindow.h"
using namespace std;

namespace Ui {
class speakerProfileForm;
}

class speakerProfileForm : public QDialog
{
    Q_OBJECT

public:
   // explicit speakerProfileForm(QWidget *parent = nullptr);
    explicit speakerProfileForm(QWidget *parent = nullptr); //IMPLEMENTIEREN HIER MUSS USER LISTE GEHOLT WERDEN
    ~speakerProfileForm();

private slots:
    void on_save_pressed();
    void on_cancel_clicked();

private:
    bool isFilled = false;
    const QString PATH_SEP = QDir::separator();
    Ui::speakerProfileForm *ui;
    QStringList registeredUsers;
    QString userWorkingDir;
    QString userProfile;
    UserRecords* userRecords = UserRecords::getInstance();
    bool isValidGender();
    bool isValidCountry();
    bool isValidEducation();
    bool isValidUsername(QString newUsername);
    bool validateParameters();
    bool is_available(QString newName);

 //   static size_t str_hash;



};

#endif //SPEAKERPROFILEFORM_H
