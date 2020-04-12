#ifndef STMMANAGER_H
#define STMMANAGER_H
#include <QFile>
#include <QStringList>
#include <QDebug>
#include <QTableWidget>
#include <QFileInfo>
#include<QTextCodec>
#include <QMessageBox>

#include "../headers/audiorecorder.h"
/*!
    \class StmManager

    \brief The StmManager class provides methods
    to manage stm files
    \author Juan Hussain

*/
class StmManager
{
private:

    const QString CSV_SEP = " ";
    const QString PATH_SEP= "/";
    QString INIT_FROM = "0.00";
    QString INIT_TO = "0.00";
    QTableWidget * tw;
    QString init_uttid;
    QString stmFilename;
    QString spk;
    QString wav;
//    QStringList tokens;

    QString get_uttid(int row);
    QString get_uttid(QString wav,QString spk,QString from,QString to);
//    QString get_init_uttid();

public:

    StmManager(QTableWidget *tw);
    bool initExistedSTM(QString spk,QString wav,QString stmFile);
    bool initNewSTM(QString spk,QString wav, QString txtFile, QString stmDir);
    bool fromSTM();//QTableWidget*& tw, QString stmFilename);
    bool toSTM();//QTableWidget*& tw, QString stmFilename);
    bool updateTable(int row,int col, QString item);
    bool updateTableAndStm(int row,int col, QString item);
    bool loadText(QString txtFilename);
    bool isRecorded(int row);
    QString getStmFilename() const;
    bool cleanToSTM(AudioRecorder* recorder,QString wavFile);


    const int STM_COL = 6;
    const int STM_UTTID_IDX = 0;
    const int STM_SPK_IDX = 1;
    const int STM_WAV_IDX = 2;
    const int STM_FROM_IDX = 3;
    const int STM_TO_IDX = 4;
    const int STM_UTT_IDX = 5; //the last one since we numerate from 0

    //    inline QStringList range(int start, int end)
    //    {
    //        QStringList l(end-start+1);
    //        std::iota(l.begin(), l.end(), start);
    //        return l;
    //    }

};

#endif // STMMANAGER_H
