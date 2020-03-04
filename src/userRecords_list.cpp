#include"../headers/userRecords_list.h"


UserRecords_list* UserRecords_list::userRecords = nullptr;

UserRecords_list::UserRecords_list()
{
    if (user_records.isEmpty())
    {
        for (int i =0;i<COL_NUM;i++) {
            user_records.append(QStringList({}));

        }
    }
    load_usernames();
}

UserRecords_list *UserRecords_list::getInstance()
{
    if (userRecords == nullptr)
        {
            userRecords = new UserRecords_list();
        }
        return userRecords;
}

void UserRecords_list::load_usernames()
{
    QFile file(options::USERLIST_FILE);
    QString msg;
    if (!file.open(QIODevice::ReadWrite)) {
        qDebug()<<"in load_usernames: can not open "<<options::USERLIST_FILE;
        return;
    }

    if (!util::readColsFromCSV(file," ",user_records,COL_NUM,msg)){
        qDebug()<<msg;
        return;
    }
    file.close();
}
void UserRecords_list::save_user_records()
{
    QString msg;
    util::writeColsToCSV(options::USERLIST_FILE," ",user_records,msg);
    qDebug()<<msg;
}
void UserRecords_list::assignUserStm(int user_index, QString stmFilename)
{
    user_records[LAST_STM_COL_NUM].insert(user_index,stmFilename);
    save_user_records();
}
void UserRecords_list::assignUserStm(QString username, QString stmFilename)
{
    user_records[LAST_STM_COL_NUM].insert(getUserIndex(username),stmFilename);
    save_user_records();
}

QString UserRecords_list::getUserStm(int user_index)
{
    if (user_index>user_records[LAST_STM_COL_NUM].size())
    {
        return "";
    }
    return user_records[LAST_STM_COL_NUM].at(user_index);
}

QStringList UserRecords_list::getRegisteredUsers() const
{
    if(USER_COL_NUM<user_records.size())
        return user_records[USER_COL_NUM];
    return QStringList({""});
}

void UserRecords_list::setRegisteredUsers(const QStringList &value)
{
    user_records[USER_COL_NUM] = value;
}

bool UserRecords_list::containUser(QString username)
{
    return getRegisteredUsers().contains(username);
}
bool UserRecords_list::addUser(QString username)
{
    if(containUser(username))
        return false;
    user_records[USER_COL_NUM].append(username);
    user_records[LAST_STM_COL_NUM].append("");
    currentUserIndex = user_records[USER_COL_NUM].size()-1;
    save_user_records();
    return true;
}
bool UserRecords_list::getUser(User &user, QString username)
{
    if (!containUser(username)) {
        return false;
    }
    int user_index = getRegisteredUsers().indexOf(username);
    user.setUsername(username);
    user.setStmFile(getUserStm(user_index));
    return true;
}
int UserRecords_list::getUserIndex(QString username)
{
    return getRegisteredUsers().indexOf(username);
}

bool UserRecords_list::addUser(const User &value)
{
    user = value;
    return addUser(user.getUsername());
}
