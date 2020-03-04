#include "headers/mainwindow.h"
#include "ui_mainwindow.h"


// function to update the text size
void MainWindow::updateTextSize(int value)
{
    // update the font size
    tableFont.setPointSize(qBound(min_font_size, tableFont.pointSize() + value, max_font_size));
    // set the new font to the table
    ui->table->setFont(tableFont);
    // update the size of the rows to feat the new text dimension
    ui->table->resizeRowsToContents();
    ui->table->resizeColumnsToContents();

}

User* MainWindow::log_in(bool closeIfCancel){
    logInForm = new LogInForm(this);
    logInForm->setCloseIfCancel(closeIfCancel);
    logInForm->setModal(true);
    logInForm->exec();
    return logInForm->getUser();
}

void MainWindow::initWindow(User* user, QString path_to_text)
{   
    tableFont.setPointSize(12);

    ui->next->setVisible(false);

    prepareTable(); // there is also postpareTable() after filling it.

    QString filename_base = init_stm(path_to_text);

    QString stmFile = stm->getStmFilename();
    user->setStmFile(stmFile);
    userRecords->setUserStm(user->getUsername(),stmFile);
    postpareTable();
    QString wavFilename = filename_base+".wav";
    QString wavFile = user->getUserAudioDir() +options::PATH_SEP+ wavFilename;

    recStateMachine->init(wavFile);
}

QString MainWindow::init_stm(QString path_to_text)
{
    QString filename_base;
    QString stmFile;
    if (path_to_text!="") {
        QFileInfo txtFile_info(path_to_text);
        filename_base = txtFile_info.completeBaseName();
        stmFile = user->getUserStmDir()+ options::PATH_SEP + filename_base +".stm";
    } else {
        stmFile = user->getStmFile();
    }

    if  (QFile::exists(stmFile)){
        QFileInfo stmFile_info(stmFile);
        filename_base = stmFile_info.completeBaseName();
        QString wavFilenameBase = filename_base;
        stm->initExistedSTM(user->getUsername(),wavFilenameBase,stmFile);
    } else {
        while (!QFile::exists(path_to_text)){
            cout << "Text file not found!" << endl;
            QMessageBox::warning(this, tr("Warning"),
                                 options::WARNING_TEXT_FILE_NOT_FOUND,
                                 QMessageBox::Ok, QMessageBox::Ok);
            // show the setting to reselect the texts folder
            path_to_text = select_text();
        }
        QFileInfo txtFile_info(path_to_text);
        filename_base = txtFile_info.completeBaseName();
        QString wavFilenameBase = filename_base;
        QString stmFilename = user->getUserStmDir()+ options::PATH_SEP + filename_base +".stm";
        stm->initNewSTM(user->getUsername(), wavFilenameBase, path_to_text,stmFilename);

    }
    return  filename_base;
}


// constructor of the MainWindow
MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //QLoggingCategory::setFilterRules("*.debug=false\n");

    if (!QDir(options::SAVE_DIR).exists())
        if(!QDir().mkdir(options::SAVE_DIR))
            qDebug()<<"MainWindow::MainWindow(): " <<"couldnt create: "<<options::SAVE_DIR;

    if (!QDir(options::SAVE_DIR_USER_RECORDS).exists())
        if(!QDir().mkdir(options::SAVE_DIR_USER_RECORDS))
            qDebug()<<"couldnt create: "<<options::SAVE_DIR_USER_RECORDS;
    userRecords = UserRecords::getInstance();
    // initialize the main window
    ui->setupUi(this);
    //setup key navigation
    key_right = new QShortcut(QKeySequence(Qt::Key_Right), ui->centralWidget);
    key_down = new QShortcut(QKeySequence(Qt::Key_Down), ui->centralWidget);
    key_left = new QShortcut(QKeySequence(Qt::Key_Left), ui->centralWidget);
    key_up = new QShortcut(QKeySequence(Qt::Key_Up), ui->centralWidget);
//    key_space = new QShortcut(QKeySequence(Qt::Key_Space), ui->centralWidget);
//    QShortcut key_space(QKeySequence(Qt::Key_Space), ui->centralWidget);

    connect(key_right, SIGNAL(activated()), this, SLOT(key_next()));
    connect(key_down, SIGNAL(activated()), this, SLOT(key_next()));
    connect(key_left, SIGNAL(activated()), this, SLOT(key_previous()));
    connect(key_up, SIGNAL(activated()), this, SLOT(key_previous()));
    connect(ui->up, SIGNAL(clicked()), this, SLOT(key_previous()));
    connect(ui->down, SIGNAL(clicked()), this, SLOT(key_next()));
//    connect(ui->rec, SIGNAL(clicked()), this, SLOT(on_rec_clicked()));
//    connect(key_space, SIGNAL(activated()), this, SLOT(on_rec_clicked()));

    resize(1900,1300);
    this->setWindowTitle("DaCToR");

    // show the main window
    MainWindow::MainWindow::show();

    User* newUser = log_in(true);
    user =  (newUser!=nullptr) ? newUser:user;
    stm = new StmManager(ui->table);
    recStateMachine = new RecStateMachine(ui, stm);
    initWindow(user, "");
    ui->table->item(0, 5)->setSelected(true);
    ui->table->selectRow(0);
}

