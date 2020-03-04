#ifndef USER_RECORDS_H
#define USER_RECORDS_H
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include<QDebug>
#include"util.h"
#include "options.h"
#include "user.h"

/*!
    \class UserRecords

    \brief save som user related information
    \author Juan Hussain

*/
class UserRecords
{
private:
    const unsigned int USER_COL_NUM = 0;
    const unsigned int LAST_STM_COL_NUM = 1;
    const unsigned int COL_NUM =2;

    static UserRecords* userRecords;
    UserRecords();


public:
    static UserRecords* getInstance();
    void DatabaseConnect();
    void DatabaseInit();


    void setUserStm(QString username, QString stmFilename);
    QString getUserStm(QString username);
    QStringList getRegisteredUsers() const;
    void setRegisteredUsers(const QStringList &value);
    bool containUser(QString username);
    bool addUser(QString username);
    User* getUser(QString username);
    bool addUser(const User &value);

};

#endif // USER_RECORDS_H
