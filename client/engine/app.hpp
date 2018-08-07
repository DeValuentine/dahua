#ifndef APP_HPP
#define APP_HPP

/* SYSTEM */
#include <cstdlib>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
//#include <csignal>
#include <queue>
#include <vector>
#include <string>
#include <iostream>
/* DAHUA NETSDK */
#include "../3rdParty/NetSDK/include/avglobal.h"
#include "../3rdParty/NetSDK/include/dhnetsdk.h"
#include "../3rdParty/NetSDK/include/dhconfigsdk.h"
/* JSON */
#include "../3rdParty/json/json.hpp"
/* OPENCV */
//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv/cv.h>
//#include <opencv/highgui.h>

using json = nlohmann::json;

//namespace value {
//    template<typename T>
//    class Ptr {
//    public:
//        Ptr(T* _data);
//        Ptr(Ptr const& _ptr);
//        ~Ptr();
//        T* const operator->();
//        T const& operator*();
//        T* get();
//        void reset();
//        void reset(T* new_ptr);
//    private:
//        T* m_data;
//        static int m_ptr_count;
//    };
//
//    template<class T, typename... Args>
//    Ptr<T> make_ptr(Args... args) {
//        return Ptr<T>(new T(std::forward(args)...));
//    }
//} //value ns//

namespace dahua {
    class Client;
    class Exception;
    namespace algorithm {
        namespace process {
            class IProcess;
            class Play;
            namespace playutils {
//                class _FrameBuffer;
//                class _Translator;
            }
            class PTZControl;
            namespace exporter {
                class ExportCenter;
                class IExportStream;
                class LocalStream;
                class WebStream;
            };
        };
        namespace h264 {
            struct Raw;
            class Queue;
//            class IDataObserver;
        };
    };
    
};

#define Vobj__SINGLETON__body(objType) \
public: \
    static objType * Instance() { \
        if (m_instance == nullptr) \
            m_instance = new objType(); \
        return m_instance; \
    } \
    void free() { \
        if (m_instance != nullptr) \
            delete m_instance; \
    } \
private: \
    static objType * m_instance;

#define Vobj__SINGLETON__def(objType) \
    objType* objType::m_instance = nullptr;



/* include list 
// APPLICATION //
#include "exception/exception.hpp"
#include "callback/callback.hpp"
#include "h264/data/data.hpp"
#include "client/client.hpp"
// PROCESSES: //
#include "algorithm/iprocess.hpp"
#include "algorithm/play/play.hpp"
#include "algorithm/control/ptzcontrol.hpp"
// EXPORTERS: //
#include "algorithm/export/iexportstream.hpp"
#include "algorithm/export/local/localstream.hpp"
#include "algorithm/export/web/webstream.hpp"
*/

#define vLOG(msg, ...) fprintf(stderr, msg, ##__VA_ARGS__);
//#define VALUE_DEBUG
//#define PACKAGE_WRITER_INFO
//#define PACKAGE_WRITER_FULL_INFO
#endif // APP_HPP
