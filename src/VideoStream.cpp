#include "VideoStream.h"

#include "IrrIMGUI/IrrIMGUI.h"
#include "IrrIMGUI/IncludeIrrlicht.h"

#include <iostream>

VideoStream::VideoStream(const std::string& uri, irr::video::IVideoDriver* const videoDriver,
                         IrrIMGUI::IIMGUIHandle* const gui) :
    gui_{gui}
{
    videoTexture_ = videoDriver->createImage(irr::video::ECF_A8R8G8B8, irr::core::dimension2du(1280, 720));
    guiVideoTexture_ = gui_->createTexture(videoTexture_);

    char const *vlc_argv[] =
    {
        //        "-I", "dummy", // Don't use any interface
        //"--ignore-config", // Don't use VLC's config
        "--no-audio", /* skip any audio track */
        "--no-xlib", /* tell VLC to not use Xlib */
        "--network-caching=4000",
    };
    int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);

    vlcInstance_ = libvlc_new(vlc_argc, vlc_argv);
    auto vlcMedia = libvlc_media_new_location(vlcInstance_,
        "http://iphone-streaming.ustream.tv/uhls/17074538/streams/live/iphone/playlist.m3u8");
    vlcMediaPlayer_ = libvlc_media_player_new_from_media(vlcMedia);
    libvlc_media_release(vlcMedia);

    libvlc_video_set_callbacks(vlcMediaPlayer_,
                               VideoStream::preRenderCallback,
                               VideoStream::postRenderCallback,
                               VideoStream::displayCallback,
                               this);

    libvlc_video_set_format(vlcMediaPlayer_, "RV32", 1280, 720, videoTexture_->getPitch());
    libvlc_media_player_play(vlcMediaPlayer_);
}

VideoStream::~VideoStream()
{
    libvlc_media_player_stop(vlcMediaPlayer_);
    libvlc_media_player_release(vlcMediaPlayer_);
    libvlc_release(vlcInstance_);

    if(gui_) {
        gui_->deleteTexture(guiVideoTexture_);
    }
}

void VideoStream::draw()
{
    std::lock_guard<std::mutex> lock(textureMutex_);
    if(updateTexture_) {
        gui_->updateTexture(guiVideoTexture_, videoTexture_);
        updateTexture_ = false;
    }
    ImGui::Image(guiVideoTexture_, ImVec2(640, 360));
}

void* VideoStream::preRenderCallback(void* data, void** p_pixels)
{
    VideoStream* videoStream = reinterpret_cast<VideoStream*>(data);

    videoStream->textureMutex_.lock();
    *p_pixels = videoStream->videoTexture_->lock();

    return nullptr;
}

void VideoStream::postRenderCallback(void* data, void* id, void *const *p_pixels)
{
    VideoStream* videoStream = reinterpret_cast<VideoStream*>(data);
    videoStream->videoTexture_->unlock();
    videoStream->updateTexture_ = true;
    videoStream->textureMutex_.unlock();
}

void VideoStream::displayCallback(void *data, void *id)
{
}
