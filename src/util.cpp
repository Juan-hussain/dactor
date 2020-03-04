#include"../headers/util.h"



bool util::readColsFromCSV(QFile& csvFile,QString csv_sep,QList<QStringList>& cols,int num_col, QString& msg)
{
    QStringList tokens;
    QTextStream in(&csvFile);
    in.setCodec(QTextCodec::codecForName("UTF-8"));
    QString item;
    while (!in.atEnd()) {
        QString line = in.readLine();
        tokens = line.split(csv_sep);
        for (int i=0;i<num_col;i++) {
            if (i<tokens.size()) item = tokens[i];
            else if (i!=0) item = "";
            else if (i==0) break;
            if(i<cols.size())
                cols[i].append(item);
            else
                cols.append(QStringList{item});
        }
    }
    csvFile.close();
    return true;

}
bool util::readColsFromCSV(QString csvFilename,QString csv_sep,QList<QStringList>& cols,int num_col,QString& msg)
{
        QFile csvFile(csvFilename);
        if (!csvFile.open(QIODevice::ReadOnly)) {
            msg = csvFile.errorString();
            return false;
        }
    return readColsFromCSV(csvFile, csv_sep,cols,num_col,msg);
}

bool util::writeColsToCSV(QFile& csvFile,QString csv_sep,QList<QStringList>& cols,QString& msg)
{
    if (cols.size()==0) {
        return false;
    }
    QString textData;
    int rows = cols[0].size();
    int columns =cols.size();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (cols[j].isEmpty()) continue;
            textData += cols[j].at(i).toUtf8();
            textData += csv_sep;
        }
        textData += "\r\n";             // (optional: for new line segmentation)
    }

//    if(!csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
//        msg = "writeColsToCSV() "+ csvFile.errorString();
//        return false;
//    }
    QTextStream out(&csvFile);
    out.setCodec(QTextCodec::codecForName("UTF-8"));
    out << textData.toUtf8();
    csvFile.close();
    return true;
}

bool util::writeColsToCSV(QString csvFilename,QString csv_sep,QList<QStringList>& cols,QString& msg)
{
    QFile csvFile(csvFilename);
    if (!csvFile.open(QIODevice::WriteOnly|QIODevice::Truncate)) {
        msg = csvFile.errorString();
        return false;
    }
    return writeColsToCSV(csvFile, csv_sep,cols,msg);
}

