#include "app.hpp"
//template<class T>
//int value::Ptr<T>::m_ptr_count = 0;
//
//template<class T>
//value::Ptr<T>::Ptr(T* _data) : m_data(_data) {}
//template<class T>
//value::Ptr<T>::Ptr(Ptr const& _ptr) { 
//    _ptr.m_data = m_data; 
//    m_ptr_count++; 
//}
//template<class T>
//value::Ptr<T>::~Ptr() {
//    if (--m_ptr_count == 0)
//        delete m_data; 
//}
//template <class T>
//T* const value::Ptr<T>::operator->() {
//    return m_data;
//}
//template <class T>
//T const& value::Ptr<T>::operator*() {
//    return *m_data;
//}
//template <class T>
//T* value::Ptr<T>::get() {
//    return m_data;
//}
//template<class T>
//void value::Ptr<T>::reset() {
//    if (--m_ptr_count == 0)
//        delete m_data;
//    m_data = nullptr;
//}
//template<class T>
//void value::Ptr<T>::reset(T* new_ptr) {
//    if (--m_ptr_count == 0)
//        delete m_data;
//    m_data = new_ptr;
//}