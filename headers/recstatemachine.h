#ifndef RECSTATEMACHINE_H
#define RECSTATEMACHINE_H
#include<qstatemachine.h>
#include<QPushButton>
#include "audiorecorder.h"
#include "conditionaltransitions.h"
#include "stmmanager.h"
#include"ui_mainwindow.h"
#include<QDebug>
#include <QShortcut>
#include <QStandardPaths>
#include <QFinalState>
#include <QTimer>
#include <QtConcurrent>


//#include"transitions.h"


class RecStateMachine: public QObject
{
    Q_OBJECT
private:
    bool REPLACE_REC = false;
    //conditional transitions
    IsNotRecordedTransition *idle_to_rec, *rec_to_rec, *rerec_to_rec;
    IsRecordedTransition *idle_to_rerec, *idle_to_play, *rec_to_idle, *rerec_to_idle;
    QState *idle;
    QState *rec;
    QState *rerec;
    QState *play;
    QState *mainState;
    Ui::MainWindow* ui;
    QStateMachine machine;
    AudioRecorder recorder;
    StmManager *stm;
    QString wavFile;
    int currentRow;
    QTimer *replay_timer;
    QColor recColor = QColor(220,230,220);
    void enable_ui_elements(int state,bool t_f);
;
    void saveCleanedRecording();
    void changeRowColor();
    void FromToSecond2byte(int row, int& byteFrom,int& byteTo);
    void resizeTable();

public:
    RecStateMachine(Ui::MainWindow *ui, StmManager *stm);
    void init(QString wavFile);
    void stop();
    QColor getRecColor() const;

public slots:
    void idle_entered();
    void rec_entered();
    void rerec_entered();
    void replay_entered();
    void mainState_entered();
    void final_entered();

    void idle_exited();
    void rec_exited();
    void rerec_exited();
    void replay_exited();
    void replay_stop();
    void mainState_exited();
    void enter_pressed();


signals:
    void end_replay();
    void already_recorded();
    void endOfTable();
    void stoping_machine();

};
#endif // RECSTATEMACHINE_H
