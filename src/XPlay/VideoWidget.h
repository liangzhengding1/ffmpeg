#pragma once
#include <QtWidgets/qwidget.h>
#include <QOpenGLWidget>
//绘制图像类
class VideoWidget:public QOpenGLWidget
{
public:
	VideoWidget(QWidget *p = NULL);
	void paintEvent(QPaintEvent *e);
	void timerEvent(QTimerEvent *e);
	virtual ~VideoWidget();
};

