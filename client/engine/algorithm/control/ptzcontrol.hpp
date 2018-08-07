#ifndef PTZCONTROL_HPP
#define PTZCONTROL_HPP

#include "../../app.hpp"
#include "../iprocess.hpp"
#include "../../exception/exception.hpp"
#include "../../client/client.hpp"
#include <unordered_map>

namespace dahua {
    namespace algorithm {
        namespace process {

            class PTZControl : public IProcess
            {
                Vobj__SINGLETON__body(PTZControl);
            public:
                typedef void(*Controller)(void); 
                typedef std::unordered_map<std::string, Controller> ControlMapType;
                
                PTZControl() { printf("--[Ctor] Creating PTZControl --\n"); }
                virtual ~PTZControl() { printf("[PTZControl] Free memory at [%p]\n", this); }
                
                void start() override final;         
                void control();
                
                ControlMapType const& getMap() const { return m_control_map; } 
                Controller const& getController(std::string _Cmd) const { return m_control_map.at(_Cmd); }
                void execute(std::string _Cmd) { m_control_map.at(_Cmd)(); }
            private:
                ControlMapType m_control_map;
                bool m_inited = false;
            };


        } /* process ns */
    } /* algorithm ns */
} /* dahua ns */


#endif // PTZCONTROL_HPP
