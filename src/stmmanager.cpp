#include "../headers/stmmanager.h"

StmManager::StmManager(QTableWidget *tw)
{
    this->tw = tw;
}
bool StmManager::initExistedSTM(QString spk,QString wav,QString newSTMFile)
{
    this->spk = spk;
    this->wav = wav.replace(' ','_');
    this->stmFilename = newSTMFile;

    QFileInfo stmFile_info(stmFilename);

    tw->setRowCount(0);
    if (stmFile_info.exists()) {
        return fromSTM();
    }
    return false;
}

bool StmManager::initNewSTM(QString spk,QString wav, QString txtFile, QString newSTMFile)
{

    this->spk = spk;
    QFileInfo wavFile_info(wav.replace(' ','_'));
    this->wav = wavFile_info.completeBaseName();
    this->stmFilename = newSTMFile;

    QFileInfo txtFile_info(txtFile);
    if (!txtFile_info.exists()) {
        qDebug() <<"File does not exists: " <<txtFile;
        return false;
    }

    //QString baseName = txtFile_info.completeBaseName();

    qDebug() << stmFilename;
    qDebug()<< "no stm, make one!";
    tw->setRowCount(0);
    return loadText(txtFile);

}

QString StmManager::get_uttid(int row)
{
    Q_ASSERT(row<tw->rowCount());
    //qDebug() << STM_WAV_IDX;
    QString wav_ = tw->item(row,STM_WAV_IDX)->text();
    QString spk_ = tw->item(row,STM_SPK_IDX)->text() ;
    QString from_= tw->item(row,STM_FROM_IDX)->text();
    QString to_  = tw->item(row,STM_TO_IDX)->text();
    //QString from_= QString("%1").arg(tw->item(row,STM_FROM_IDX)->text().toDouble(), 2,2, 'g', '0');
    //QString to_= QString("%1").arg(tw->item(row,STM_TO_IDX)->text().toDouble(), 2,2, 'g', '0');
    return get_uttid(wav_, spk_, from_, to_);
}
QString StmManager::get_uttid(QString wav,QString spk,QString from,QString to)
{
    QString from_= QString::number(from.toDouble(), 'f', 2).rightJustified(8,'0').replace(".","");
    QString to_  = QString::number(to.toDouble(), 'f', 2).rightJustified(8,'0').replace(".","");
    return wav + "_" + spk + "_" + from_ + "_" + to_;
}
bool StmManager::fromSTM()//QTableWidget *& tw, QString stmFilename)
{
    QFile stmFile(stmFilename);
    if (!stmFile.open(QIODevice::ReadOnly)) {
        qDebug() <<"fromSTM()"<<stmFilename <<stmFile.errorString();
        return false;
    }
    tw->setColumnCount(STM_COL);
    //    tw->setRowCount(5);
    QStringList tokens;
    QTextStream in(&stmFile);
    in.setCodec(QTextCodec::codecForName("UTF-8"));

    int r = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        tokens = line.split(CSV_SEP);
        QStringList utt_l;
        for (int i = STM_COL-1; i < tokens.size(); ++i) {
            utt_l.append(tokens[i]);
        }
        QString utt = utt_l.join(CSV_SEP).toUtf8();
        tw->setRowCount(tw->rowCount() + 1);
        for(int c = 0; c< STM_COL -1 ;c++)
        {
            tw->setItem(r,c,new QTableWidgetItem(tokens[c]));
        }
        tw->setItem(r,STM_UTT_IDX,new QTableWidgetItem(utt));
        tw->item(r, STM_UTT_IDX)->setTextAlignment(Qt::AlignLeft);

        r++;
    }
    //tw->resizeColumnsToContents();
    stmFile.close();
    return true;
}
bool StmManager::toSTM()//QTableWidget *&tw, QString stmFilename)
{
    QString textData;
    int rows = tw->rowCount();
    int columns = tw->columnCount();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {

            textData += tw->item(i,j)->text().toUtf8();
            textData += CSV_SEP;
        }
        textData += "\r\n";             // (optional: for new line segmentation)
    }

    QFile stmFile(stmFilename);
    if(!stmFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() <<"toSTM()"<<stmFilename<< stmFile.errorString();
        return false;
    }
    QTextStream out(&stmFile);
    out.setCodec(QTextCodec::codecForName("UTF-8"));
    out << textData.toUtf8();
    //tw->resizeColumnsToContents();
    stmFile.close();
    return true;
}
bool StmManager::cleanToSTM(AudioRecorder* recorder,QString wavFile)//QTableWidget *&tw, QString stmFilename)
{
    QString textData, stringFrom,stringTo,wav,spk;
    int rows = tw->rowCount();
    int columns = tw->columnCount();
    double from, to, newFrom, newTo;
    recorder->clearSaveBuff();

    for (int i = 0; i < rows; i++) {

        //cleaning+++++++++++++
        from = tw->item(i,STM_FROM_IDX)->text().toDouble();
        to = tw->item(i,STM_TO_IDX)->text().toDouble();
        if (static_cast<int>(to)!=0) {
            newFrom = recorder->getSaveBufferPos()/recorder->getBytePerSec();
            if (!recorder->savePart2Buff(from,to)){ continue;}
            newTo = recorder->getSaveBufferPos()/recorder->getBytePerSec();
            //+++++++++++++++++++++++
            QString *tokens = new QString[STM_COL];
            Q_ASSERT(columns==STM_COL);

            for (int j = 0; j < columns; j++) {

                if (j==STM_FROM_IDX){
                    stringFrom=QString::number(newFrom, 'f', 2);
                    tokens[STM_FROM_IDX] =stringFrom;
                } else if (j==STM_TO_IDX){
                    stringTo = QString::number(newTo, 'f', 2);
                    tokens[STM_TO_IDX] =stringTo;
                } else if (j==STM_SPK_IDX){
                    spk = tw->item(i,j)->text().toUtf8();
                    tokens[STM_SPK_IDX] =spk;
                } else if (j==STM_WAV_IDX){
                    wav = tw->item(i,j)->text().toUtf8();
                    tokens[STM_WAV_IDX] =wav;
                } else{
                    tokens[j] = tw->item(i,j)->text().toUtf8();
                }
            }
            tokens[STM_UTTID_IDX] = get_uttid(wav,spk,stringFrom,stringTo);
            for (int i=0;i<STM_COL;++i) {
                textData += tokens[i] +CSV_SEP;
            }
        } else {
            for (int j = 0; j < columns; j++) {
                textData += tw->item(i,j)->text().toUtf8();
                textData += CSV_SEP;
            }
        }
        textData += "\r\n";             // (optional: for new line segmentation)
    }
    if (recorder->getSaveLength()<10 /*second*/) {
        qDebug()<<"no clean, length <10 s";
        return false;
    }
    qDebug()<<"recorder->getRecLength()"<<recorder->getRecLength();
    qDebug()<<"recorder->getSaveLength()"<<recorder->getSaveLength();
    double deletePercent = 100 - recorder->getSaveLength()*100/recorder->getRecLength();
    if(deletePercent>1){
        int res = QMessageBox::question(tw ,"Warning",
                                        "about <font color=\"red\" size=\"+5\">" + QString::number(deletePercent,'f',0) + "%</font> of the file will be cleaned while deleting the unused segments. If you cancel you can do it later by open the stm again. The cleaning is then run when closing it",
                                        QMessageBox::Ok, QMessageBox::Cancel);
        if (res == QMessageBox::Cancel) {
            return false;
        }
    } else {
        return false;
    }
    recorder->saveBuff2file(wavFile);

    QFile stmFile(stmFilename);
    if(!stmFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() <<"toSTM()"<<stmFilename<< stmFile.errorString();
        return false;
    }
    QTextStream out(&stmFile);
    out.setCodec(QTextCodec::codecForName("UTF-8"));
    out << textData.toUtf8();
    stmFile.close();
    return true;
}

