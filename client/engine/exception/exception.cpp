#include "exception.hpp"

namespace dahua {

    Exception::Exception(const char* _ErrMsg)        : m_info(_ErrMsg) {}
    Exception::Exception(const std::string& _ErrMsg) : m_info(_ErrMsg.c_str()) {}

    const char* const Exception::CLIENT_INIT_FAILED_ERROR = "in Client::Init(): Failed. Initialisation aborted.";
//    const char* const Exception::REDIRECT_DATA_UNHANDLED_PROCESS_ERROR = "in callback::onDataRecieved(): Process does not exist.";
//    const char* const Exception::SIGNAL_ALREADY_TAKEN_ERROR = "in IExportStream(): Signal is already being used. Terminating...";
    const char* const Exception::DATA_QUEUE_INVALID_UPDATED_COUNT_ERROR = "in DataQueue::try_pop(): Updated observers count is greater than actual number. Terminating...";
    const char* const Exception::EXPORT_DATA_WRITE_FAILED = "Write(): Could not open file. Terminating...";
}
