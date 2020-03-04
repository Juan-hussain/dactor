#ifndef WAVEPCMFILE_H
#define WAVEPCMFILE_H

#include<QString>
#include<QAudioFormat>
#include<QFile>
#include<QDataStream>

//class WavePCMFile
//{
//public:
//    WavePCMFile();
//};


class WavePCMFile:public QFile {
public:
    WavePCMFile(const QString & name, const QAudioFormat & format, QObject *parent = 0);
    bool open();
    void close();
    void write(QByteArray arr);

private:
    void writeHeader();
    bool hasSupportedFormat();
    QAudioFormat format;
};

#endif // WAVEPCMFILE_H

