#include "headers/conditionaltransitions.h"


IsNotRecordedTransition::IsNotRecordedTransition(QObject *senderObj, QByteArray signal, QTableWidget *tw, StmManager *stm)
    :QSignalTransition(senderObj, signal)
{
    this->stm = stm;
    this->tw = tw;
}

bool IsNotRecordedTransition::eventTest(QEvent *event)
{
    if(!QSignalTransition::eventTest(event))
        return false;
    return !stm->isRecorded(tw->currentRow());
}


IsRecordedTransition::IsRecordedTransition(QObject *senderObj, QByteArray signal, QTableWidget *tw, StmManager *stm)
    :QSignalTransition(senderObj, signal)
{
    this->stm = stm;
    this->tw = tw;
}

bool IsRecordedTransition::eventTest(QEvent *event)
{
    if(!QSignalTransition::eventTest(event))
        return false;
    return stm->isRecorded(tw->currentRow());
}
