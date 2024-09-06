#include "XAudioPlay.h"
#include <QAudioOutput>
#include <QThread>
class CXAudioPlay :public XAudioPlay
{
public:
	QAudioOutput *output = NULL;
	QIODevice *io = NULL;
	QMutex mutex;
	//停止播放
	void Stop()
	{
		mutex.lock();
		if (output)
		{
			output->stop();
			delete output;
			output = NULL;
			io = NULL;
		}
		mutex.unlock();
	}
	bool Start()
	{
		Stop();//防止出错先暂停一次
		mutex.lock();
		QAudioFormat fmt;
		fmt.setSampleRate(this->sampleRate);//采样率
		fmt.setSampleSize(this->sampleSize);//采样大小
		fmt.setChannelCount(this->channel);//设置通道数
		fmt.setCodec("audio/pcm");//设置编解码器
		fmt.setByteOrder(QAudioFormat::LittleEndian);//设置字节序
		fmt.setSampleType(QAudioFormat::UnSignedInt);//设置采样类型
		output = new QAudioOutput(fmt);
		io = output->start();//启动音频播放
		mutex.unlock();
		return true;
	}
	void Play(bool isplay)
	{
		mutex.lock();
		if (!output)
		{
			mutex.unlock();
			return;
		}
		if (isplay)
		{
			output->resume();//恢复播放
		}
		else
		{
			output->suspend();//暂停
		}
		mutex.unlock();
	}

	virtual int GetFree()
	{
		mutex.lock();
		if (!output)
		{
			mutex.unlock();
			return 0;
		}
		int free = output->bytesFree();//返回音频缓冲区中剩余的可写字节数
		mutex.unlock();
		return free;
	}

	bool Write(const char *data, int datasize)
	{
		if (!data || datasize <= 0)
			return false;
		mutex.lock();
		//写入音频帧数据
		if (io)
			io->write(data, datasize);
		mutex.unlock();
		return true;
	}
};

XAudioPlay::XAudioPlay()
{
}


XAudioPlay::~XAudioPlay()
{
}

//使用纯虚父类API获取到静态CXAudioPlay，为了输入不同的编解码器类型的可扩容性。
XAudioPlay *XAudioPlay::Get()
{
	static CXAudioPlay ap;
	return &ap;
}