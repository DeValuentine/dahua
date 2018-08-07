#ifndef IPROCESS_HPP
#define IPROCESS_HPP

#include "../app.hpp"

namespace dahua {
    namespace algorithm {
        namespace process {

            class IProcess
            {
            public:
                IProcess() { printf("--[Ctor] Creating IProcess --\n"); }
                virtual ~IProcess() { printf("[IProcess] Free memory at [%p]\n", this); }
                virtual void start() = 0;
            };
//                virtual int compare_id(int) const = 0;

//                static IProcess* fetchByID(int _id) {
//                    for ( IProcess* process : m_processes ) {
//                        if (process->compare_id(_id))
//                            return process;
//                    }
//                    return nullptr;
//                }
//                static auto get() const -> decltype(m_processes) { return m_processes; }
//                static void push(IProcess* _proc) { m_processes.push_back(_proc); }
//            private:
//                static std::vector<IProcess*> m_processes;
            



        } /* process ns */
    } /* algorithm ns */
} /* dahua ns */




#endif // IPROCESS_HPP
