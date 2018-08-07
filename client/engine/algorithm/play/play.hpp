#ifndef PLAY_HPP
#define PLAY_HPP

#include "../../app.hpp"
#include "../iprocess.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv/highgui.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

//#define _F_TEMPORARY "temporary.h264"

namespace dahua {
    namespace algorithm {
        namespace process {

            
            class Play : public IProcess 
            {
                Vobj__SINGLETON__body(Play);
            public: 
                typedef DH_RealPlayType type;
                
                Play() { printf("--[Ctor] Creating Player --"); }
                virtual ~Play() { printf("[Play] Free memory at [%p]\n", this); }
                
                void start() override final;
                void stop();
                void play();
                bool isActive() const { std::lock_guard<std::mutex> lock(m_mutex); return m_is_active; }
            private:
                bool m_is_active;
                mutable std::mutex m_mutex;
            };


        } /* process ns */
    } /* algorithm ns */
} /* dahua ns */



#endif // PLAY_HPP
