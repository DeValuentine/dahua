#include "localstream.hpp"
#include <fstream>

//Vobj__SINGLETON__def(dahua::algorithm::process::exporter::LocalStream);

//dahua::callback::onSignal const dahua::callback::exporter_default::local_exporter   =     [](int sig) -> void {
//#ifdef PACKAGE_WRITER_FULL_INFO
//    printf("[LocalStream] Recieved Signal[%d]\n", sig);
//#endif
//    
//    dahua::algorithm::process::exporter::ExportCenter::Instance()->getLocalExporter()->write(data_queue->front());
//    DEFAULT_EXCEPTION_CATCHER( data_queue->try_pop() );
//};

void dahua::algorithm::process::exporter::LocalStream::start() {
    //activate();
}
void dahua::algorithm::process::exporter::LocalStream::update(dahua::algorithm::h264::Raw& _data) {
#ifdef PACKAGE_WRITER_FULL_INFO
    printf("[LocalStream] Updating...\n");
#endif
    write(_data);
    return;
}
void dahua::algorithm::process::exporter::LocalStream::activate() {
//    printf("[LocalStream] Listening SIGNAL[%d] now...\n\n", SIG_EXPORT_DATA_TO_LOCAL_STREAM);
//    signal(SIG_EXPORT_DATA_TO_LOCAL_STREAM, m_exporter);  
}

const char* MonthName[] = {
    "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
};

void dahua::algorithm::process::exporter::LocalStream::write(h264::Raw& _Raw) { 
    if (_Raw.data == nullptr) {
        fprintf(stderr, "Writer Failure[ 'bad_data' ]\n");
        return;
    }
    bool _has_Metadata = true;
    if (_Raw.meta == nullptr) {
#ifdef PACKAGE_WRITER_FULL_INFO
        fprintf(stderr, "--[!] package w/o metadata --\n");
#endif
        _has_Metadata = false;   
    }
#ifdef PACKAGE_WRITER_FULL_INFO
    printf("_Package[%p]: Writting data to file...\n", _Raw.data);
#endif
    char _f_out_dir[64];
    char _cmd_create_dir[96];
    char _f_out_path[128];
    
    struct tm _t_curr;
    
    if (_has_Metadata) {
        // PACKAGE W/ METADATA //
        snprintf(_f_out_dir, sizeof(_f_out_dir), "dahua/video_archive/%u_%s/day_%u/hour_%u", _Raw.meta->struTime.dwYear, 
                                                                                            MonthName[_Raw.meta->struTime.dwMonth], 
                                                                                            _Raw.meta->struTime.dwDay, 
                                                                                            _Raw.meta->struTime.dwHour);
        snprintf(_cmd_create_dir, sizeof(_cmd_create_dir), "mkdir -p %s", _f_out_dir);
        system(_cmd_create_dir);
        snprintf(_f_out_path, sizeof(_f_out_path), _Raw.meta->struTime.dwMinute < 10 ? "%s/video_min_0%u.h264" : "%s/video_min_%u.h264", _f_out_dir, _Raw.meta->struTime.dwMinute); 
    } 
    else {
        // PACKAGE W/O METADATA //
        time_t tmp_tm_null = time(NULL);
        localtime_r(&tmp_tm_null, &_t_curr);
#ifdef VALUE_DEBUG
        fprintf(stderr, "[DEBUG] ShowTime() --< %i-%i-%i %i:%i:%i >-- \n", 1900 + _t_curr.tm_year, 1 + _t_curr.tm_mon, _t_curr.tm_mday, _t_curr.tm_hour, _t_curr.tm_min, _t_curr.tm_sec);
#endif        
        snprintf(_f_out_dir, sizeof(_f_out_dir), "dahua/video_archive/%u_%s/day_%u/hour_%u", 1900 + _t_curr.tm_year, MonthName[_t_curr.tm_mon], _t_curr.tm_mday, _t_curr.tm_hour);
        snprintf(_cmd_create_dir, sizeof(_cmd_create_dir), "mkdir -p %s", _f_out_dir);
        system(_cmd_create_dir);
        snprintf(_f_out_path, sizeof(_f_out_path), _t_curr.tm_min < 10 ? "%s/video_min_0%u.h264" : "%s/video_min_%u.h264", _f_out_dir, _t_curr.tm_min);
    }
    
    std::ofstream fout(_f_out_path, std::ios::binary | std::ios::app);
    if (!fout.is_open())
        throw Exception(Exception::EXPORT_DATA_WRITE_FAILED);
    for (int iter = 0; iter < _Raw.size; iter++)
        fout << *(_Raw.data+iter);
    fout.close();   
#ifdef PACKAGE_WRITER_INFO
    fprintf(stdout, "_Package[%p]: '%s' Success!\n", _Raw.data, _f_out_path);
#endif
    
    av_free(_Raw.data);
    return;
}

//void dahua::algorithm::process::exporter::LocalStream::dump(const h264::data& _Raw, struct tm _Now) {
//    if (!m_inited) {
//        m_start = _Now;
//        m_inited = true;
//    }
//    bool to_rewrite = false;
//    if ((_Now.tm_hour*3600 + _Now.tm_min*60 + _Now.tm_sec) - (m_start.tm_hour*3600 + m_start.tm_min*60 + m_start.tm_sec) > 15) {
//        to_rewrite = true;
//        m_start = _Now;
//    } 
//    
//    std::ofstream fout(_F_TEMPORARY, to_rewrite ? (std::ios::binary | std::ios::trunc) : (std::ios::binary | std::ios::app));
//    if (!fout.is_open())
//        throw Exception(Exception::EXPORT_DATA_WRITE_FAILED);
//    for (int iter = 0; iter < _Raw.size; iter++)
//        fout << *(_Raw.buf+iter);
//    fout.close();  
//    
////    dahua::algorithm::process::Play::Instance()->start();
//}