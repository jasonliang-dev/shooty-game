#pragma once

#include <initializer_list>
#include <new>
#include <string.h>

template <typename T> struct PODVector {
    PODVector() = default;
    PODVector(int capacity) { reserve(capacity); }

    PODVector(std::initializer_list<T> list) {
        reserve((int)list.size());
        for (auto &it : list) {
            push_back(std::move(it));
        }
    }

    ~PODVector() {
        operator delete(m_buffer);
    }

    PODVector(const PODVector &other) {
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        m_buffer = operator new(sizeof(T) * m_capacity);

        memcpy(m_buffer, other.m_buffer, sizeof(T) * m_size);
    }

    PODVector &operator=(const PODVector &other) {
        if (this != &other) {
            if (m_capacity < other.m_capacity) {
                m_capacity = other.m_capacity;
                operator delete(m_buffer);
                m_buffer = (T *)operator new(sizeof(T) * m_capacity);
            }

            m_size = other.m_size;
            memcpy(m_buffer, other.m_buffer, sizeof(T) * m_size);
        }

        return *this;
    }

    PODVector(PODVector &&other) noexcept {
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        m_buffer = other.m_buffer;

        other.m_size = 0;
        other.m_capacity = 0;
        other.m_buffer = nullptr;
    }

    PODVector &operator=(PODVector &&other) noexcept {
        if (this != &other) {
            operator delete(m_buffer);

            m_size = other.m_size;
            m_capacity = other.m_capacity;
            m_buffer = other.m_buffer;

            other.m_size = 0;
            other.m_capacity = 0;
            other.m_buffer = nullptr;
        }

        return *this;
    }

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
            reserve(m_capacity * 2);
        }

        m_buffer[m_size] = t;
        m_size++;
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
    T &operator[](int i) { return m_buffer[i]; }
    const T &operator[](int i) const { return m_buffer[i]; }

  private:
    int m_size = 0;
    int m_capacity = 0;
    T *m_buffer = nullptr;
};
