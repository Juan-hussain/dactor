#include "../headers/loginform.h"
#include "ui_loginform.h"

LogInForm::LogInForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogInForm)
{
    ui->setupUi(this);
    this->setWindowTitle("Log in");
}

LogInForm::~LogInForm()
{
    delete ui;
}

void LogInForm::setCloseIfCancel(bool value)
{
    closeIfCancel = value;
}
//here we get the user object made by his name
//the environment is prepared each time the user log in
void LogInForm::on_login_btn_clicked()
{
    QString new_user_name = ui->usernameLineEdit->text();
    if (new_user_name == ""){
        QMessageBox::warning(this, tr("Warning"), QString::fromStdString("username pls!"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    username = new_user_name;
    //here we get the user object made by his name
    //the environment is prepared each time the user log in
    user = userRecords->getUser(username);
    if (user != nullptr) {
        closeIfCancel = false;
        this->close();
    } else {
        QString error = "This username is has no account please check spelling or register: \n\n";
        QMessageBox::warning(this, tr("Warning"), error,
                             QMessageBox::Ok, QMessageBox::Ok);
    }
}


void LogInForm::on_register_btn_clicked()
{
    speakerProfileForm speakerForm(this);
    speakerForm.setModal(true);
    speakerForm.exec();
}

void LogInForm::closeEvent(QCloseEvent *event)
{
    if (!closeIfCancel)
    {
        this->close();
    } else {
        // if user decides to cancel a warning message is shown
        int ret = QMessageBox::warning(this, tr("Warning"),options::WARNING_CLOSE_IF_CANCEL,
                                       QMessageBox::Cancel | QMessageBox::Close, QMessageBox::Cancel);
        // switch the clicked button
        switch (ret)
        {
        // if the user clicks close the program will close
        case QMessageBox::Close:
            // destroy the program
            exit(EXIT_FAILURE);
            // else just close the warning
        default:
            qDebug()<<"break state";
            event->ignore();
        }
    }
}

User* LogInForm::getUser() const
{
    return user;
}



