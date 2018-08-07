#include "data.hpp"
extern "C" {
#include <libavutil/mem.h>
}
#include <future>
//const int dahua::algorithm::h264::Data::SIG_DATA_RECIEVED = 9424;

//static const dahua::algorithm::h264::Raw NULLRAW = {nullptr, 0, nullptr};

dahua::algorithm::h264::Queue* dahua::algorithm::h264::Queue::m_instance = nullptr;

dahua::algorithm::h264::Queue* dahua::algorithm::h264::Queue::Instance() {
    if (m_instance == nullptr)
        m_instance = new Queue();
    return m_instance;
}

dahua::algorithm::h264::IDataObserver::IDataObserver() { 
    printf("--[Ctor] Creating 'IDataObserver' --\n"); 
    dahua::algorithm::h264::Queue::Instance()->attach_observer(this); 
}

void dahua::algorithm::h264::Queue::attach_observer(IDataObserver* _obs) {
    std::cout << "[Queue] Attaching Observer...\n";
    m_mutex.lock();
    m_observers.push_back(_obs);
    m_mutex.unlock();
}

//inline void dahua::algorithm::h264::Queue::notify_observers() {
//#ifdef PACKAGE_WRITER_FULL_INFO
//    printf("[Queue] Notifying Observers...\n");
//#endif
//#ifndef VALUE_EXPORTER_CSIGNAL_VERSION
//    
//    for (IDataObserver* observer : m_observers) {
////        observer->update(this);
//        std::thread(&IDataObserver::update, observer, this).detach();
//    }
//#else
//    std::cout << "Raising exporter signals...\n";
//    for (int sig_exporter_i = SIG_EXPORT_DATA_START; sig_exporter_i < SIG_EXPORT_DATA_END; sig_exporter_i++) {
//        raise(sig_exporter_i);
//        printf("raised[%d]", sig_exporter_i);
//    }
//#endif
//}

void dahua::algorithm::h264::Queue::push(BYTE* _buf, DWORD _buf_sz, tagVideoFrameParam* _meta) {
#ifdef PACKAGE_WRITER_FULL_INFO
    printf("[Queue] Recieved new _Package[%p]...\n", _buf);
#endif
    m_mutex.lock();
    for (IDataObserver* observer : m_observers) {
//        observer->update(this);
       BYTE* bytes = (BYTE*)av_memdup(_buf, _buf_sz);
       Raw _temp;
       _temp.data = bytes; 
       _temp.size = _buf_sz; 
       _temp.meta = _meta;
       std::thread(&IDataObserver::update, observer, std::ref(_temp)).detach();
    }
    m_mutex.unlock();
}

//dahua::algorithm::h264::Raw const& dahua::algorithm::h264::Queue::front() const noexcept {
//    std::lock_guard<std::mutex> lock(m_mutex);
//#ifdef PACKAGE_WRITER_FULL_INFO
//    printf("[Queue] Retrieved front _Package...\n");
//#endif
//    return m_queue.front();
//}
//
//void dahua::algorithm::h264::Queue::try_pop() {
//    return;
//    std::lock_guard<std::mutex> lock(m_mutex);
//#ifndef VALUE_EXPORTER_CSIGNAL_VERSION
//    if (++m_updated_count < m_observers.size())
//        return;
//    else if (m_updated_count == m_observers.size()) {
//        av_free((void*)m_queue.front().data);
//        m_queue.pop();
//        m_updated_count = 0;
//    }
//    else
//        throw Exception(Exception::DATA_QUEUE_INVALID_UPDATED_COUNT_ERROR);
//#else
//    if (++m_updated_count < SIG_EXPORT_DATA_AMOUNT_OF_EXPORTERS)
//        return;
//    else if (m_updated_count == SIG_EXPORT_DATA_AMOUNT_OF_EXPORTERS) {
//        m_queue.pop();
//        m_updated_count = 0;
//    }
//    else
//        throw Exception(Exception::DATA_QUEUE_INVALID_UPDATED_COUNT_ERROR);
//#endif
//}

void dahua::algorithm::h264::Queue::free() {
    m_observers.clear();
    if (m_instance != nullptr)
    delete m_instance;
}