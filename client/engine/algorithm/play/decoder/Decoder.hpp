/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Decoder.hpp
 * Author: valentine
 *
 * Created on July 19, 2018, 11:04 AM
 */

#ifndef DECODER_HPP
#define DECODER_HPP

#include "../../../app.hpp"
#include "../../../exception/exception.hpp"
#include "../../../h264/data/data.hpp"
#include <opencv2/opencv.hpp>
extern "C" 
{
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
#include <libavutil/pixfmt.h>
#include <libswscale/swscale.h>
#include <libavcodec/ac3_parser.h>
#include <libavcodec/adts_parser.h>
#include <libavcodec/vorbis_parser.h>
}
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
//#include <functional>

namespace dahua {
    namespace algorithm {
        namespace process {
            namespace playutils {
                
//============================================================================//
//========================== COMMON TEMPLATE UTILS ===========================//
//============================================================================//
                
//                >
//                
                
//template   < class _WrappedType >
//class _UniqueContainer {
//public:
//    typedef _WrappedType    wrapped_type; 
//    typedef _WrappedType*   pointer; 
//    typedef pointer(*alloc_type)(void);
//    typedef void(*dealloc_type)(pointer*);
//public:
////    _UniqueContainer() : m_data(nullptr) {}
//    _UniqueContainer(alloc_type _A, dealloc_type _D) : m_data(nullptr), m_alloc_data(_A), m_free_data(_D) {}
//    _UniqueContainer(const _UniqueContainer& orig) = delete;
//    ~_UniqueContainer() { reset(); }
//    
//    pointer data() { 
//        if (m_data == nullptr) 
//            m_data = m_alloc_data();
//        return m_data; 
//    }
//    void reset(pointer _Val = nullptr) {
//        if (m_data != nullptr)
//            m_free_data(&m_data);
//        m_data = _Val;
//    }
//    pointer operator*() {
//        return data();
//    }
//private:
//    pointer m_data;
//    alloc_type m_alloc_data;
//    dealloc_type m_free_data;
//};
//
///*
// * Thread-safe Cycled Buffer Array class *
// */
//template < template < class > class _ContainerType, class _WrappedType, size_t _Size >
//class _CycledBufferArray {
//    static_assert(_Size > 0, "Check size > 0");
//public:
//    typedef _ContainerType<_WrappedType>        value_type;
//    typedef const _ContainerType<_WrappedType>  const_value_type;
//    typedef _ContainerType<_WrappedType>&       reference;
//    typedef const _ContainerType<_WrappedType>& const_reference;
//    typedef _ContainerType<_WrappedType>*       pointer;
//    typedef const _ContainerType<_WrappedType>* const_pointer;
//    typedef _WrappedType wrapped_type;
//    
//        /*
//         * inner class body *
//         * STL-style Iterator
//         */
//        template <class _OuterClass, class _IndexType = size_t>
//        class _Iterator {
//        public:
//            typedef _OuterClass outer_type;
//            typedef _IndexType  index_type;
//    //        typedef _ValueType  value_type;
//    //        typedef _ValueType& reference;
//    //        typedef _ValueType* pointer;
//        public:
//            _Iterator(outer_type* _Outer) : idx(0), outer_class(_Outer) {}
//            _Iterator(index_type _Idx, outer_type* _Outer) : idx(_Idx), outer_class(_Outer) {}
//            _Iterator(_Iterator const& Iorig) { idx = Iorig.idx; outer_class = Iorig.outer_class; }
//            ~_Iterator() = default;
//
//            /*
//             * Get 
//             */
//            reference operator*() {
//                std::lock_guard<std::mutex> lock(outer_class->m_mutex);
//                return (*outer_class)[ idx ];
//            }
//
//            /*
//             * Prefix increment *
//             */
//            _Iterator& operator++() {
//                increment();
//                return *this;
//            }
//            /*
//             * Postfix increment *
//             */
//            _Iterator operator++(int) {
//                _Iterator old_iter(*this);
//                increment();
//                return (old_iter);
//            } 
//            /*
//             * Prefix decrement *
//             */
//            _Iterator& operator--() {
//                decrement();
//                return *this;
//            } 
//            /*
//             * Postfix decrement *
//             */
//            _Iterator operator--(int) {
//                _Iterator old_iter(*this);
//                decrement();
//                return old_iter;
//            }
//
//            /*
//             * Overloaded compare operators *
//             */
//            friend bool operator!=(_Iterator const& _Left, _Iterator const& _Right) {
//                return _Left.idx != _Right.idx;
//            }
//            friend bool operator==(_Iterator const& _Left, _Iterator const& _Right) {
//                return _Left.idx == _Right.idx;
//            }
//            /*
//             * Returns true if equals end-iterator;
//             */
//            bool is_end() {
//                return outer_class->equals_idx_end(idx);
//            }
//            /*
//             * Non-cycled access *
//             */
//            index_type inc_no_cycle() {
//                if (idx < _Size)
//                    ++idx;
//                return idx;
//            }
//        private:
//    //        #define __CycledIncrement__(dst, max) (++dst) % max
//    //        #define __CycledDecrement__(dst, max) (dst > 0) ? dst - 1 : max - 1
//            void increment() {
//                ++idx %= _Size;
//            }
//            void decrement() {
//                idx = idx > 0 ? idx - 1 : _Size - 1;
//            }
//        private:
//            index_type idx;
//            outer_type* outer_class;
//        };
//    
//    //--------------------------- End of Iterator ----------------------------//
//    
//    /*
//     * Main class body *
//     * _CycledBufferArray
//     */
//    typedef _Iterator< _CycledBufferArray<_ContainerType, _WrappedType, _Size> > Iterator;
//public:
//    _CycledBufferArray() = default;
//    virtual ~_CycledBufferArray() = default;
//    /*
//     * Returns copy of iterator to the first element.
//     */
//    Iterator begin() { std::lock_guard<std::mutex> lock(m_mutex); return Iterator(idx_start, this); }
//    /* 
//     * Returns copy of iterator to the last element. 
//     * Data behind end-iterator is always empty.
//     */
//    Iterator end() { std::lock_guard<std::mutex> lock(m_mutex); return Iterator(idx_end, this); } 
//    /*
//     * Same as queue.front() *
//     */
//    reference front() const {
//        std::lock_guard<std::mutex> lock(m_mutex);
//        return m_data[idx_end];
//    }
//    /*
//     * Adds new element and calculates indexes.
//     */
//    void push(wrapped_type* _Val) {
//        std::lock_guard<std::mutex> lock(m_mutex);
//        m_data[ idx_end ].reset(_Val);
//        (++idx_end) %= _Size;
//        if ( idx_end == idx_start ) {
//            m_data[ idx_end ].reset(nullptr);
//            (++idx_start) %= _Size;
//        }
//    }
//    /*
//     * Random access *
//     */
//    reference operator[](size_t _Idx) {
//        std::lock_guard<std::mutex> lock(m_mutex);
//        return m_data[_Idx%_Size];
//    }
//    bool equals_idx_end(const size_t& _Idx) const {
//        std::lock_guard<std::mutex> lock(m_mutex);
//        return _Idx == (idx_end+1);
//    }
//public:
//    mutable std::mutex m_mutex;
//protected:
//    value_type m_data[ _Size ];
//    size_t idx_end = 0;
//    size_t idx_start = 0;
//};
//
//#define DAHUA_CAM_FPS 25
//#define DAHUA_CAM_BUFFERED_SEC 10
//#define DAHUA_FRAME_BUFFER_SIZE DAHUA_CAM_FPS * DAHUA_CAM_BUFFERED_SEC
//
//typedef _UniqueContainer<AVFrame> Frame;
//typedef _CycledBufferArray< Frame, AVFrame, DAHUA_FRAME_BUFFER_SIZE > FrameBuffer;
                
//============================================================================//
//======================= END OF COMMON TEMPLATE UTILS =======================//
//============================================================================//


//class _FrameQueue {
//public:
//    typedef AVFrame* value_type;
//public:
//    _Frame() {}
//    ~_Frame() {}
//    
//    void push(value_type _val) {
//        m_mutex.lock();
//        m_queue.push(_val);
//        m_mutex.unlock();
//    }
//    value_type front(void) {
//        std::lock_guard<std::mutex> lock(m_mutex);
//        return m_queue.front();
//    }
//    void pop(void) {
//        m_mutex.lock();
//        m_queue.pop();
//        m_mutex.unlock();
//    }
//    
//private:
//    std::mutex m_mutex;
//    std::queue<value_type> m_queue;
//};


class _Translator : public dahua::algorithm::h264::IDataObserver {
public:
    _Translator() { unlock(); }
    virtual ~_Translator() = default;
    void init();
    void deinit();
public:
    void update(dahua::algorithm::h264::Raw&) override final;
    void decode_package(dahua::algorithm::h264::Raw const&);
    void convertToMat(AVFrame*, cv::Mat&);
    void setToCurrent(cv::Mat&);
    
    
    inline void notify() {
        m_condition.notify_all();
    }
    inline void lock() {
        m_lock.lock();
    }
    inline void unlock() {
        m_lock.unlock();
    }
    inline void wait() {
        lock();
        m_condition.wait(m_lock);
        unlock();
    }
    bool has_no_frames() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_frames.empty();
    }
//    FrameBuffer::Iterator createIterator();
private:
    AVPacket*       m_pkt_recieved = nullptr;
    AVPacket*       m_pkt_parsed = nullptr;
    AVCodec*        m_codec = nullptr;
    AVCodecContext* m_ctx = nullptr;
//    const AVBitStreamFilter* m_filter = nullptr;
    AVBSFContext* m_bsf_ctx = nullptr;
    AVCodecParserContext* m_parser = nullptr;
    const cv::Size m_frame_size = cv::Size(2592, 1520);
    int m_frame_count = 0;
    bool m_inited = false;
    mutable std::mutex m_mutex;
    std::condition_variable m_condition;
    mutable std::unique_lock<std::mutex> m_lock = std::move(std::unique_lock<std::mutex>(m_mutex));
    std::queue<AVFrame*> m_frames;
};

typedef _Translator Translator;
                
                
            } // ns playutils
        } // ns process
    } // ns algorithm
} // ns dahua




#endif /* DECODER_HPP */

