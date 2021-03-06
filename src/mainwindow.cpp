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

bool MainWindow::initWindow(User* user, QString path_to_text)
{
//    recStateMachine->stop();
    QString wavFile = init_stm(path_to_text, user);
    if(wavFile=="")
        return false;
    // stop to restart later
    recStateMachine->stop();
    tableFont.setPointSize(12);

    ui->next->setVisible(false);

    prepareTable(); // there is also postpareTable() after filling it.

    QString stmFile = stm->getStmFilename();
    user->setStmFile(stmFile);
    userRecords->setUserStm(user->getUsername(),stmFile);
    postpareTable();
    ui->label_user->setText(user->getUsername());
    ui->label_file->setText(user->getStmFile());
    recStateMachine->init(wavFile);
    return true;
}

QString MainWindow::init_stm(QString path_to_text, User *user)
{
    /* the cases to look at:
     * 1. empty path (login or change user)
     *    a. user has stm (existed user): load this,
     *          check(wav): no => abnormal
     *    b. no stm (new user): choose text
     *          check(wav): yes => abnormal
     * 2. no empty path (selected text or stm)
     *    a. text file:
     *        I. exists stm: load
     *              check(wav): no => abnormal
     *        II. no stm: make one:
     *              check(wav): yes =>abnormal
     *    b. stm file:
     *        check(wav) yes and no => normal
     */
    /* the algorithm
     * if path empty => find path
     * if file ext==txt =>
     */

    QString stmFile;
    QString userstmFile = user->getStmFile();
    // case 1.b
    while (!QFile::exists(path_to_text) && !QFile::exists(userstmFile)) {
            cout << "Text or STM file not found!" << endl;
            int res = QMessageBox::warning(this, tr("Warning"),
                                 options::WARNING_TEXT_FILE_NOT_FOUND,
                                 QMessageBox::Ok, QMessageBox::Cancel);
            if (res == QMessageBox::Cancel) {
                return "";
            }
            path_to_text = select_text();
     }
    // all other cases:
    QFileInfo txtFile_info(path_to_text);
    QString directory = txtFile_info.absolutePath();
    QString filename_base = txtFile_info.completeBaseName();
    QString ext = txtFile_info.completeSuffix();
    QString wavFilenameBase = filename_base+".wav";
    QString wavFile = user->getUserAudioDir() +options::PATH_SEP+ wavFilenameBase;
    if (ext == "stm") {
            QString filename_base = txtFile_info.completeBaseName();
            stmFile = path_to_text;
    } else if (ext == "txt") { // case 1.b, 2.a.II, 2.a.I
            stmFile = user->getUserStmDir()+ options::PATH_SEP + filename_base +".stm";
            if (QFile::exists(stmFile)){//case 2.a.I
                if(!QFile::exists(wavFile)){
                    QMessageBox::critical(this,tr("Error"),options::ERROR_STM_NO_WAVE);
                    return "";
                }
            } else {             //case 1.b , 2.a.II
                if(QFile::exists(wavFile)){
                    QMessageBox::critical(this,tr("Error"),options::ERROR_WAVE_NO_STM);
                    return "";
                }
                stm->initNewSTM(user->getUsername(), wavFilenameBase, path_to_text,stmFile);
                return  wavFile;
            }
    } else if (QFile::exists(userstmFile)) { //case 1.a
        stmFile = userstmFile;
        QFileInfo stmFile_info(userstmFile);
        filename_base = stmFile_info.completeBaseName();
        wavFilenameBase = filename_base+".wav";
        wavFile = user->getUserAudioDir() +options::PATH_SEP+ wavFilenameBase;
    }
    //check(wav)
    while (!QFile::exists(wavFile)) {
            cout << "wav file not found!" << endl;
            int res = QMessageBox::warning(this, tr("Warning"),
                                 options::WARNING_WAV_FILE_NOT_FOUND,
                                 QMessageBox::Ok, QMessageBox::Cancel);
            if (res == QMessageBox::Cancel) {
                return "";
            }
            wavFile = QFileDialog::getOpenFileName(this, tr("Open Directory"), directory,
                                                            tr("WAV files (*.wav)"));
            QFileInfo wavFile_info(wavFile);
            wavFilenameBase = wavFile_info.completeBaseName();
    }
    //case 2.a.I, 2.b, 1.a
    stm->initExistedSTM(user->getUsername(),wavFilenameBase,stmFile);
    return  wavFile;
}
//QString MainWindow::init_stm(QString path_to_text)
//{
//    /* the cases to look at:
//     * 1. empty path (login or change user)
//     *    a. user has stm (existed user): load this,
//     *          check(wav): no => abnormal
//     *    b. no stm (new user): choose text
//     *          check(wav): yes => abnormal
//     * 2. no empty path (selected text or stm)
//     *    a. text file:
//     *        I. exists stm: load
//     *              check(wav): no => abnormal
//     *        II. no stm: make one:
//     *              check(wav): yes =>abnormal
//     *    b. stm file:
//     *        check(wav)
//     */
//    QString statecase;
//    QString stmFile;
//    QString userstmFile = user->getStmFile();
//    // case 1: path empty
//    if (path_to_text==""){
//        if(QFile::exists(userstmFile)){
//            statecase = "1.a";
//        } else {
//            statecase="1.b";
//        }
//    } else {
//            QFileInfo txtFile_info(path_to_text);
//            QString directory = txtFile_info.absolutePath();
//            QString filename_base = txtFile_info.completeBaseName();
//            QString ext = txtFile_info.completeSuffix();
//            if (ext == "stm") {
//                statecase = "2.a";
//            }
//            else if (ext == "txt") {
//                statecase = "2.b";
//            }
//    }

