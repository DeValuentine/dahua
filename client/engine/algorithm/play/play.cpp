#include "play.hpp"
#include "decoder/Decoder.hpp"

Vobj__SINGLETON__def(dahua::algorithm::process::Play);

void dahua::algorithm::process::Play::play() {     
    vLOG("\n[PlayerThread] --Started--\n");
    
    std::string window_name = "DahuaCamera";
    cv::Mat frame;
    
    cv::namedWindow(window_name);
    cv::waitKey(1);
    
    dahua::algorithm::process::playutils::Translator translator;
    translator.init();
    
    std::this_thread::sleep_for(std::chrono::seconds(120));
    while (42) {
        if (!isActive()) {
            vLOG("[PlayerThread] -> is not active. Stopping thread...\n")
            break;
        }
        while (translator.has_no_frames()) {
            translator.wait();
        }
        translator.setToCurrent(frame);
        if (frame.empty()) {
            vLOG("[PlayerThread] -> frame is empty\n");
            continue;
        }
        cv::imshow(window_name, frame);
        if (cv::waitKey(10) == 27) 
            break;
    }
    
    translator.deinit();
    vLOG("\n[PlayerThread] --Stopped--\n");
}

void dahua::algorithm::process::Play::start() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_is_active)
        return;
    m_is_active = true;
    std::thread(&Play::play, this).detach();
}

void dahua::algorithm::process::Play::stop() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_is_active)
        return;
    m_is_active = false;
    
}




