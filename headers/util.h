#ifndef UTIL_H
#define UTIL_H
#include <QFile>
#include <QDebug>
#include <QTextCodec>

/*!
    \namespace util

    \brief general utilities
    \author Juan Hussain

*/
namespace util {
bool readColsFromCSV(QFile& csvFile,QString csv_sep,QList<QStringList>& cols,int num_col,QString& msg);
bool readColsFromCSV(QString stmFilename,QString CSV_SEP,QList<QStringList>& cols,int num_col,QString& msg);
bool writeColsToCSV(QFile& csvFile,QString csv_sep,QList<QStringList>& cols,QString& msg);
bool writeColsToCSV(QString stmFilename,QString CSV_SEP,QList<QStringList>& col,QString& msg);


}

#endif // UTIL_H
