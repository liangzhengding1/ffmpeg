#include "xplay.h"
#include <QFileDialog>
#include <QMessageBox>
#include "XFFmpeg.h"
#include "XAudioPlay.h"
static bool isPressSlider = false;
static bool isPlay = true;

XPlay::XPlay(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
	startTimer(40);
	//openFile("video.mp4");
}
void XPlay::resizeEvent(QResizeEvent *e)
{
	ui.openGLWidget->resize(size());
	ui.playButton->move(this->width() / 2 + 50, this->height() - 80);
	ui.openButton->move(this->width() / 2 - 50, this->height() - 80);
	ui.playslider->move(25, this->height() - 120);
	ui.playslider->resize(this->width() - 50, ui.playslider->height());
	ui.playtime->move(25, ui.playButton->y());
	ui.sp->move(ui.playtime->x() + ui.playtime->width() + 5, ui.playtime->y());
	ui.totaltime->move(150, ui.playButton->y());
}

void XPlay::timerEvent(QTimerEvent *e)//QTimerEvent定时器，定时出发事件
{
	//计算当前播放时间的分钟和秒数
	int min = (XFFmpeg::Get()->pts /1000)/60;
	int sec = (XFFmpeg::Get()->pts / 1000) % 60;
	//将分钟和秒数格式化为mm:ss形式的字符串
	char buf[1024] = { 0 };
	sprintf(buf, "%03d:%02d", min, sec);
	//设置playtime的文本为格式化后的时间字符串
	ui.playtime->setText(buf);

	if (XFFmpeg::Get()->totalMs > 0)
	{
		 //计算播放进度的百分比
		float rate = (float)XFFmpeg::Get()->pts / (float)XFFmpeg::Get()->totalMs;
		//如果滑块没有被拖动
		if (!isPressSlider)
			ui.playslider->setValue(rate * 1000);//设置播放进度滑块的位置
	}

}

//滑块是否被移动
void XPlay::sliderPress()
{
	isPressSlider = true;
}

void XPlay::sliderRelease()
{
	isPressSlider = false;
	float pos = 0;
	pos = (float)ui.playslider->value() / (float)(ui.playslider->maximum() + 1);// 计算滑块当前位置的百分比
	XFFmpeg::Get()->Seek(pos);// 调用 Seek 函数根据计算的位置调整播放进度
}

void XPlay::play()
{
	isPlay = !isPlay;
	XFFmpeg::Get()->isPlay = isPlay;
	if (isPlay)
	{
		//pause
		ui.playButton->setStyleSheet(PAUSE);
	}
	else
	{
		ui.playButton->setStyleSheet(PLAY);
	}

}
void XPlay::openFile(QString name)
{
	if (name.isEmpty())
		return;
	this->setWindowTitle(name);
	int totalMs = XFFmpeg::Get()->Open(name.toLocal8Bit());
	if (totalMs <= 0)
	{
		QMessageBox::information(this, "err", "file open failed!");
		return;
	}
	XAudioPlay::Get()->sampleRate = XFFmpeg::Get()->sampleRate;
	XAudioPlay::Get()->channel = XFFmpeg::Get()->channel;
	XAudioPlay::Get()->sampleSize = 16;
	XAudioPlay::Get()->Start();
	char buf[1024] = { 0 };
	int min = (totalMs / 1000) / 60;
	int sec = (totalMs / 1000) % 60;
	sprintf(buf, "%03d:%02d", min, sec);
	ui.totaltime->setText(buf);
	isPlay = false;
	play();
}
void XPlay::open()
{
	QString name = QFileDialog::getOpenFileName(
		this, QString::fromLocal8Bit("ѡ����Ƶ�ļ�"));
	openFile(name);
}
XPlay::~XPlay()
{

}
