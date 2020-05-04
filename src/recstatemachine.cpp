#include "../headers/recstatemachine.h"



RecStateMachine::RecStateMachine(Ui::MainWindow *ui, StmManager *stm):
    ui(ui), stm(stm)
{
    recorder.init();
    //states
    mainState= new QState(&machine);
    idle = new QState(mainState);
    rec = new QState(mainState);
    play = new QState(mainState);
    mainState->setInitialState(idle);
    QFinalState* final = new QFinalState(&machine);
    rec->setProperty("objectName", "rec");
    play->setProperty("objectName", "replay");

    if (REPLACE_REC) {
        rerec = new QState(mainState);
        rerec->setProperty("objectName", "rerec");
        //define custom conditional transition
        idle_to_rec = new IsNotRecordedTransition(ui->rec, SIGNAL(clicked()), ui->table, stm);
        idle_to_rerec = new IsRecordedTransition(ui->rec, SIGNAL(clicked()), ui->table, stm);
        idle_to_play = new IsRecordedTransition(ui->play, SIGNAL(clicked()), ui->table, stm);

        rec_to_idle = new IsRecordedTransition(ui->table, SIGNAL(currentCellChanged(int, int, int, int)), ui->table, stm);
        rec_to_rec = new IsNotRecordedTransition(ui->table, SIGNAL(currentCellChanged(int, int, int, int)), ui->table, stm);

        rerec_to_idle = new IsRecordedTransition(ui->table, SIGNAL(currentCellChanged(int, int, int, int)), ui->table, stm);
        rerec_to_rec = new IsNotRecordedTransition(ui->table, SIGNAL(currentCellChanged(int, int, int, int)), ui->table, stm);

        //target state
        idle_to_rec->setTargetState(rec);
        idle_to_rerec->setTargetState(rerec);
        idle_to_play->setTargetState(play);

        rec_to_idle->setTargetState(idle);
        rec_to_rec->setTargetState(rec);

        rerec_to_idle->setTargetState(idle);
        rerec_to_rec->setTargetState(rec);

        //transition
        idle->addTransition(idle_to_rec);
        idle->addTransition(idle_to_rerec);
        idle->addTransition(idle_to_play);

        rerec->addTransition(ui->rec, SIGNAL(clicked()), idle);
        rerec->addTransition(rerec_to_idle);
        rerec->addTransition(rerec_to_rec);
        rerec->addTransition(this, SIGNAL(endOfTable()), idle);

        rec->addTransition(rec_to_idle);
        rec->addTransition(rec_to_rec);

        //rerec->assignProperty(ui->rec,"text","stop");
        connect(rerec, SIGNAL(entered()), this, SLOT(rerec_entered()));
        connect(rerec, SIGNAL(exited()), this, SLOT(rerec_exited()));

    } else{
        idle->addTransition(ui->rec, SIGNAL(clicked()),rec);
        idle->addTransition(ui->play, SIGNAL(clicked()),play);
        rec->addTransition(ui->table, SIGNAL(currentCellChanged(int, int, int, int)),rec);
        //rec->addTransition(ui->table, SIGNAL(itemSelectionChanged()),rec);

    }

    rec->addTransition(ui->rec, SIGNAL(clicked()),idle);
 //   rec->addTransition(ui->w., SIGNAL(clicked()),idle);

    rec->addTransition(this, SIGNAL(endOfTable()), idle);

    play->addTransition(ui->play, SIGNAL(clicked()), idle);
    play->addTransition(this, SIGNAL(end_replay()), idle);
    play->addTransition(ui->table, SIGNAL(itemSelectionChanged()), idle);

    // to implement later for continous replay
    //play->addTransition(ui->table, SIGNAL(itemSelectionChanged()), play);
    //play->addTransition(this, SIGNAL(endOfTable()), idle); to

    mainState->addTransition(this,SIGNAL(stoping_machine()),final);

    // connect to state entered signal
    connect(idle, SIGNAL(entered()), this, SLOT(idle_entered()));
    connect(rec, SIGNAL(entered()), this, SLOT(rec_entered()));
    connect(play, SIGNAL(entered()), this, SLOT(replay_entered()));
    connect(mainState, SIGNAL(entered()), this, SLOT(mainState_entered()));
    connect(final, SIGNAL(entered()), this, SLOT(final_entered()));

    // connect to state exited signal
    connect(idle, SIGNAL(exited()), this, SLOT(idle_exited()));
    connect(rec, SIGNAL(exited()), this, SLOT(rec_exited()));
    connect(play, SIGNAL(exited()), this, SLOT(replay_exited()));
    connect(mainState, SIGNAL(exited()), this, SLOT(mainState_exited()));

    //add the states
    machine.setInitialState(mainState);
}
void RecStateMachine::resizeTable()
{
    ui->table->resizeRowsToContents();
    ui->table->resizeColumnsToContents();
}
void RecStateMachine::init(QString wavFile)
{
    this->wavFile = wavFile;
    replay_timer = new QTimer();

    recorder.init();
    recorder.reset();

    resizeTable();

    if (!QFile(wavFile).exists())
        qDebug()<< "RecStateMachine::init(): There is no audio file.. load one";
    else
        recorder.load_audio(wavFile);
    Q_ASSERT(!machine.isRunning());
//    if (machine.isRunning()) {
//        stop();
//        QEventLoop loop;
//        connect(&machine, SIGNAL(finished()), &loop, SLOT(quit()));
//    }
    machine.start();
}

