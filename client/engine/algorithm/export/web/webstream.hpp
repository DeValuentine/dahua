#define WEBSTREAM_HPP

#ifndef WEBSTREAM_HPP
#define WEBSTREAM_HPP

#include "../../../app.hpp"
#include "../iexportstream.hpp"
#include "../../../callback/callback.hpp"
#include "../../../h264/data/data.hpp"
#include "../../iprocess.hpp"

namespace dahua {
    namespace algorithm {
        namespace process {
            namespace exporter {

                class WebStream : public IProcess, public IExportStream, public h264::IDataObserver
                {
                public:
                    WebStream() : m_exporter(dahua::callback::exporter_default::web_exporter) { printf("Creating WebStream...\n"); }
                    ~WebStream() override = default;

                    static WebStream* Instance() {
                        if (m_instance == nullptr)
                            m_instance = new WebStream();
                        return m_instance;
                    }
                    void start() override final;
                    void activate() override final;
                    void update() override final;
                    void write(const h264::Raw&) override final;
                    void free() {
                        if (m_instance != nullptr)
                        delete m_instance;
                    }
                private:
                    static WebStream* m_instance;
                    const int m_id = 2;
                    callback::onSignal m_exporter;
                };


            } /* exporter ns */
        } /* process ns */
    } /* algorithm ns */
} /* dahua ns */

#endif // WEBSTREAM_HPP
