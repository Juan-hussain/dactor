#include"../headers/userRecords.h"


UserRecords* UserRecords::userRecords = nullptr;
UserRecords::UserRecords()
{
    // -- DATABASE INIT --
    DatabaseConnect();
    DatabaseInit();
}

UserRecords *UserRecords::getInstance()
{
    if (userRecords == nullptr)
    {
        userRecords = new UserRecords();
    }
    return userRecords;
}


void UserRecords::DatabaseConnect()
{
    const QString DRIVER("QSQLITE");

    if(QSqlDatabase::isDriverAvailable(DRIVER))
    {
        QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);

        db.setDatabaseName(options::USERLIST_FILE);

        if(!db.open())
            qWarning() << "MainWindow::DatabaseConnect - ERROR: " << db.lastError().text();
    }
    else
        qWarning() << "MainWindow::DatabaseConnect - ERROR: no driver " << DRIVER << " available";
}

void UserRecords::DatabaseInit()
{
    QSqlQuery query("CREATE TABLE IF NOT EXISTS userRecords (username TEXT UNIQUE, stmfile TEXT DEFAULT '')");

    if(!query.isActive())
        qWarning() << "MainWindow::DatabaseInit - ERROR: " << query.lastError().text();

}


void UserRecords::setUserStm(QString username, QString stmFilename)
{
    QSqlQuery query;
    query.prepare("UPDATE userRecords SET stmFile=:stmFilename WHERE username=:username");
    query.bindValue(":stmFilename",stmFilename);
    query.bindValue(":username",username);
    if(!query.exec())
        qWarning() << "UserRecords::setUserStm - ERROR: " << query.lastError().text();
}


bool UserRecords::containUser(QString username)
{
    QSqlQuery query;
    query.prepare("SELECT 1 FROM userRecords WHERE username =:username");
    query.bindValue(":username",username);
    query.exec();
    if (query.first())
        return true;
    return false;
}
bool UserRecords::addUser(QString username)
{
    QSqlQuery query;
    query.prepare("INSERT INTO userRecords(username) VALUES(:username)");
    query.bindValue(":username",username);
    if(!query.exec())
    {
        qWarning() << "UserRecords::addUser(username) - ERROR: " << query.lastError().text();
        return false;
    }
    return true;
}
bool UserRecords::addUser(const User &user)
{
    QSqlQuery query;
    query.prepare("INSERT INTO userRecords(username,stmFile)" \
                  " VALUES(:username,:stmFile)");
    query.bindValue(":username",user.getUsername());
    query.bindValue(":stmFile",user.getStmFile());
    if(!query.exec())
    {
        qWarning() << "UserRecords::addUser(User) - ERROR: " << query.lastError().text();
        return false;
    }
    return true;
}

User* UserRecords::getUser(QString username)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM userRecords WHERE username=:username");
    query.bindValue(":username",username);
    if(!query.exec())
    {
        qWarning() << "getUser(User, QString) - ERROR: " << query.lastError().text();
        return nullptr;
    }
    if (query.first()) {
        User* user = new User();
        user->setUsername(query.value(USER_COL_NUM).toString());
        user->setStmFile(query.value(LAST_STM_COL_NUM).toString());
        return user;
    }
    return nullptr;
}