void RecStateMachine::stop()
{
    emit stoping_machine();
    //machine.stop(); this send the signal finish but the machine still running and final does not enter
    QEventLoop loop;
    connect(&machine, SIGNAL(finished()), &loop, SLOT(quit()));
    qDebug()<< "machine running: "<< machine.isRunning();
}
void RecStateMachine::idle_entered()
{
    qDebug()<<"idle_entered";
    if (recorder.getIsRecording()) {
        recorder.stop();
    }
    //ui->table->setFocus();
}

QColor RecStateMachine::getRecColor() const
{
    return recColor;
}
// 0 for rec and rerec, 1 for play
void RecStateMachine::enable_ui_elements(int state,bool t_f)
{
    switch (state) {
    case 0 :
        ui->play->setEnabled(t_f);
        break;
    case 1:
        ui->rec->setEnabled(t_f);
    }
    ui->settings->setEnabled(t_f);
    ui->add_user->setEnabled(t_f);
}

void RecStateMachine::rec_entered()
{
    qDebug()<<"rec_entered";
    //ui->table->setFocus();
    enable_ui_elements(0,false);

    recorder.record();
    currentRow = ui->table->currentRow();
    qDebug()<<"curr row: "<< currentRow;
    stm->updateTable(currentRow, stm->STM_FROM_IDX, QString::number(recorder.getBufferPos()/recorder.getBytePerSec()));
}

void RecStateMachine::rerec_entered()
{
    qDebug()<<"rerec_entered";
    ui->table->setFocus();

    enable_ui_elements(0,false);

    //connect(ui->table, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(save_current(int, int, int, int)));

    currentRow = ui->table->currentRow();
    qDebug()<<"curr row: "<< currentRow;

    double from = ui->table->item(currentRow, stm->STM_FROM_IDX)->data(Qt::DisplayRole).toDouble()*recorder.getBytePerSec();
    double to = ui->table->item(currentRow, stm->STM_TO_IDX)->data(Qt::DisplayRole).toDouble()*recorder.getBytePerSec();
    recorder.replace_start(from, to);
}

void RecStateMachine::replay_entered()
{
    qDebug()<<"replay_entered";
    enable_ui_elements(1,false);

    //ui->table->setFocus();

    int curr_row = ui->table->currentRow();
    double from = ui->table->item(curr_row, stm->STM_FROM_IDX)->data(Qt::DisplayRole).toDouble();
    double to = ui->table->item(curr_row, stm->STM_TO_IDX)->data(Qt::DisplayRole).toDouble();

    recorder.play(from, to);
    replay_timer->setInterval(static_cast<int>((to-from)*1000));
    replay_timer->setSingleShot(true);
    connect(replay_timer, SIGNAL(timeout()), SLOT(replay_stop()));
    replay_timer->start();
}

void RecStateMachine::mainState_entered()
{
    qDebug()<<"main state entered";
}

void RecStateMachine::final_entered()
{
    qDebug()<<"finished entered";
    //saveCleanedRecording();
    stm->cleanToSTM(&recorder,wavFile);
}
void RecStateMachine::idle_exited()
{
    qDebug()<<"idle_exited";
}