//    while (!QFile::exists(path_to_text) && !QFile::exists(userstmFile)) {
//            cout << "Text or STM file not found!" << endl;
//            int res = QMessageBox::warning(this, tr("Warning"),
//                                 options::WARNING_TEXT_FILE_NOT_FOUND,
//                                 QMessageBox::Ok, QMessageBox::Cancel);
//            if (res == QMessageBox::Cancel) {
//                return "";
//            }
//            path_to_text = select_text();
//     }
//    // all other cases:
//    QFileInfo txtFile_info(path_to_text);
//    QString directory = txtFile_info.absolutePath();
//    QString filename_base = txtFile_info.completeBaseName();
//    QString ext = txtFile_info.completeSuffix();
//    QString wavFilenameBase = filename_base+".wav";
//    QString wavFile = user->getUserAudioDir() +options::PATH_SEP+ wavFilenameBase;
//    if (ext == "stm") {
//            QString filename_base = txtFile_info.completeBaseName();
//            stmFile = path_to_text;
//    }
//    else if (ext == "txt") {
//            stmFile = user->getUserStmDir()+ options::PATH_SEP + filename_base +".stm";
//            //case 1.b , 2.a.II
//            if (!QFile::exists(stmFile))
//            {
//                stm->initNewSTM(user->getUsername(), wavFilenameBase, path_to_text,stmFile);
//                return  wavFile;
//            }
//    }
//    //case 1.a
//    else if (QFile::exists(userstmFile)) {
//        stmFile = userstmFile;
//        QFileInfo stmFile_info(userstmFile);
//        filename_base = stmFile_info.completeBaseName();
//        wavFilenameBase = filename_base+".wav";
//        wavFile = user->getUserAudioDir() +options::PATH_SEP+ wavFilenameBase;
//    }
//    //case 2.b.II
//    while (!QFile::exists(wavFile)) {
//            cout << "wav file not found!" << endl;
//            int res = QMessageBox::warning(this, tr("Warning"),
//                                 options::WARNING_WAV_FILE_NOT_FOUND,
//                                 QMessageBox::Ok, QMessageBox::Cancel);
//            if (res == QMessageBox::Cancel) {
//                return "";
//            }
//            wavFile = QFileDialog::getOpenFileName(this, tr("Open Directory"), directory,
//                                                            tr("WAV files (*.wav)"));
//            QFileInfo wavFile_info(wavFile);
//            wavFilenameBase = wavFile_info.completeBaseName();
//    }

//    stm->initExistedSTM(user->getUsername(),wavFilenameBase,stmFile);
//    return  wavFile;
//}

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
    key_space = new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_Space), ui->centralWidget);
    //key_enter = new QShortcut(QKeySequence(Qt::Key_Return), ui->centralWidget);
    key_shift = new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_P), ui->centralWidget);
    key_ctrl_s = new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_S), ui->centralWidget);
