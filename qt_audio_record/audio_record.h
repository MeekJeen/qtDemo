#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QtCore/QObject>
#include <QtCore/QIODevice>
#include <QAudioInput>

struct WavFileHead
{
    char RIFFNAME[4];
    unsigned int nRIFFLength;
    char WAVNAME[4];
    char FMTNAME[4];
    unsigned int nFMTLength;
    unsigned short nAudioFormat;
    unsigned short nChannleNumber;
    unsigned int nSampleRate;
    unsigned int nBytesPerSecond;
    unsigned short nBytesPerSample;
    unsigned short   nBitsPerSample;
    char   DATANAME[4];
    unsigned int  nDataLength;
};


class DataSource : public QIODevice
{
    Q_OBJECT
public:
    explicit DataSource(QObject *parent = nullptr);

    Q_INVOKABLE void startRecord();
    Q_INVOKABLE void stopRecord();
    Q_INVOKABLE void saveAsWavFile();

protected:
    qint64 readData(char * data, qint64 maxSize);
    qint64 writeData(const char * data, qint64 maxSize);
private:
    QAudioInput* m_audioInput;
    WavFileHead  m_wavFileHead;
    QByteArray   m_audioData;

};


#endif // DATASOURCE_H
