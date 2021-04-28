#include "audio_record.h"
#include <QApplication>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QQuickView>
#include <QAudioInput>
#include <unistd.h>

DataSource::DataSource( QObject *parent) :QIODevice(parent){}

qint64 DataSource::readData(char * data, qint64 maxSize){
	Q_UNUSED(data)
		Q_UNUSED(maxSize)
		return -1;
}

qint64 DataSource::writeData(const char * data, qint64 maxSize){
	m_audioData.append(data, maxSize);
	return maxSize;
}

void DataSource::startRecord(){
	int sampleRate = 16000;
	int channels = 2;
	int sampleSize = 16;

	QAudioFormat formatAudio;
	formatAudio.setSampleRate(sampleRate);
	formatAudio.setChannelCount(channels);
	formatAudio.setSampleSize(sampleSize);
	formatAudio.setCodec("audio/pcm");
	formatAudio.setByteOrder(QAudioFormat::LittleEndian);
	formatAudio.setSampleType(QAudioFormat::UnSignedInt);

	QAudioDeviceInfo inputDevices = QAudioDeviceInfo::defaultInputDevice();

	if (!inputDevices.isFormatSupported(formatAudio)) {
		qWarning() << "Default format not supported, trying to use the nearest.";
		formatAudio = inputDevices.nearestFormat(formatAudio);
	}

	memcpy(m_wavFileHead.RIFFNAME, "RIFF", 4);
	memcpy(m_wavFileHead.WAVNAME, "WAVE", 4);
	memcpy(m_wavFileHead.FMTNAME, "fmt ", 4);
	memcpy(m_wavFileHead.DATANAME, "data", 4);

	m_wavFileHead.nRIFFLength = 36;
	m_wavFileHead.nFMTLength = sampleSize;
	m_wavFileHead.nAudioFormat = 0x01;
	m_wavFileHead.nChannleNumber = channels;
	m_wavFileHead.nSampleRate = sampleRate;
	m_wavFileHead.nBytesPerSecond = (sampleSize / 8) * channels * sampleRate;
	m_wavFileHead.nBytesPerSample = (sampleSize / 8) * channels;
	m_wavFileHead.nBitsPerSample = sampleSize;
	m_wavFileHead.nDataLength = 0;

	m_audioInput = new QAudioInput(inputDevices, formatAudio);
	auto io = m_audioInput->start();
	//1.OK
	// connect(io, &QIODevice::readyRead,
	// 				[&, io]() {
	// 					qint64 len = m_audioInput->bytesReady();
	// 					printf("len = %lld\n",len);
	// 					const int BufferSize = 4096;
	// 					if (len > BufferSize)
	// 						len = BufferSize;

	// 					QByteArray buffer(len, 0);
	// 					qint64 l = io->read(buffer.data(), len);
	// 					printf("l = %lld\n",l);
	// 					if (l > 0)
	// 						writeData(buffer.constData(), l);
	// 					saveAsWavFile();
	// 				});
	
	//2.OK
	while(1){
		qint64 len = m_audioInput->bytesReady();
		const int BufferSize = 4096;
		if (len > BufferSize)
			len = BufferSize;

		QByteArray buffer(len, 0);
		qint64 l = io->read(buffer.data(), len);
		if (l > 0){
			printf(" read data, len = %lld\n",l);
			writeData(buffer.constData(), l);
		}
		saveAsWavFile();
		usleep(50);
	}	 
}

void DataSource::stopRecord(){
	m_audioInput->stop();
}

int m_header = 0;
void DataSource::saveAsWavFile(){
	QFile f("/data/test.pcm");
	bool bisOk = f.open(QIODevice::ReadWrite);
	if(bisOk == true){
		// if(m_header == 0){
		// 	m_wavFileHead.nDataLength = m_audioData.size() / 2;
		// 	f.write((char *)&m_wavFileHead, sizeof(WavFileHead));
		// 	m_header = 1;
		// }
		f.write(m_audioData.data(), m_audioData.size());
		f.close();
	}
}

int main(int argc, char *argv[]){
	QCoreApplication app(argc, argv);
	DataSource* dataSource = new DataSource();
	
	dataSource->startRecord();	
	return app.exec();
}
