#ifndef CALLBACK_HPP
#define CALLBACK_HPP

#include "../app.hpp"

namespace dahua {
    namespace callback {
        
        typedef void(*onSignal)(int);
        typedef fHaveReConnect      onReconnect;
        typedef fDisConnect         onDisconnect;
        typedef fRealDataCallBackEx onDataRecieved;

        namespace client_default {
            extern const callback::onDisconnect print_info_on_disc;
            extern const callback::onReconnect  print_info_on_rec;
        }; /*client_default ns*/

        namespace process_default {
            extern const callback::onDataRecieved redirect_data;
        }; /*process_default ns*/

        namespace exporter_default {
//            extern const callback::onSignal local_exporter;
//            extern const callback::onSignal web_exporter;
        }; /*exporter_default ns*/

    };
};
#endif // CALLBACK_HPP
