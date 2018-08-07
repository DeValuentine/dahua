#ifndef DAHUA_CLIENT_HPP
#define DAHUA_CLIENT_HPP

#include "../app.hpp"
#include "../exception/exception.hpp"
#include "../h264/data/data.hpp"
#include "../callback/callback.hpp"

namespace dahua {


#define DAHUA_CLIENT_CONFIG_JSON_PATH "settings/client_cfg.json"
#define DEFAULT_PLAY_TYPE DH_RType_Realplay
#define DEFAULT_DATA_TYPE 0x00000001
    
    struct net_info {
        std::string ip = "\0";
        WORD        port = 0;
    };
    struct user_info {
        std::string login = "admin";
        std::string password = "admin";
    };
    struct connection_info {
        enum em_protocol_t {  TCP = 0,
                ActiveReg = 2,
                Multicast,
                UDP,
                MainConnection = 6,
                SSL,
                Remote = 9,
                LDAP = 12,
                AD,
                Radius,
                Socks5,
                Proxy,
                WebPP = 19,
                Mobile 
        } protocol = TCP;
        std::string option = "\0";
    };
    struct client_info {
        LLONG   login_handle = 0;
        LLONG   stream_handle = 0;
        int     channel = 1;
        net_info net;
        user_info user;
        connection_info connection;
    };
    
    
    class Client
    {
    public:
        Client() { printf("--[Ctor] Creating Client --\n"); }
        virtual ~Client() { printf("[Client] Free memory at [%p]\n", this); }
        static Client* Instance();
        void init();
        void shutdown();
        
        void startVideoStream();
        void stopVideoStream();
        
        void parseSettingsJSON();
        void setCustomCallbacks(callback::onDisconnect, callback::onReconnect);
        
        void free() {
            if (m_instance != nullptr)
            delete m_instance;
        }
    private:
        void setClientInfo(const json&);
        void setConnectTime(const json&);
        
    public:
        user_info const& 
            getUserInfo() const { return m_info.user; }
        net_info const& 
            getNetInfo() const { return m_info.net; }
        connection_info const&
            getConnectionInfo() const { return m_info.connection; }
        int getChannel() const { return m_info.channel; }
        LLONG getStreamHandle() const { return m_info.stream_handle; }
        LLONG getLoginHandle() const { return m_info.login_handle; }
        BOOL isInited()    const { return m_is_inited; }
        BOOL isStreaming() const { return m_is_streaming; }
    private:
        static Client* m_instance;
        BOOL    m_is_inited = FALSE;
        BOOL    m_is_streaming = FALSE;
        callback::onDisconnect  m_disconnect_cb = callback::client_default::print_info_on_disc;
        callback::onReconnect   m_reconnect_cb = callback::client_default::print_info_on_rec;
        client_info m_info;        
    };
    
    
} // end of namespace dahua

#endif // DAHUA_CLIENT_HPP