// destructor of the MainWindow
MainWindow::~MainWindow()
{
    delete ui;
}


// function to increment the size of the text when the appropriate button is clicked
void MainWindow::on_text_plus_clicked()
{
    updateTextSize(1);
}


// decrement the size of the text when the appropriate button is clicked
void MainWindow::on_text_minus_clicked()
{
    updateTextSize(-1);
}


void MainWindow::on_settings_clicked()
{
    QString selected_text = select_text();
    if (selected_text == "") {
        return;
    }
    initWindow(user,selected_text);
}
void MainWindow::on_rec_clicked()
{
    //emit record_button_clicked();
    qDebug()<<"rec clicked";
    if (recOddClick) {
        ui->rec->setStyleSheet("QPushButton#rec{image: url(:icons/mic_released_full.png);  border-style: solid; border-width: 0px; border-color: gray;} #rec:hover{image: url(:icons/mic_released_hover.png);} #rec:pressed{image: url(:icons/mic_released_pressed.png);}");
        recOddClick = false;
    } else {
        ui->rec->setStyleSheet("QPushButton#rec{image: url(:icons/mic_button.png);  border-style: solid; border-width: 0px; border-color: gray;} #rec:hover{image: url(:icons/mic_hover.png);} #rec:pressed{image: url(:icons/mic_pressed.png);}");
        recOddClick = true;
    }
}
void MainWindow::on_play_clicked()
{
    qDebug()<<"play clicked";
    if (playOddClick) {
        playOddClick = false;
    } else {
        playOddClick = true;
    }
}

QString MainWindow::select_text()
{
    //savedSettings = new QSettings("RELATER", "settings")
    settings settingsWindow;
    settingsWindow.setModal(true);
    settingsWindow.exec();
    return settingsWindow.getSelectedText();
}

void MainWindow::on_add_user_clicked()
{
    user = log_in(false);
    if (user!=nullptr)
    {
        recStateMachine->stop();
        initWindow(user,"");
    }
}

void MainWindow::closeEvent( QCloseEvent *event)
{
    qDebug()<<"application closed";
    recStateMachine->stop();
}

void MainWindow::key_next()
{
    bool isEndReached = end_of_table(false);
    qDebug()<<" MainWindow::key_next(): isEndReached"<< isEndReached;
    if (isEndReached){
        ui->table->setCurrentCell(ui->table->currentRow(), 5);
    }
    else {
        ui->table->setCurrentCell(ui->table->currentRow()+1, 5);
    }
    ui->table->setFocus();

}

void MainWindow::key_previous()
{
    bool isEndReached = end_of_table(true);
    qDebug()<<"MainWindow::key_previous(): isEndReached "<< isEndReached;

    if (isEndReached){
        ui->table->setCurrentCell(ui->table->currentRow(), 5);
    }
    else{
        ui->table->setCurrentCell(ui->table->currentRow()-1, 5);
    }
    ui->table->setFocus();

}


bool MainWindow::end_of_table(bool look_upwards)
{
    // QString currentState = machine.configuration().toList().at(0)->property("objectName").toString();
    if (look_upwards){
        if (ui->table->currentRow()-1 < 0){
            emit recStateMachine->endOfTable();
            return true;
        } else {
            return false;
        }
    }else{
        if (ui->table->currentRow()+1 >= ui->table->rowCount()){
            emit recStateMachine->endOfTable();
            return true;
        }else
            return false;
    }
}

void MainWindow::prepareTable()
{
    // init the column count
    ui->table->setColumnCount(stm->STM_COL);

    // set the 3 first columns (uttID, skpID, path_to_file) as invisible
    ui->table->setColumnHidden(stm->STM_UTTID_IDX, true);
    ui->table->setColumnHidden(stm->STM_SPK_IDX, true);
    ui->table->setColumnHidden(stm->STM_WAV_IDX, true);

    // stretsh the last column to fit the remaining space
    ui->table->horizontalHeader()->setStretchLastSection(true);

    // hide the horizontale header of the table
    ui->table->horizontalHeader()->setVisible(false);

    //hide grid
    ui->table->setShowGrid(false);
  
    // disable text editing
    ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers);


    // set the font of the text
    ui->table->setFont(tableFont);
}



void MainWindow::postpareTable()
{
    // resize the rows to the content
    ui->table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->table->setCurrentCell(0,stm->STM_UTT_IDX);
    //ui->table->set

    //set recorded to green
//    for(int i=0; i<ui->table->rowCount(); i++){
//        if (stm->isRecorded(i)){
//            QTableWidgetItem* row1 = new QTableWidgetItem(QString::number(i));
//            row1->setBackground(QColor(0,255,0));
//            row1->setForeground(QColor(0,255,0));
//            ui->table->setVerticalHeaderItem(i,row1);

////            ui->table->item(i, stm->STM_FROM_IDX)->setBackgroundColor(recStateMachine->getRecColor());
////            ui->table->item(i, stm->STM_TO_IDX)->setBackgroundColor(recStateMachine->getRecColor());
////            ui->table->item(i, stm->STM_UTT_IDX)->setBackgroundColor(recStateMachine->getRecColor());

//        }
//    }
}

