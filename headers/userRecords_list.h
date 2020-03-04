#ifndef USER_RECORDS_H
#define USER_RECORDS_H

#include<QFile>
#include<QDebug>
#include"util.h"
#include "options.h"
#include "user.h"

/*!
    \class UserRecords_list

    \brief uses csv files to save some user infrotmation
    this class is still bugy
    \author Juan Hussain

*/
class UserRecords_list
{
private:
    static UserRecords_list* userRecords;
    UserRecords_list();

    const unsigned int USER_COL_NUM = 0;
    const unsigned int LAST_STM_COL_NUM = 1;
    const unsigned int COL_NUM =2;
    QList<QStringList> user_records{QStringList(),QStringList()};
    QStringList registeredUsers;
    User user;
    int currentUserIndex;


public:
    static UserRecords_list* getInstance();
    void load_usernames();
    void save_user_records();
    void assignUserStm(int user_index, QString stmFilename);
    void assignUserStm(QString username, QString stmFilename);
    QString getUserStm(int user_index);
    QStringList getRegisteredUsers() const;
    void setRegisteredUsers(const QStringList &value);
    bool containUser(QString username);
    bool addUser(QString username);
    bool getUser(User &user, QString username);
    bool addUser(const User &value);
    int getUserIndex(QString username);


};

#endif // USER_RECORDS_H