bool StmManager::loadText(QString txtFilename)
{
    QFile txtFile(txtFilename);
    if (!txtFile.open(QIODevice::ReadOnly)) {
        qDebug() <<"loadText()"<<txtFilename << txtFile.errorString();
        return false;
    }

    QTextStream in(&txtFile);
    in.setCodec(QTextCodec::codecForName("UTF-8"));

    // read the countent of the text file
    QString text = in.readAll().toUtf8();

    QRegExp rx1("(\\s*\\n+\\s*)+$");
    QRegExp rx2("(\\s*\\n+\\s*)+");

    text.replace(rx1, "");
    text.replace(rx2, "\n");

    in.reset();
    in.setString(&text, QIODevice::ReadWrite);

    int row = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        //    QStringList utt_list = txtFile.split("\n");
        //    for (int var = 0; var < utt_list.size(); ++var) {
        updateTable(row,STM_UTT_IDX,line);
        // alighnment: right for arabic and left for latine)
        row++;
    }
    return toSTM();
}

bool StmManager::updateTable(int row,int col, QString item)
{
    int nRow = tw->rowCount();
    if (row >= nRow) { //new row
        tw->setRowCount(tw->rowCount() + 1);
        tw->setColumnCount(STM_COL);
        tw->setItem(row,STM_SPK_IDX,new QTableWidgetItem(spk));
        tw->setItem(row,STM_WAV_IDX,new QTableWidgetItem(wav));
        tw->setItem(row,STM_FROM_IDX,new QTableWidgetItem(INIT_FROM));
        tw->setItem(row,STM_TO_IDX,new QTableWidgetItem(INIT_TO));
        // tw->setItem(row, 6, new QTableWidgetItem("test"));
    }
    if (col == STM_FROM_IDX||col==STM_TO_IDX) {
        //item = QString("%1").arg(item.toDouble(), 2, 'g', '0');
        item = QString::number(item.toDouble(), 'f', 2);
    }
    tw->setItem(row,col,new QTableWidgetItem(item));
    tw->setItem(row,STM_UTTID_IDX,new QTableWidgetItem(get_uttid(row)));

    //    tw->item(row, STM_FROM_IDX)->setFlags(Qt::ItemIsEditable);
    //    tw->item(row, STM_TO_IDX)->setFlags(Qt::ItemIsEditable);
    return true;
}

bool StmManager::updateTableAndStm(int row,int col, QString item)
{
    updateTable(row,col,item);
    //toSTM(tw, this->stmFilename);
    return toSTM();
}

bool StmManager::isRecorded(int row)
{
    if (row< tw->rowCount()&& tw->item(row,STM_TO_IDX)) {
        return tw->item(row,STM_TO_IDX)->text().toFloat()>0;
    } else {
        return false;
    }
}
QString StmManager::getStmFilename() const
{
    return stmFilename;
}