//    QShortcut key_space(QKeySequence(Qt::Key_Space), ui->centralWidget);
    editor= new EditDialog(this);

    connect(key_right, SIGNAL(activated()), this, SLOT(key_next()));
    connect(key_down, SIGNAL(activated()), this, SLOT(key_next()));
    connect(key_left, SIGNAL(activated()), this, SLOT(key_previous()));
    connect(key_up, SIGNAL(activated()), this, SLOT(key_previous()));
    connect(ui->up, SIGNAL(clicked()), this, SLOT(key_previous()));
    connect(ui->down, SIGNAL(clicked()), this, SLOT(key_next()));
    connect(ui->save_btn, SIGNAL(clicked()), this, SLOT(save_table()));

//    connect(ui->rec, SIGNAL(clicked()), this, SLOT(on_rec_clicked()));
    connect(key_space, SIGNAL(activated()), this, SLOT(key_space_pressed()));
    connect(key_ctrl_s, SIGNAL(activated()), ui->save_btn, SIGNAL(clicked()));
//    connect(key_enter, SIGNAL(activated()), this, SLOT(key_enter_pressed()));
    connect(key_shift, SIGNAL(activated()), this, SLOT(key_shift_pressed()));
//    connect(ui->table,SIGNAL(cellChanged(int, int)),this,SLOT(cellChangedCallback()));
    connect(ui->table,SIGNAL(cellDoubleClicked(int, int)),this,SLOT(cellClickedCallback(int,int)));
    connect(editor,SIGNAL(apply()),this,SLOT(editorApply()));

    resize(1900,1300);
    this->setWindowTitle("DaCToR");

    // show the main window
    MainWindow::MainWindow::show();

    User* newUser = log_in(true);
    user =  (newUser!=nullptr) ? newUser:user;
    stm = new StmManager(ui->table);
    recStateMachine = new RecStateMachine(ui, stm);
    if (!initWindow(user, ""))
        return;
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
    User *newUser = log_in(false);
    if (newUser!=nullptr)
    {
        if (initWindow(newUser,""))
            user=newUser;
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
        ui->table->setCurrentCell(ui->table->currentRow(), stm->STM_UTT_IDX);
        ui->rec->setChecked(false);
    }
    else {
        ui->table->setCurrentCell(ui->table->currentRow()+1, stm->STM_UTT_IDX);
    }
    ui->table->setFocus();

}

void MainWindow::key_previous()
{
    bool isEndReached = end_of_table(true);
    qDebug()<<"MainWindow::key_previous(): isEndReached "<< isEndReached;

    if (isEndReached){
        ui->table->setCurrentCell(ui->table->currentRow(), stm->STM_UTT_IDX);
        ui->rec->setChecked(false);
    }
    else{
        ui->table->setCurrentCell(ui->table->currentRow()-1, stm->STM_UTT_IDX);
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

void MainWindow::key_space_pressed()
{
    ui->rec->click();
}

bool MainWindow::save_table()
{
    return stm->toSTM();
}
//void MainWindow::key_enter_pressed()
//{
//    key_next();
//    stm->toSTM();
//}

void MainWindow::key_shift_pressed()
{
    ui->play->click();
}

void MainWindow::cellClickedCallback(int r, int c)
{
    qDebug()<<"cell clicked";
    this->editedItem= ui->table->item(r,c);
    editor->setText(this->editedItem->text(),static_cast<Qt::Alignment>(this->editedItem->textAlignment()),ui->table->font());
    editor->show();


    //ui->table->setFocus();
}
void MainWindow::editorApply()
{
    QString text = editor->getText();
    this->editedItem->setText(text);
}
void MainWindow::cellChangedCallback()
{
    //stm->toSTM();
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
    //ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
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


void MainWindow::on_ltr_clicked()
{
    for (int r=0;r<ui->table->rowCount();++r) {
        if (ui->table->columnCount()<=stm->STM_UTT_IDX) continue;
        ui->table->item(r, stm->STM_UTT_IDX)->setTextAlignment(Qt::AlignLeft);
    }
}

void MainWindow::on_rtl_clicked()
{
    for (int r=0;r<ui->table->rowCount();++r) {
        if (ui->table->columnCount()<=stm->STM_UTT_IDX) continue;
        ui->table->item(r, stm->STM_UTT_IDX)->setTextAlignment(Qt::AlignRight);
    }
}
