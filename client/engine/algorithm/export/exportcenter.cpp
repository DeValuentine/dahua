#include "iexportstream.hpp"
#include "local/localstream.hpp"

dahua::algorithm::process::exporter::ExportCenter* dahua::algorithm::process::exporter::ExportCenter::m_instance = nullptr;

dahua::algorithm::process::exporter::ExportCenter* dahua::algorithm::process::exporter::ExportCenter::Instance() {
    if (m_instance == nullptr) {
        m_instance = new ExportCenter();
    }
    return m_instance;
}

void dahua::algorithm::process::exporter::ExportCenter::deactivate_all() {
    if (m_exporters.empty())
        return;
    for (IExportStream* exporter : m_exporters) {
        if (exporter != nullptr)
            delete exporter;
    }
    m_exporters.clear();
}
inline void dahua::algorithm::process::exporter::ExportCenter::add(dahua::algorithm::process::exporter::IExportStream* _Exporter) {
    std::cout << "[ExportCenter] Attaching exporter...\n";
    m_exporters.push_back(_Exporter);
}

dahua::algorithm::process::exporter::IExportStream* dahua::algorithm::process::exporter::ExportCenter::startLocalExporter() {
    dahua::algorithm::process::exporter::IExportStream* LocalExporter = new dahua::algorithm::process::exporter::LocalStream();
    LocalExporter->activate();
    m_local_exporter = LocalExporter;
    return LocalExporter;
}

dahua::algorithm::process::exporter::IExportStream::IExportStream() { 
    std::cout << "--[Ctor] Creating IExportStream --\n"; 
    ExportCenter::Instance()->add(this);  
}