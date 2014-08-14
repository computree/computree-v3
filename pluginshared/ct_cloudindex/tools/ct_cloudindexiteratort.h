#ifndef CT_CLOUDINDEXITERATORT_H
#define CT_CLOUDINDEXITERATORT_H

#include <stdlib.h>

template <typename T> class CT_AbstractCloudIndexT;

template<typename T>
class CT_CloudIndexIteratorT
{
public:
    typedef CT_CloudIndexIteratorT<T> self_type;

    CT_CloudIndexIteratorT(const CT_AbstractCloudIndexT<T> &index, const size_t &startPos = 0)
    {
        m_index = (CT_AbstractCloudIndexT<T>*)&index;
        m_currentPos = startPos;
    }

    inline size_t cIndex() { return (*m_index)[m_currentPos]; }
    const T& cT() const { return operator*(); }
    T& cT() { return operator*(); }

    const T& operator*() const { return m_index->tAt(m_currentPos); }
    const T* operator->() const { return &(operator*()); }

    T& operator*() { return m_index->tAt(m_currentPos); }
    T* operator->() { return &(operator*()); }

    self_type& operator++() { ++m_currentPos; return *this; }
    self_type operator+(const size_t &i) { self_type tmp(*this); tmp.m_currentPos += i; return tmp; }
    self_type& operator+=(const size_t &i) { m_currentPos += i; return *this; }

    self_type& operator--() { --m_currentPos; return *this; }
    self_type operator-(const size_t &i) { self_type tmp(*this); tmp.m_currentPos -= i; return tmp; }
    self_type& operator-=(const size_t &i) { m_currentPos -= i; return *this; }

    bool operator==(const self_type &other) const { return ((m_index == other.m_index) && (m_currentPos == other.m_currentPos)); }
    bool operator!=(const self_type &other) const { return ((m_index != other.m_index) ||    (m_currentPos != other.m_currentPos)); }

private:
    CT_AbstractCloudIndexT<T>   *m_index;
    size_t                      m_currentPos;
};

#endif // CT_CLOUDINDEXITERATORT_H
