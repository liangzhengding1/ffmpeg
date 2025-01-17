#ifndef __IAVI_PLAYER_H_INCLUDED__
#define __IAVI_PLAYER_H_INCLUDED__
#include <ITexture.h>
#include <IVideoDriver.h>
#include <audiere.h> // AckisExtensions
#include <windows.h>
#include <vfw.h>

namespace irr{
namespace scene{
class IAVIPlayer{
public:
IAVIPlayer(){}
virtual void DrawMovie() = 0;
virtual video::ITexture *getVideoTexture() = 0;
virtual void SetMovieFPS(int fps) = 0;
virtual void SetLooped(bool fl) = 0;
virtual u32  getCurrentFrame() = 0;
virtual void setCurrentFrame(s32 frame) = 0;
virtual void Stop() = 0;
virtual core::dimension2d<u16> getMovieDimension() = 0;
virtual void Run() = 0;
virtual bool IsRunning() = 0;
virtual bool OpenAVIStream(const char* filename, video::IVideoDriver* irrVideo, bool playAudio = false) = 0;
virtual bool CloseAVIStream() = 0;
virtual void drop() = 0;
};}}
#endif	// __IAVI_PLAYER_H_INCLUDED__
