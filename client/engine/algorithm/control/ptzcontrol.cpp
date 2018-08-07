#include <opencv2/highgui.hpp>

#include "ptzcontrol.hpp"
#include "../play/play.hpp"

Vobj__SINGLETON__def(dahua::algorithm::process::PTZControl);

#define __CONTROLFUNCTION__( CMD ) []()->void {  \
auto client = dahua::Client::Instance();\
CLIENT_DHPTZControlEx(client->getLoginHandle(), client->getChannel(), CMD, 0,0,0, TRUE); \
}

void dahua::algorithm::process::PTZControl::start() {
    if (m_inited)
        return;
    char buffer[1024 * 8] = "";
    int nerr = 0;
    auto client = dahua::Client::Instance();
    if ( !CLIENT_QueryNewSystemInfo(client->getLoginHandle(), CFG_CAP_CMD_PTZ, client->getChannel(), buffer, (DWORD)sizeof(buffer), &nerr) )
    {
        char _msg[256];
        snprintf(_msg, sizeof(_msg), "CLIENT_QueryNewSystemInfo failed: 0x%x", CLIENT_GetLastError());
        throw Exception(_msg);
    }
    CFG_PTZ_PROTOCOL_CAPS_INFO ptz_caps_info = {sizeof(CFG_PTZ_PROTOCOL_CAPS_INFO)};
    if ( !CLIENT_ParseData(CFG_CAP_CMD_PTZ, buffer, &ptz_caps_info, sizeof(ptz_caps_info), nullptr) )
    {
        char _msg[256];
        snprintf(_msg, sizeof(_msg), "CLIENT_ParseData failed: %x", CLIENT_GetLastError());
        throw Exception(_msg);
    }
    typedef std::pair<std::string, Controller> ControlPair;
    if (ptz_caps_info.bTile) {
        m_control_map.insert(ControlPair("MOVE_UP",        __CONTROLFUNCTION__(DH_PTZ_UP_CONTROL)));
        m_control_map.insert(ControlPair("MOVE_DOWN",      __CONTROLFUNCTION__(DH_PTZ_DOWN_CONTROL)));
    }
    if (ptz_caps_info.bPan) {
        m_control_map.insert(ControlPair("MOVE_LEFT",      __CONTROLFUNCTION__(DH_PTZ_LEFT_CONTROL)));
        m_control_map.insert(ControlPair("MOVE_RIGHT",     __CONTROLFUNCTION__(DH_PTZ_RIGHT_CONTROL)));
    }
    if (ptz_caps_info.bTile && ptz_caps_info.bPan) {
        m_control_map.insert(ControlPair("MOVE_LEFT+UP",   __CONTROLFUNCTION__(DH_EXTPTZ_LEFTTOP)));
        m_control_map.insert(ControlPair("MOVE_LEFT+DOWN", __CONTROLFUNCTION__(DH_EXTPTZ_LEFTDOWN)));
        m_control_map.insert(ControlPair("MOVE_RIGHT+UP",  __CONTROLFUNCTION__(DH_EXTPTZ_RIGHTTOP)));
        m_control_map.insert(ControlPair("MOVE_RIGHT+DOWN", __CONTROLFUNCTION__(DH_EXTPTZ_RIGHTDOWN)));
    }
    if (ptz_caps_info.bZoom) {
        m_control_map.insert(ControlPair("ZOOM+",          __CONTROLFUNCTION__(DH_PTZ_ZOOM_ADD_CONTROL)));
        m_control_map.insert(ControlPair("ZOOM-",          __CONTROLFUNCTION__(DH_PTZ_ZOOM_DEC_CONTROL)));
    }
    if (ptz_caps_info.bFocus) {
        m_control_map.insert(ControlPair("FOCUS+",         __CONTROLFUNCTION__(DH_PTZ_FOCUS_ADD_CONTROL)));
        m_control_map.insert(ControlPair("FOCUS-",         __CONTROLFUNCTION__(DH_PTZ_FOCUS_DEC_CONTROL)));
    }
    if (ptz_caps_info.bIris) {
        m_control_map.insert(ControlPair("APERTURE+",      __CONTROLFUNCTION__(DH_PTZ_APERTURE_ADD_CONTROL)));
        m_control_map.insert(ControlPair("APERTURE-",      __CONTROLFUNCTION__(DH_PTZ_APERTURE_DEC_CONTROL)));
    }
    m_inited = true;
    vLOG("[PTZControl] Ready...\n");
}

//inline char getCmd(char& _Handle) {
//    char _tmp;
//    std::cin >> _tmp;
//    _Handle = (char)tolower(_tmp);
//    return _Handle;
//}

void dahua::algorithm::process::PTZControl::control() {
    char _U_Input = '\0';
    char tmp;
    auto player = dahua::algorithm::process::Play::Instance();
    printf("\n[Dahua PTZControl Menu]\n--(w.a.s.d): to Move camera\n--(r.f): to Zoom\n--(t.g): to Focus\n--(y.h): to change Aperture\n--(q): to exit\n[/Usage Guide]\n\n");
    do {
        printf("Cmd >");
        std::cin >> tmp;
        _U_Input = tolower(tmp);
        switch (_U_Input) {
//            case 'p':
//                player->isActive() ? player->stop() : player->start(); 
//                vLOG("UsedAction > 'Toggled player'\n"); 
//                break;
            case 'w': 
                 execute("MOVE_UP"); 
                 vLOG("UsedAction > 'Moved up'\n"); break;
            case 'a':
                 execute("MOVE_LEFT"); 
                 vLOG("UsedAction > 'Moved left'\n"); break;
            case 's':
                 execute("MOVE_DOWN"); 
                 vLOG("UsedAction > 'Moved down'\n"); break;
            case 'd':
                 execute("MOVE_RIGHT"); 
                 vLOG("UsedAction > 'Moved right'\n"); break;
            case 'r':
                 execute("ZOOM+"); 
                 vLOG("UsedAction > 'Zoomed+'\n"); break;
            case 'f':
                 execute("ZOOM-"); 
                 vLOG("UsedAction > 'Zoomed-'\n"); break;
            case 't':
                 execute("FOCUS+"); 
                 vLOG("UsedAction > 'Focused+'\n"); break;
            case 'g':
                 execute("FOCUS-"); 
                 vLOG("UsedAction > 'Focused-'\n"); break;
            case 'y':
                 execute("APERTURE+"); 
                 vLOG("UsedAction > 'Apertured+'\n"); break;
            case 'h':
                 execute("APERTURE-"); 
                 vLOG("UsedAction > 'Apertured-'\n"); break;
            default:
                if (_U_Input != 'q')
                    vLOG("err > Unknown 'ActionCode'\n");
        }
    } while (_U_Input != 'q');
}