void RecStateMachine::rec_exited()
{
    qDebug()<<"rec_finished";

    //stm update row, col, time
    stm->updateTableAndStm(currentRow, stm->STM_TO_IDX, QString::number(recorder.getBufferPos()/recorder.getBytePerSec()));
    recorder.save(wavFile);
    //ui->table->resizeColumnsToContents();
    //ui->table->resizeRowsToContents();

    enable_ui_elements(0,true);
    //resizeTable();
}
void RecStateMachine::changeRowColor()
{
    ui->table->item(currentRow, stm->STM_FROM_IDX)->setBackgroundColor(recColor);
    ui->table->item(currentRow, stm->STM_TO_IDX)->setBackgroundColor(recColor);
    ui->table->item(currentRow, stm->STM_UTT_IDX)->setBackgroundColor(recColor);
}

void RecStateMachine::rerec_exited()
{
    qDebug()<<"rerec_exited";
    qDebug()<<"curr row: "<< currentRow;

    int from,to;
    FromToSecond2byte(currentRow,from,to);
    long int time_shift = recorder.replace_finish(from, to);

    //update phrases which were recorded after this one
    int p_from, p_to, n_from, n_to;

    for(int row=0; row<ui->table->rowCount(); row++){
        if(stm->isRecorded(row)){
            FromToSecond2byte(row,p_from,p_to);
            if(p_from == from){
                n_from = p_from;
                n_to = p_to + time_shift;
            }
            if(p_from > from){
                n_from = p_from + time_shift;
                n_to = p_to + time_shift;
            }
            if(p_from >= from){
                stm->updateTable(row, stm->STM_FROM_IDX, QString::number(n_from/recorder.getBytePerSec()));
                stm->updateTable(row, stm->STM_TO_IDX, QString::number(n_to/recorder.getBytePerSec()));
                ui->table->item(row, stm->STM_FROM_IDX)->setBackgroundColor(recColor);
                ui->table->item(row, stm->STM_TO_IDX)->setBackgroundColor(recColor);
                ui->table->item(row, stm->STM_UTT_IDX)->setBackgroundColor(recColor);

            }
        }
    }
    stm->toSTM();
    recorder.save(wavFile);

    enable_ui_elements(0, true);

}

void RecStateMachine::FromToSecond2byte(int row, int& byteFrom,int& byteTo)
{
    double from = ui->table->item(row, stm->STM_FROM_IDX)->data(Qt::DisplayRole).toDouble()*recorder.getBytePerSec();
    double to = ui->table->item(row, stm->STM_TO_IDX)->data(Qt::DisplayRole).toDouble()*recorder.getBytePerSec();
    byteFrom = static_cast<int>(from);
    byteTo = static_cast<int>(to);
    recorder.correctByteIndex(byteFrom,byteTo);
}


void RecStateMachine::replay_exited()
{
    qDebug()<<"replay_finished";

    replay_timer->stop();

    recorder.pausePlaying();
    ui->play->setChecked(false);

    enable_ui_elements(1,true);
}

void RecStateMachine::replay_stop()
{
    qDebug()<<"RecStateMachine::replay_exit(): timer alarm";
    // ui->down->click(); for continous replay todo later
    emit end_replay();

}

void RecStateMachine::mainState_exited()
{
    qDebug()<<"main state exited";
}

void RecStateMachine::enter_pressed()
{
    stm->toSTM();
}

//not used
void RecStateMachine::saveCleanedRecording()
{
    qDebug()<<"RecStateMachine::saveCleanedRecording()";
    recorder.clearSaveBuff();
    int byteFrom, byteTo;
    for(int row=0; row<ui->table->rowCount(); row++){
        if(stm->isRecorded(row)){
            FromToSecond2byte(row,byteFrom,byteTo);
            double newFrom = recorder.getSaveBufferPos();
            if (!recorder.savePart2Buff(byteFrom,byteTo)){ continue;}
            stm->updateTable(row, stm->STM_FROM_IDX, QString::number(newFrom/recorder.getBytePerSec()));
            double newTo = recorder.getSaveBufferPos();
            stm->updateTable(row, stm->STM_TO_IDX, QString::number(newTo/recorder.getBytePerSec()));

        }
    }
    //it is important to finish saving audio before the time informaion
    recorder.saveBuff2file(wavFile);
    stm->toSTM();
}
