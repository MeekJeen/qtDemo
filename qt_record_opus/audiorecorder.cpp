#include <iostream>
#include <QAudioRecorder>
#include <QApplication>
#include <QUrl>

QAudioRecorder *m_audioRecorder = nullptr;
QString OutputName = "/home/111.opus";

void startRecord(){
	QStringList audioInputList = m_audioRecorder->audioInputs();
	QStringList audioCodecsList = m_audioRecorder->supportedAudioCodecs(); 

	m_audioRecorder->setAudioInput(audioInputList[0]);//default mic

	QAudioEncoderSettings settings;
	settings.setCodec("audio/x-opus");	
	settings.setSampleRate(16000);
	settings.setChannelCount(2);
	settings.setQuality(QMultimedia::HighQuality);
	m_audioRecorder->setEncodingSettings(settings);
	m_audioRecorder->record();//start record
}

void setOutputLocation(){
	qDebug() << "OutputName =" << OutputName;
	m_audioRecorder->setOutputLocation(QUrl::fromLocalFile(OutputName));
}

void getCodecAndDeviceParameter(){
	QStringList audioInputList = m_audioRecorder->audioInputs(); //audioInputList[0]为麦克风
	for(int i = 0; i < audioInputList.size(); i++)
		std::cout << "input_name[" << i << "] = "  <<  audioInputList[i].toStdString() << std::endl;
	std::cout << std::endl;
	QStringList audioCodecsList = m_audioRecorder->supportedAudioCodecs();      //audioCodecsList[0]为 audio/pcm
	for(int i = 0; i < audioCodecsList.size(); i++)
		std::cout << "audio_codec_name[" << i << "] = "  <<  audioCodecsList[i].toStdString() << std::endl;

	QStringList audioContainers = m_audioRecorder->supportedContainers();
	for(int i = 0; i < audioContainers.size(); i++)
		std::cout << "container_name[" << i << "] = "  <<  audioContainers[i].toStdString() << std::endl;

	QList<int> audioSample = m_audioRecorder->supportedAudioSampleRates();
	printf("xxx------------------>line = %d, audioSample = %d\n",__LINE__,audioSample.size());
	for(int i = 0; i < audioSample.size(); i++)
		std::cout << "i = " <<  i << std::endl;

	QStringList videoCodecList =  m_audioRecorder->supportedVideoCodecs();
	for(int i = 0; i < videoCodecList.size(); i++)
		std::cout << "video_codec_name[" << i << "] = "  <<  videoCodecList[i].toStdString() << std::endl;
}

int main(int argc, char *argv[]){
	QApplication app(argc, argv);
	m_audioRecorder = new QAudioRecorder();
	//1.get parameter
	getCodecAndDeviceParameter();
	
	//2.set output filename
	setOutputLocation();
	
	//3.start Record 
  startRecord();	
	return app.exec();
}
