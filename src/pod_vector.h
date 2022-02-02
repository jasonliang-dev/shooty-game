#pragma once

#include <string.h>

template <typename T> struct PODVector {
    void destroy() { operator delete(m_buffer); }

    void reserve(int capacity) {
        if (capacity > m_capacity) {
            T *buffer = (T *)operator new(sizeof(T) * capacity);
            memcpy(buffer, m_buffer, sizeof(T) * m_size);
            operator delete(m_buffer);
            m_buffer = buffer;
            m_capacity = capacity;
        }
    }

    void resize(int size) {
        reserve(size);
        m_size = size;
    }

    void push_back(const T t) {
        if (m_size == m_capacity) {
            reserve(m_capacity * 2 + 1);
        }

        m_buffer[m_size++] = t;
    }

    void clear() { m_size = 0; }

    int size() const { return m_size; }
    int capacity() const { return m_capacity; }
    T *data() { return m_buffer; }
    const T *data() const { return m_buffer; }
    T *begin() { return m_buffer; }
    const T *begin() const { return m_buffer; }
    T *end() { return m_buffer + m_size; }
    const T *end() const { return m_buffer + m_size; }
    T &front() { return m_buffer[0]; }
    const T &front() const { return m_buffer[0]; }
    T &back() { return m_buffer[m_size - 1]; }
    const T &back() const { return m_buffer[m_size - 1]; }
    T &operator[](int i) { return m_buffer[i]; }
    const T &operator[](int i) const { return m_buffer[i]; }

  private:
    int m_size = 0;
    int m_capacity = 0;
    T *m_buffer = nullptr;
};
