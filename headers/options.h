#ifndef OPTIONS_H
#define OPTIONS_H
#include <QStandardPaths>
/*!
    \namespace options

    \brief general options
    \author Juan Hussain

*/
namespace options {
const QString PATH_SEP ="/";

const QString APP_NAME = "Data Collection Tool for RELATER";

//App directories
// define the Documents directory as default directory to save the collected data
const QString DOC_DIR = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0];
const QString SAVE_DIR = DOC_DIR + "/VoiData";
const QString SAVE_DIR_USER_RECORDS = SAVE_DIR+ "/users";
//const QString SAVE_DIR_USER_DATA = SAVE_DIR+ "/data";
const QString USERLIST_FILE = SAVE_DIR_USER_RECORDS + "/user_records";
const QString AUDIO_DIR_NAME = "audio";
const QString STM_DIR_NAME =  "stm";
const QString SPEAKER_PROFILE_FILENAME = "/speakerProfile.csv";

//User Directories
// %1:username
const QString USER_WORKING_DIR = SAVE_DIR+ PATH_SEP + "%1";
const QString STM_DIR = USER_WORKING_DIR + PATH_SEP + STM_DIR_NAME;
const QString AUDIO_DIR = USER_WORKING_DIR + PATH_SEP +AUDIO_DIR_NAME ;

const QString USER_PROFILE_PATH = USER_WORKING_DIR + SPEAKER_PROFILE_FILENAME;


//Messages
const QString WARNING_TEXT_FILE_NOT_FOUND = "The specified file was not found!\n"
                                             "Please choose new text file";
const QString WARNING_CLOSE_IF_CANCEL = "By canceling the program will end! Procceed anyway?\n";
const QString WARNING_CORRECT_ENTRIES = "Please correct the following entries: \n\n";
const QString ERROR_GENDER = "- Gender \n";
const QString ERROR_COUNTRY = "- Country of origin \n";
const QString ERROR_EDUCATION = "- Educational level \n";
const QString ERROR_ENTER_USER_NAME = "- Enter User name pls \n";
const QString ERROR_USER_NAME_EXISTS = "- This user name already exists choose new one pls \n";
const QString ERROR_USER_NAME_SPACES = "- User name should not contains spaces \n";
}
#endif // OPTIONS_H
