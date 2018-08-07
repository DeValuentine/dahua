#ifndef LOCALSTREAM_HPP
#define LOCALSTREAM_HPP


#include "../../../app.hpp"
#include "../iexportstream.hpp"
#include "../../../callback/callback.hpp"
#include "../../../h264/data/data.hpp"
#include "../../iprocess.hpp"

namespace dahua {
    
    namespace algorithm {
        namespace process {
            namespace exporter {
                    
                
                class LocalStream : public IProcess, public IExportStream, public h264::IDataObserver
                {
                public:
                    LocalStream() { 
                        printf("--[Ctor] Creating LocalStream exporter --\n"); 
                    }
                    ~LocalStream() override { 
                        printf("[LocalStream] Free memory at [%p]\n", this); 
                    }
                    
                    void start() override final;
                    void update(dahua::algorithm::h264::Raw&) override final;
                    void activate() override final; // signal(SIG_EXPORT_DATA_TO_LOCAL_STREAM, m_exporter);
                    inline void write(h264::Raw&) override final;
//                    void dump(const h264::data&, struct tm);
//                    Vobj__SINGLETON__body(LocalStream);
                private:
                    const int m_id = 1;
//                    callback::onSignal m_exporter;
                    struct tm m_start;
                    bool m_inited = false;
                };
                

//                class LocalStream : public IExportStream
//                {
//                public:
//                    LocalStream() = default;
//                    ~LocalStream() override = default;

//                    static LocalStream* Instance() {
//                        if (m_instance == nullptr)
//                            m_instance = new LocalStream();
//                        return m_instance;
//                    }

//                    void start() override {
//                        if (m_working)
//                            return;
//                        algorithm::process::exporter::Center::push(this);
//                        m_working = true;
//                        m_worker = std::thread(&LocalStream::m_algorithm, this);
//                        m_worker.detach();
//                    }
//                    bool compare_id(int _id) const override {
//                        return m_id == _id;
//                    }
//                    void update() override {
//                        std::unique_lock lock(m_mutex);
//                        m_has_update = true;
//                        m_update.notify_all();
//                    }
//                    bool is_working() {
//                        std::lock_guard lock(m_mutex);
//                        return m_working;
//                    }
//                    friend void operator<<(LocalStream* _stream, const h264::data& _data) {
//                        // EXPORT h264 BITSTREAM IN FILE HERE PLS DONT FORGET
//                        NET_TIME* _time = &_data.meta->struTime;
//                        char date[256];
//                        snprintf(date, sizeof(date), "%u-%u-%u %u:%u:%u", _time->dwYear,
//                                                                          _time->dwMonth,
//                                                                          _time->dwDay,
//                                                                          _time->dwHour,
//                                                                          _time->dwMinute,
//                                                                          _time->dwSecond);
//                    }
//                    void job_is_done_wait_update() {
//                        std::unique_lock lock(m_mutex);
//                        m_has_update = false;
//                        while (!m_has_update)
//                            m_update.wait(lock);
//                    }
//                private:
//                    static LocalStream* m_instance;
//                    int                 m_id;
//                    bool                m_working = false;
//                    bool                m_has_update = false;
//                    std::thread         m_worker;
//                    std::mutex          m_mutex;
//                    std::condition_variable m_update;
//                    ExportAlgorithm     m_algorithm = []()->void {
//                        process::exporter::LocalStream* local_stream = process::exporter::LocalStream::Instance();
//                        h264::DataQueue* data_queue = h264::DataQueue::Instance();
//                        while (local_stream->is_working()) {
//                            auto _data = data_queue->front();
//                            local_stream << _data;
//                            local_stream->job_is_done_wait_update();
//                        }
//                    };
//                };


                


            } /* exporter ns */
        } /* process ns */
    } /* algorithm ns */
} /* dahua ns */


#endif // LOCALSTREAM_HPP
