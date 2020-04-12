#include "headers/audiorecorder.h"


AudioRecorder::AudioRecorder()
{
}

void AudioRecorder::init()
{
    format.setSampleRate(SampleRate);
    format.setChannelCount(1);
    format.setSampleSize(SampleSize);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    audioInput.reset(new QAudioInput(format));
    audioOutput.reset(new QAudioOutput(format));

    wrBuff.open(QBuffer::WriteOnly);
    rdBuff.open(QBuffer::ReadOnly);
    playBuff.open(QBuffer::ReadWrite);
    rerecBuff.open(QBuffer::ReadWrite);
    saveBuff.open(QBuffer::ReadWrite);
}


void AudioRecorder::appendBuff()
{
    //qDebug()<<"exit rec";
    rdBuff.buffer().append(wrBuff.buffer());
    wrBuff.buffer().clear();
    wrBuff.seek(0);
}

void AudioRecorder::record()
{

    // define a connection to save the audio in the reading buffer
    QObject::connect(&wrBuff, SIGNAL(readyRead()), this, SLOT(appendBuff()));
  //  QObject::connect(&wrBuff, SIGNAL(byteWritten), this, SLOT(appendBuff()));

    //QObject::connect(&wrBuff, &QIODevice::bytesWritten, this, SLOT(appendBuff(qint64 bytes)));

    // start saving audio data in writing buffer
    audioInput->start(&wrBuff);

    isRecording = true;
    finished = false;
}

void AudioRecorder::pause()
{
    if (!isChecked)
    {
        audioInput->stop();
        isChecked = true;
        isRecording = false;
    }
    else
    {
        audioInput->start(&wrBuff);
        isChecked = false;
        isRecording = true;
    }
}

void AudioRecorder::play(int start_buff_idx, int end_buff_idx)
{
    isRecording = false;

    // rest index to start position
    playBuff.buffer().clear();

    int length = abs(end_buff_idx - start_buff_idx);

    // copy the content to be played in the buffer
    playBuff.buffer() = rdBuff.buffer().mid(start_buff_idx, length);

    playBuff.seek(0);

    // play content of buffer
    audioOutput->start(&playBuff);

    playing = true;

}
void AudioRecorder::play(double from, double to)
{
    int byteFrom, byteTo;
    second2byte(from, to, byteFrom,byteTo);
    play(byteFrom,byteTo);
}

void AudioRecorder::pausePlaying()
{
    if (playing)
    {
        try {
            audioOutput->stop();
        } catch (exception e) {
            qDebug()<<"pausePlaying threw exception: "<<e.what();
        }
        playing = false;
    }
}

void AudioRecorder::stop()
{
    // stop recording
    audioInput->stop();
    isRecording = false;
}

bool AudioRecorder::savePart2Buff(int start_buff_idx, int end_buff_idx)
{
    if (start_buff_idx>=end_buff_idx){
        qDebug()<<"not valid time interval to save";
        return false;
    }
    saveBuff.buffer().insert(static_cast<int>(saveBuff.size()),rdBuff.data().mid(start_buff_idx,end_buff_idx-start_buff_idx+1));
    return true;
}
bool AudioRecorder::savePart2Buff(double from, double to)
{
    int byteFrom, byteTo;
    second2byte(from, to, byteFrom,byteTo);
    return savePart2Buff(byteFrom,byteTo);
}
bool AudioRecorder::saveBuff2file(QString path)
{
    // define the file to save the data
    m_file = new WavePCMFile(path, audioInput->format());
    qDebug()<< path;
    // open file for writing
    m_file->open();

    // whrite the buffer countent in the file
    m_file->write(saveBuff.buffer());

    // close the file
    m_file->close();

    return true;
}
void AudioRecorder::correctByteIndex(int& byteFrom,int& byteTo)
{
    if(byteFrom&0x0001) byteFrom = (byteFrom>0)?byteFrom-1:byteFrom;
    if(!(byteTo&0x0001)) byteTo =(byteTo>0)?byteTo-1:byteTo;//if zero keep it as it is
}
void AudioRecorder::second2byte(double from, double to,int& byteFrom,int& byteTo)
{
    byteFrom = static_cast<int>(from * getBytePerSec());
    byteTo = static_cast<int>(to * getBytePerSec());
    if(byteFrom&0x0001) byteFrom = (byteFrom>0)?byteFrom-1:byteFrom;
    if(!(byteTo&0x0001)) byteTo =(byteTo>0)?byteTo-1:byteTo;//if zero keep it as it is
}
void AudioRecorder::clearSaveBuff()
{
    // clear saving buffer
    saveBuff.buffer().clear();
    saveBuff.seek(0);
}

double AudioRecorder::getRecLength()
{
    long long newLength = rdBuff.size();
    return newLength/bytePerSec;
}

double AudioRecorder::getSaveLength()
{
    long long newLength = saveBuff.size();
    return newLength/bytePerSec;
}
void AudioRecorder::save(QString path)
{
    clearSaveBuff();
    // copy content to save to the saving buffer
    saveBuff.buffer() = rdBuff.buffer();
    saveBuff2file(path);
    finished = true;
}

void AudioRecorder::replace_start(int start_buff_idx, int end_buff_idx)
{
//    if (!isReplacing)
//    {
        // start recording
        audioInput->start(&rerecBuff);

        int length = abs(end_buff_idx - start_buff_idx);

        // remove the undesired section
        rdBuff.buffer().remove(start_buff_idx, length);

        isReplacing = true;

        isRecording = true;
  //)  }

}

