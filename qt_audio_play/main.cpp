#include <QtCore/QCoreApplication>
#include <QtMultimedia/QMediaPlayer>
#include <QEventLoop>

int main(int argc, char *argv[]){
	QCoreApplication a(argc, argv);
	QEventLoop loop;
	QMediaPlayer * player = new QMediaPlayer; 
	player->setMedia(QUrl::fromLocalFile("/data/Young.mp3"));
	player->setVolume(50);
	player->play();
	
	loop.exec();//Or:loop.exec(QEventLoop::AllEvents);
	
	return a.exec();
}
