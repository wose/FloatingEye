#ifndef FLOATING_EYE_VIDEOSTREAM_H
#define FLOATING_EYE_VIDEOSTREAM_H

#include <vlc/vlc.h>

#include <memory>
#include <mutex>
#include <string>

namespace irr { namespace video {
class IVideoDriver;
class IImage;
}
}

namespace IrrIMGUI {
class IGUITexture;
class IIMGUIHandle;
}

class VideoStream
{
public:
    VideoStream(const std::string& uri, irr::video::IVideoDriver* const videoDriver, IrrIMGUI::IIMGUIHandle* const gui);
    virtual ~VideoStream();

    void draw();

    static void* preRenderCallback(void *data, void **p_pixels);
    static void postRenderCallback(void *data, void *id, void *const *p_pixels);
    static void displayCallback(void *data, void *id);

    irr::video::IImage* videoTexture_;
    IrrIMGUI::IGUITexture* guiVideoTexture_;
    IrrIMGUI::IIMGUIHandle* gui_;

private:
    libvlc_instance_t* vlcInstance_;
    libvlc_media_player_t* vlcMediaPlayer_;

    bool updateTexture_ = false;
    std::string uri_;
    std::mutex textureMutex_;
};

#endif // FLOATING_EYE_VIDEOSTREAM_H
