#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <string>
#include <iostream>

namespace dahua {

    class Exception : public std::exception
    {
    public: /* C&Dtors */
        Exception(const char*);
        Exception(const std::string&);
        virtual ~Exception() = default;
    public: /* Methods */
        virtual std::string info() const { return std::string(m_info); }
        virtual const char* what() const _GLIBCXX_TXN_SAFE_DYN noexcept override { return m_info; }
    public: /* Defaults */
        static const char* const CLIENT_INIT_FAILED_ERROR;
        static const char* const DATA_QUEUE_INVALID_UPDATED_COUNT_ERROR;
        static const char* const EXPORT_DATA_WRITE_FAILED;
    private: /* Fields */
        const char* m_info;
    };


#define DEFAULT_EXCEPTION_CATCHER( function_call ) try { function_call; } catch(Exception& ex) { std::cout << ex.what(); }



} /*dahua ns*/

#endif // EXCEPTION_HPP
