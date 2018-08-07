#include <fstream>

#include "client.hpp"

dahua::Client* dahua::Client::m_instance = nullptr;

dahua::callback::onDisconnect const dahua::callback::client_default::print_info_on_disc = [](LLONG  _id,
                                                                                            char*   _dvr_ip,
                                                                                            LONG    _dvr_port,
                                                                                            LDWORD  _user ) -> void {
    printf( "DISCONNECTED:\n" );
    printf( "L_ID[0x%x]", _id );
    printf( "USER[%d]\n", _user );
    if ( _dvr_ip != nullptr ) {
        printf( "DVR_IP[%s]\n", _dvr_ip );
    }
    printf( "DVR_PORT[%d]\n", _dvr_port );
    printf( "---\n" );
    return;
};

dahua::callback::onReconnect const dahua::callback::client_default::print_info_on_rec   = [](LLONG  _id,
                                                                                            char*   _dvr_ip,
                                                                                            LONG    _dvr_port,
                                                                                            LDWORD  _user ) -> void {
    printf( "RECONNECTED:\n" );
    printf( "L_ID[0x%x]", _id );
    printf( "USER[%d]\n", _user );
    if ( _dvr_ip != nullptr ) {
        printf( "DVR_IP[%s]\n", _dvr_ip );
    }
    printf( "DVR_PORT[%d]\n", _dvr_port );
    printf( "---\n" );
    return;
};

dahua::callback::onDataRecieved const dahua::callback::process_default::redirect_data =   [](LLONG  _process_handle,
                                                                                            DWORD   _data_type,
                                                                                            BYTE*   _buf,
                                                                                            DWORD   _buf_size,
                                                                                            LONG    _param,
                                                                                            LDWORD  _user ) -> void {
// audio&yum are useless atm
//    if (_data_type != dahua::algorithm::h264::constants::V_MPEG4_H264_DATA) {
//        return;
//    }
    tagVideoFrameParam* meta_data = (tagVideoFrameParam*)_param;
    dahua::algorithm::h264::Queue* data_queue = dahua::algorithm::h264::Queue::Instance();
    data_queue->push( _buf, _buf_size, meta_data );
};


dahua::Client* dahua::Client::Instance() {
    if (m_instance == nullptr) {
        m_instance = new Client();
    }
    return m_instance;
}

inline void dahua::Client::parseSettingsJSON() {
    printf("[Client] Loading settings from json...\n");
    std::ifstream fin(DAHUA_CLIENT_CONFIG_JSON_PATH);
    if (!fin.is_open())
        throw Exception("\n[ERROR] Client::loadSettings(): Could not open cfg file.\n");
    char buffer[1024 * 32];
    fin.read(buffer, sizeof(buffer));
    fin.close();
    json settings = json::parse(buffer);
#ifdef VALUE_DEBUG
    std::cout << buffer << std::endl;
    std::cout << settings << std::endl;
#endif
    setClientInfo(settings["ClientInfo"]);
    setConnectTime(settings["ConnectTime"]);
#ifdef VALUE_DEBUG
    std::cout << m_info.net.ip << std::endl << m_info.net.port << std::endl;
#endif
}

void dahua::Client::init() {
    parseSettingsJSON();
    printf("[Client] Initialising...\n");
    m_is_inited = CLIENT_Init(m_disconnect_cb, m_info.login_handle);
    if (!m_is_inited)
        throw dahua::Exception(dahua::Exception::CLIENT_INIT_FAILED_ERROR);
    NET_DEVICEINFO_Ex device_info = {0};
    int err_code = 0;
    printf("[Client] Trying to Login...\n");
    while (m_info.login_handle == 0) {
        m_info.login_handle = CLIENT_LoginEx2(m_info.net.ip.c_str(),
                                         m_info.net.port,
                                         m_info.user.login.c_str(),
                                         m_info.user.password.c_str(),
                                         (EM_LOGIN_SPAC_CAP_TYPE)m_info.connection.protocol,
                                         (void*) (m_info.connection.option.empty() ? nullptr : m_info.connection.option.c_str()),
                                         &device_info,
                                         &err_code);
        if (m_info.login_handle == 0)
            printf("[Client] @(%s:%d): Login Status[Failed].Error[%x : %x]\n",
               m_info.net.ip.c_str(),
               m_info.net.port, 
               CLIENT_GetLastError(), err_code
               );
        else 
            printf("[Client] @(%s:%d): Login Status[Success], LoginHandle[%d]\n", m_info.net.ip.c_str(), m_info.net.port, m_info.login_handle);
        sleep(1);
    }
    // YES!
    startVideoStream();
    printf("[Client] Setting RedirectData callback...\n");
    CLIENT_SetRealDataCallBackEx(m_info.stream_handle, callback::process_default::redirect_data, NULL, DEFAULT_DATA_TYPE);
}

void dahua::Client::shutdown() {
    if (m_instance != nullptr) {
        if (getLoginHandle()) {
            // NO, GOD!
            stopVideoStream();
            if ( CLIENT_Logout(getLoginHandle()) ) {
                printf("[Client] Logout sucess...\n");
                m_info.login_handle = 0;
            }
            else
                printf("[Client] Logout Status[Failed].Error[%x]\n", CLIENT_GetLastError());
        }
        if (m_is_inited) {
            CLIENT_Cleanup();
            m_is_inited = FALSE;
        }
    }
}

inline void dahua::Client::setClientInfo(const json& json_client_info) {
    m_info.login_handle = 0;
    m_info.stream_handle = 0;
    m_info.channel = json_client_info["ChannelID"].get<int>();
    m_info.net.ip = json_client_info["Network"]["ip"].get<std::string>();
    m_info.net.port = json_client_info["Network"]["port"].get<WORD>();
    m_info.user.login = json_client_info["User"]["login"].get<std::string>();
    m_info.user.password = json_client_info["User"]["password"].get<std::string>();
    m_info.connection.protocol = (connection_info::em_protocol_t)json_client_info["Connection"]["protocol"].get<int>();
    m_info.connection.option = json_client_info["Connection"]["option"].get<std::string>();
#ifdef VALUE_DEBUG
    std::cout << json_client_info["Network"]["ip"].get<std::string>() << std::endl << json_client_info["Network"]["port"].get<WORD>() << std::endl;
#endif
}
inline void dahua::Client::setConnectTime(const json& json_connect_time) {
    CLIENT_SetAutoReconnect(m_reconnect_cb, m_info.login_handle);
    CLIENT_SetConnectTime(json_connect_time["ms"].get<int>(),
                          json_connect_time["times"].get<int>());
    NET_PARAM net_param = {0};
    net_param.nConnectTime = json_connect_time["timeout"].get<int>();
    CLIENT_SetNetworkParam(&net_param);
}

void dahua::Client::startVideoStream() {
    if (m_is_streaming)
        return;
    printf("[Client] Starting video stream...\n");
    m_info.stream_handle = CLIENT_RealPlayEx(m_info.login_handle, m_info.channel, nullptr, DEFAULT_PLAY_TYPE);
    if (m_info.stream_handle != 0)
        m_is_streaming = TRUE;
}
void dahua::Client::stopVideoStream() {
    printf("[Client] Stopping video stream...\n");
    if (!m_is_streaming)
        return;
    CLIENT_StopRealPlayEx(m_info.stream_handle);
    m_is_streaming = FALSE;
    m_info.stream_handle = 0;
}


inline void dahua::Client::setCustomCallbacks(callback::onDisconnect _onDisc, 
                                              callback::onReconnect _onRec) {
    m_reconnect_cb = _onRec;
    m_disconnect_cb = _onDisc;
}