long int AudioRecorder::replace_finish(int start_buff_idx, int end_buff_idx)
{
//    if (isReplacing)
//    {
        // stop recording
        audioInput->stop();

        long long newLength = rerecBuff.size();

        // insert the recorded piece in the buffer
        rdBuff.buffer().insert(start_buff_idx, rerecBuff.buffer());

        // clear the rerrecording buffer
        rerecBuff.buffer().clear();
        rerecBuff.seek(0);

        // calculate the resulting timeshift
        long int previousLength = abs(end_buff_idx - start_buff_idx);

        isReplacing = false;

        isRecording = false;

        // return the time shift in milliseconds
        return (newLength - previousLength);
 //   }
}

bool AudioRecorder::isBufferEmpty()
{
    return (rdBuff.size() == 0);
}

long long AudioRecorder::getBufferPos()
{
    return rdBuff.size();
}
long long AudioRecorder::getSaveBufferPos()
{
    return saveBuff.size();
}

void AudioRecorder::load_audio(QString path)
{
    rdBuff.buffer().clear();
    rdBuff.seek(0);

    QFile m_WAVFile;
    m_WAVFile.setFileName(path);
    if(m_WAVFile.exists()==false)
    {
        qDebug()<<"File doesn't exist";
        return;
    }
    m_WAVFile.open(QIODevice::ReadWrite);

    char strm[4];
    char s[1];
   // QByteArray wav;
    quint32 conv;

    qDebug()<<"\nstart";
    qDebug()<<m_WAVFile.read(4);//RIFF
    // m_WAVHeader.RIFF = m_WAVFile.read(4).data();

    m_WAVFile.read(strm,4);//chunk size
    qDebug()<<qFromLittleEndian<quint32>((uchar*)strm);

    m_WAVFile.read(strm,4);//format
    qDebug()<<strm;

    m_WAVFile.read(strm,4);//subchunk1 id
    qDebug()<<strm;

    m_WAVFile.read(strm,4);//subchunk1 size
    qDebug()<<qFromLittleEndian<quint32>((uchar*)strm);

    m_WAVFile.read(strm,2);//audio format
    qDebug()<<qFromLittleEndian<quint32>((uchar*)strm);

    m_WAVFile.read(strm,2);//NumChannels
    conv = qFromLittleEndian<quint32>((uchar*)strm);
    qDebug()<<conv;
    if(conv!=1)
    {
        qDebug()<<"Import wav file, Wav file must be mono";
       // QMessageBox::warning(this, "Import wav file", "Wav file must be mono",QMessageBox::Ok,QMessageBox::NoButton);
        return;
    }

    m_WAVFile.read(strm,4);//Sample rate
    conv = qFromLittleEndian<quint32>((uchar*)strm);
    qDebug()<<conv;
    if(conv!=SampleRate)
    {
        qDebug()<<"Import wav file, Use file with SampleRateHz sample rate for native sample rate";
        //QMessageBox::warning(this, "Import wav file", "Use file with 11025Hz sample rate for native sample rate",QMessageBox::Ok,QMessageBox::NoButton);
    }

    m_WAVFile.read(strm,4);//Byte rate
    qDebug()<<qFromLittleEndian<quint32>((uchar*)strm);

    m_WAVFile.read(strm,2);//Block Allign
    qDebug()<<qFromLittleEndian<quint32>((uchar*)strm);

    m_WAVFile.read(strm,2);//BPS
    conv = qFromLittleEndian<quint32>((uchar*)strm);
    qDebug()<<conv;
    if(conv!=SampleSize)
    {
        qDebug()<<"Import wav file, Wav file must be unsigned SampleSize bit";
        //QMessageBox::warning(this, "Import wav file", "Wav file must be unsigned 8 bit",QMessageBox::Ok,QMessageBox::NoButton);
        return;
    }

    m_WAVFile.read(strm,4);//subchunk2 id
    qDebug()<<strm;

    m_WAVFile.read(strm,4);//subchunk2 size
    qDebug()<<qFromLittleEndian<quint32>((uchar*)strm);

  //  outBuffLen[waveNum] = 0;
    while(!m_WAVFile.atEnd())
    {
        m_WAVFile.read(s,1);
        rdBuff.buffer().append(s[0]);
    }
    m_WAVFile.close();

//    dsBuffer[waveNum] = QAudioBuffer(wav,fmt);
//    outBuffLen[waveNum] = dsBuffer[waveNum].sampleCount();
    qDebug()<<" Processed:";
    //qDebug()<<outBuffLen[waveNum];
//    wavePath[waveNum] = wavFile;
//    QFileInfo fileInfo(wavFile);
//    waveName[waveNum] = fileInfo.fileName();
}

void AudioRecorder::reset()
{

    isChecked = false;
    isReplacing = false;
    isRecording = false;
    finished = false;

    rdBuff.buffer().clear();
    rdBuff.seek(0);

    wrBuff.buffer().clear();
    wrBuff.seek(0);

    playBuff.buffer().clear();
    playBuff.seek(0);

    rerecBuff.buffer().clear();
    rerecBuff.seek(0);
}

void AudioRecorder::setBytePerSec(double value)
{
    bytePerSec = value;
}

double AudioRecorder::getBytePerSec() const
{
    return bytePerSec;
}

int AudioRecorder::getSampleSize() const
{
    return SampleSize;
}

void AudioRecorder::setSampleSize(int value)
{
    SampleSize = value;
}

int AudioRecorder::getSampleRate() const
{
    return SampleRate;
}

void AudioRecorder::setSampleRate(int value)
{
    SampleRate = value;
}
