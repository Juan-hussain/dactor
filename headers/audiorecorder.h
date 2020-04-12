#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include<iostream>
#include<QBuffer>
#include<QAudioInput>
#include<QAudioOutput>
#include<QtEndian>
#include<QDebug>
#include "headers/wavepcmfile.h"

using namespace std;

class AudioRecorder : public QObject
{
        Q_OBJECT
private:
    bool isRecording;
    bool isReplacing;
    bool finished;
    bool isChecked;
    bool playing;
    int SampleRate = 16000 ;
    int SampleSize = 16;
    double bytePerSec = SampleRate * SampleSize/8;
    WavePCMFile *m_file;
    QAudioFormat format;
    QScopedPointer<QAudioInput> audioInput;
    QScopedPointer<QAudioOutput> audioOutput;
    QBuffer rdBuff;
    QBuffer wrBuff;
    QBuffer playBuff;
    QBuffer rerecBuff;
    QBuffer saveBuff;

//private slots:

public:
    AudioRecorder();
    void init();
    void record();
    void pause();
    void stop();
    void reset();
    void play(int start_buff_idx, int end_buff_idx);
    void play(double from, double to);
    void replace_start(int start_buff_idx, int end_buff_idx);
    long int replace_finish(int start_buff_idx, int end_buff_idx);
    bool isRe_recording(){return isReplacing;}
    bool finishedRecording(){return finished;}
    bool isPaused(){return isChecked;}
    bool isPlaying(){return playing;}
    bool isBufferEmpty();
    void pausePlaying();
    bool savePart2Buff(int start_buff_idx, int end_buff_idx);
    bool savePart2Buff(double from, double to);

    void correctByteIndex(int& byteFrom,int& byteTo);
    void second2byte(double from, double to,int& byteFrom,int& byteTo);

    void save(QString path);
    long long getBufferPos();
    long long getSaveBufferPos();
    bool saveBuff2file(QString path);
    void clearSaveBuff();
    double getRecLength();
    double getSaveLength();
    void load_audio(QString path);

    double getBytePerSec() const;
    void setBytePerSec(double value);

    int getSampleSize() const;
    void setSampleSize(int value);

    int getSampleRate() const;
    void setSampleRate(int value);

    bool getGetIsRecording() const;

public slots:
    void appendBuff();

    //    void finishedPlaying();



};

#endif // AUDIORECORDER_H
