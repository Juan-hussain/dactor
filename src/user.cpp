#include"../headers/user.h"


User::User(){}


QString User::getStmFile() const
{
    return stmFile;
}

void User::setStmFile(const QString &value)
{
    stmFile = value;
}

QString User::getUsername() const
{
    return username;
}

void User::setUsername(const QString &value)
{
    username = value;
    prepareEnvironment();
}
QString User::getUserProfilePath() const
{
    return userProfilePath;
}

void User::setUserProfilePath(const QString &value)
{
    userProfilePath = value;
}

QString User::getUserStmDir() const
{
    return userStmDir;
}

void User::setUserStmDir(const QString &value)
{
    userStmDir = value;
}

QString User::getUserAudioDir() const
{
    return userAudioDir;
}

void User::setUserAudioDir(const QString &value)
{
    userAudioDir = value;
}

void User::prepareEnvironment()
{
    userProfilePath = QString(options::USER_PROFILE_PATH).arg(username);
    userStmDir = QString(options::STM_DIR).arg(username);
    userAudioDir = QString(options::AUDIO_DIR).arg(username);
    QDir().mkdir(QString(options::USER_WORKING_DIR).arg(username));
    QDir().mkdir(userAudioDir);
    QDir().mkdir(userStmDir);

}
bool User::saveProfile(Profile profile)
{
    QFile file(userProfilePath);
    if (!file.open(QFile::WriteOnly|QFile::Truncate))
    {
        return false;
    }
    QTextStream stream(&file);
    // stream << "Speaker ID: \t\t" << str_hash << "\n"
    stream << "Speaker ID;" << profile.getUsername() << "\n"
           << "Age;" <<  profile.getAge() << "\n"
           << "Gender;" << profile.getGender() << "\n"
           << "Country;" << profile.getCountry() << "\n"
           << "Edu. level;"  << profile.getEducation()<<"\n";
    file.close();
    return false;

}

// class profile implementation

Profile::Profile(QString username,QString age, QString gender,
                 QString country,QString education):\
    username(username), age(age), gender(gender),\
    country(country), education(education)
{}

QString Profile::getUsername() const
{
    return username;
}

void Profile::setUsername(const QString &value)
{
    username = value;
}

QString Profile::getAge() const
{
    return age;
}

void Profile::setAge(const QString &value)
{
    age = value;
}

QString Profile::getGender() const
{
    return gender;
}

void Profile::setGender(const QString &value)
{
    gender = value;
}

QString Profile::getCountry() const
{
    return country;
}

void Profile::setCountry(const QString &value)
{
    country = value;
}

QString Profile::getEducation() const
{
    return education;
}
