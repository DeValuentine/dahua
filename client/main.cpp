//#include "gui/MainWindow/mainwindow.hpp"
//#include <QApplication>

#include "engine/app.hpp"
#include "engine/client/client.hpp"
#include "engine/h264/data/data.hpp"
#include "engine/algorithm/export/local/localstream.hpp"
#include "engine/algorithm/control/ptzcontrol.hpp"
#include "engine/algorithm/play/play.hpp"

//class AutoClient {
//public:
//    AutoClient() {
//        m_client = value::Ptr<dahua::Client>(dahua::Client::Instance());
//        m_client->init();
//        m_client->startVideoStream();
//    }
//    virtual ~AutoClient() {
//        m_client->stopVideoStream();
//        m_client->shutdown();
//        m_client.reset();
//    }
//    dahua::Client* get() {
//        return m_client.get();
//    }
//    dahua::Client* const operator->() {
//        return m_client.get();
//    }
//    dahua::Client const& operator*() {
//        return *m_client;
//    }
//private:
//    value::Ptr<dahua::Client> m_client;
//};



int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//
//    return a.exec();    
    try {
        // INIT SERVICES //
        auto client = dahua::Client::Instance();
        auto export_center = dahua::algorithm::process::exporter::ExportCenter::Instance();
        auto queue = dahua::algorithm::h264::Queue::Instance();
        auto ptz = dahua::algorithm::process::PTZControl::Instance();
        auto player = dahua::algorithm::process::Play::Instance();
        // ! //
        
        client->init();
        system("rm temporary.h264");
        export_center->startLocalExporter();
        
        player->start();
        
        ptz->start();
        ptz->control();
        
        client->shutdown();
    
        // CLEAN RESOURCES //
        if (player->isActive())
            player->stop();
        player->free();
        ptz->free();
        export_center->free();
        queue->free();
        client->free();
        // ! //
    } catch (std::exception & ex) {
        printf( "Exception[ %s ]\n", ex.what() );
        return -1;
    }
    return 0;
}
