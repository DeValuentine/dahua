#ifndef IEXPORT_HPP
#define IEXPORT_HPP

#include "../../app.hpp"
#include "../../exception/exception.hpp"
#include "../../callback/callback.hpp"
#include "../../h264/data/data.hpp" 

namespace dahua {
    namespace algorithm {
        namespace process {
            namespace exporter {
                
                class ExportCenter {
                public:
                    ExportCenter() { printf("--[Ctor] Creating ExportCenter --\n"); }
                    virtual ~ExportCenter() { printf("[ExportCenter] Free memory at [%p]\n", this); }
                    static ExportCenter* Instance();
                    void deactivate_all();
                    void add(IExportStream*);
                    IExportStream* startLocalExporter();
                    IExportStream* getLocalExporter() const { return m_local_exporter; }
                    void free() {
                        deactivate_all();
                        if (m_instance != nullptr)
                            delete m_instance;
                    }
                protected:
                    static ExportCenter* m_instance;
                    IExportStream* m_local_exporter;
                    std::vector<IExportStream*> m_exporters;
                };
                
                class IExportStream {
                public:
                    IExportStream();
                    virtual ~IExportStream() { printf("[IExportStream] Free memory at [%p]\n", this); }
                    virtual void activate() = 0;
                    virtual void write(dahua::algorithm::h264::Raw&) = 0;
//                    virtual void free() = 0;
                };

                
    //                static IExportStream* fetchByID(int _id) const {
    //                    for (IExportStream* exporter : m_exporters) {
    //                        if (exporter->compare_id(_id))
    //                            return exporter;
    //                    }
    //                }
    //                static auto get() const -> decltype(m_exporters) { return m_exporters; }
    //                static void push(IProcess* _exper) { m_exporters.push_back(_exper); }
    //            private:
    //                static std::vector<IExportStream*> m_exporters;
                


            } /* exporter ns */
        } /* process ns */
    } /* algorithm ns */
} /* dahua ns */


#endif // IEXPORT_HPP
