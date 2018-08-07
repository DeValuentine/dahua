#ifndef DATA_HPP
#define DATA_HPP


#include "../../app.hpp"
#include "../../exception/exception.hpp"
#include <thread>
#include <mutex>

// DEPRECATED AFTER v3.0 //
#ifdef VALUE_EXPORTER_CSIGNAL_VERSION        
#define SIG_EXPORT_DATA_0               942400
#define SIG_EXPORT_DATA_(n)             (SIG_EXPORT_DATA_0 + n)
            
#define SIG_EXPORT_DATA_TO_LOCAL_STREAM SIG_EXPORT_DATA_(1)
#define SIG_EXPORT_DATA_TO_WEB_STREAM   SIG_EXPORT_DATA_(2)
            
#define SIG_EXPORT_DATA_L               SIG_EXPORT_DATA_(1)
#define SIG_EXPORT_DATA_AMOUNT_OF_EXPORTERS (SIG_EXPORT_DATA_L - SIG_EXPORT_DATA_0)

#define SIG_EXPORT_DATA_START           SIG_EXPORT_DATA_(1)
#define SIG_EXPORT_DATA_END             (SIG_EXPORT_DATA_START + SIG_EXPORT_DATA_AMOUNT_OF_EXPORTERS)
#endif

namespace dahua {
    namespace algorithm {
        namespace h264 {
            

            struct Raw {
                BYTE* data;
                DWORD size;
                tagVideoFrameParam* meta;
            };
            
            
            class IDataObserver {
            public:
                IDataObserver(); 
                virtual ~IDataObserver() { printf("[IDataObserver] Free memory at [%p]\n", this); }
                virtual void update(Raw&) = 0;
            };
            
            // Thread-safe
            class Queue {
            public:
                Queue() { std::cout << "--[Ctor] Creating 'Queue' --\n"; }
                virtual ~Queue() { printf("[Queue] Free memory at [%p]\n", this); }

                static Queue* Instance();
                void attach_observer(IDataObserver* _obs);
//                inline void notify_observers();
                void push(BYTE* _buf, DWORD _buf_sz, tagVideoFrameParam* _meta);
//                const Raw& front() const noexcept;
//                void try_pop();
                void free();
            private:
                static Queue* m_instance;
                int m_updated_count = 0;
                Raw m_temp;
                std::vector<IDataObserver*> m_observers;
                mutable std::mutex m_mutex;
            };


            namespace constants {
                enum dahua_data_t {
                    AV_ORIG_DATA = 0,
                    V_MPEG4_H264_DATA,
                    V_YUV_DATA,
                    A_PCM_DATA,
                    A_ORIG_DATA,
                };
                enum resolution_ratio_t {
                    CIF = 0,
                    HD1,
                    CIF2,
                    D1,
                    VGA,
                    QCIF,
                    QVGA,
                    SVCD,
                    QQVGA,
                    SVGA,
                    XVGA,
                    WXGA,
                    SXGA,
                    WSXGA,
                    UXGA,
                    WUXGA,
                    LFT,
                    HD720,
                    FHD1080
                };
                enum frame_t {
                    I = 0,
                    P,
                    B
                };
                enum encode_t {
                    MPEG4 = 1,
                    H264_Dahua,
                    H264_ADI,
                    H264_Standart
                };
                enum format_t {
                    PAL = 0,
                    NTSC
                };
            }
            
            
        } /* h264 */
    } /* algorithm */
} /* dahua */


#endif // DATA_HPP
