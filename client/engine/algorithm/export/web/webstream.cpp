#include "webstream.hpp"

//dahua::algorithm::process::exporter::WebStream* dahua::algorithm::process::exporter::WebStream::m_instance = nullptr;
//
//dahua::callback::onSignal const dahua::callback::exporter_default::web_exporter     =     [](int sig) -> void {
//    auto data_queue = dahua::algorithm::h264::Queue::Instance();
//    dahua::algorithm::process::exporter::WebStream::Instance()->write(data_queue->front());
//    DEFAULT_EXCEPTION_CATCHER( data_queue->try_pop() );
//};
//
//void dahua::algorithm::process::exporter::WebStream::start() {
//    activate();
//}
//void dahua::algorithm::process::exporter::WebStream::update() {
//    
//}
//inline void dahua::algorithm::process::exporter::WebStream::activate() {
//    signal(SIG_EXPORT_DATA_TO_WEB_STREAM, m_exporter);  
//} 
//void dahua::algorithm::process::exporter::WebStream::write(const h264::Raw& _Raw) {
//    return;
//}
