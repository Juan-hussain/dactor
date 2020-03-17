#include "../headers/speakerprofileform.h"
#include "ui_speakerprofileform.h"

speakerProfileForm::speakerProfileForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::speakerProfileForm)
{
    ui->setupUi(this);
    setFixedSize(600,400);
    this->setWindowTitle("Add User");
}

speakerProfileForm::~speakerProfileForm()
{
    delete ui;
}

void speakerProfileForm::on_save_pressed()
{
    if (!validateParameters())
    {
        return;
    }
    QString newUsername = ui->usernameLineEdit->text();
    if (!userRecords->addUser(newUsername))
        qDebug() << "speakerProfileForm: could not add the user"<< newUsername;
        // close the form window
        this->close();
}
void speakerProfileForm::on_cancel_clicked()
{
    // destinguish between creating a profile for the first time and adding a new profile
    // if no profile exists the user can't use the program until he successfully finishes
    // creating a profile
    if (false)//(MainWindow::isInitial())
    {
        // if user decides to cancel a warning message is shown
        int ret = QMessageBox::warning(this, tr("Warning"), options::WARNING_CLOSE_IF_CANCEL,
                                       QMessageBox::Cancel | QMessageBox::Close, QMessageBox::Cancel);

        // switch the clicked button
        switch (ret)
        {
        // if the user clicks close than the program is close
        case QMessageBox::Close:
            if (ui->save->isVisible())
                // destroy the program
                exit(EXIT_FAILURE);
            // else just close the warning
        default:
            break;
        }
    }
    else
        this->close();

}

bool speakerProfileForm::is_available(QString newName)
{
    return !userRecords->containUser(newName);
}
bool speakerProfileForm::isValidGender()
{
    return ui->gender->currentIndex() != 0;
}
bool speakerProfileForm::isValidCountry()
{
    return ui->country->text().length() > 3;
}
bool speakerProfileForm::isValidEducation()
{
    return ui->education->currentIndex() != 0;
}
bool speakerProfileForm::isValidUsername(QString newUsername)
{
    return newUsername.length() > 0;
}

bool speakerProfileForm::validateParameters()
{
    QString newUsername = ui->usernameLineEdit->text();
    // verify the if argument are not empty
    bool isGender = isValidGender();
    bool isCountry = isValidCountry();
    bool isEducation = isValidEducation();
    bool isUsername = isValidUsername(newUsername);
    bool isUsernameWithoutSpaces = !(newUsername.contains(" "));
    bool isUsernameAvailable=true;
    if (isUsername)
        isUsernameAvailable = is_available(newUsername);

    // combile a warning message to inform the user about the wrong entries
    QString warning_str = options::WARNING_CORRECT_ENTRIES;
    if (!isGender)
        warning_str.append(options::ERROR_GENDER);
    if (!isCountry)
        warning_str.append(options::ERROR_COUNTRY);
    if (!isEducation)
        warning_str.append(options::ERROR_EDUCATION);
    if(!isUsername)
        warning_str.append(options::ERROR_ENTER_USER_NAME);
    if(!(isUsernameAvailable))
        warning_str.append(options::ERROR_USER_NAME_EXISTS);
    if(!(isUsernameWithoutSpaces))
        warning_str.append(options::ERROR_USER_NAME_SPACES);
    // if entries are wrong, warn the user
    if (!(isGender && isCountry && isEducation && isUsername && isUsernameAvailable && isUsernameWithoutSpaces))
    {
        QMessageBox::warning(this, tr("Warning"), warning_str,
                             QMessageBox::Ok, QMessageBox::Ok);
        return false;
    }
    return true;
}
