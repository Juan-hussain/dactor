#ifndef CONDITIONALTRANSITIONS_H
#define CONDITIONALTRANSITIONS_H

#include "stmmanager.h"

#include <QSignalTransition>
#include <QTableWidget>


class IsNotRecordedTransition: public QSignalTransition{

public:
    IsNotRecordedTransition(QObject *senderObj, QByteArray signal, QTableWidget *tw, StmManager *stm);

protected:
    bool eventTest(QEvent *event) override;

private:
    StmManager *stm;
    QTableWidget *tw;
};


class IsRecordedTransition: public QSignalTransition{

public:
    IsRecordedTransition(QObject *senderObj, QByteArray signal, QTableWidget *tw, StmManager *stm);

protected:
    bool eventTest(QEvent *event) override;

private:
    StmManager *stm;
    QTableWidget *tw;
};


#endif // CONDITIONALTRANSITIONS_H
