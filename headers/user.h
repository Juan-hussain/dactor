#ifndef USER_H
#define USER_H


#include <QDir>
#include <QTextStream>
#include"options.h"
/*!
    \class Profile

    \brief for user profile
    \author Juan Hussain

*/
class Profile
{
private:
    QString username;
    QString age;
    QString gender;
    QString country;
    QString education;
public:
    Profile(){}
    Profile(QString username,QString age ="", QString gender = "m",
            QString country="",QString education="");
    QString getUsername() const;
    void setUsername(const QString &value);
    QString getAge() const;
    void setAge(const QString &value);
    QString getGender() const;
    void setGender(const QString &value);
    QString getCountry() const;
    void setCountry(const QString &value);
    QString getEducation() const;
};

/*!
    \class User

    \brief define user related properties and operation
    \author Juan Hussain

*/
class User
{
private:
    QString username;
    QString stmFile;
    QString userProfilePath;
    QString userStmDir;
    QString userAudioDir;
    Profile profile;
    void prepareEnvironment();

public:
    User();
    QString getUsername() const;
    void setUsername(const QString &value);
    QString getStmFile() const;
    void setStmFile(const QString &value);
    QString getUserProfilePath() const;
    void setUserProfilePath(const QString &value);
    bool saveProfile(Profile profile);

    QString getUserStmDir() const;
    void setUserStmDir(const QString &value);
    QString getUserAudioDir() const;
    void setUserAudioDir(const QString &value);
};



#endif // USER_H
