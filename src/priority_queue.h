#pragma once

#include <assert.h>
#include <string.h>

template <typename T> struct PriorityQueue {
    void destroy() {
        delete[] m_data;
        delete[] m_costs;
    }

    void reserve(int capacity) {
        if (capacity <= m_capacity) {
            return;
        }

        T *data = (T *)operator new(sizeof(T) * capacity);
        memcpy(data, m_data, sizeof(T) * m_size);
        operator delete(m_data);
        m_data = data;

        float *costs = new float[capacity];
        memcpy(costs, m_costs, sizeof(float) * m_size);
        delete[] m_costs;
        m_costs = costs;

        m_capacity = capacity;
    }

    void push_min(T item, float cost) {
        if (m_size == m_capacity) {
            reserve(m_size * 2 + 1);
        }

        m_data[m_size] = item;
        m_costs[m_size] = cost;
        m_size++;

        int j = m_size / 2;
        for (int i = m_size; m_costs[i - 1] < m_costs[j - 1]; i /= 2) {
            swap(i - 1, j - 1);
            j /= 2;
        }
    }

    T pop_min() {
        assert(m_size != 0);
        T top = m_data[0];

        m_data[0] = m_data[m_size - 1];
        m_costs[0] = m_costs[m_size - 1];
        m_size--;

        int i = 0;
        int j = 1;
        while (i != j) {
            i = j;
            if (i * 2 + 1 <= m_size) {
                if (m_costs[i - 1] >= m_costs[i * 2 - 1]) {
                    j = i * 2;
                }
                if (m_costs[j - 1] >= m_costs[i * 2]) {
                    j = i * 2 + 1;
                }
            } else if (i * 2 <= m_size) {
                if (m_costs[i - 1] >= m_costs[i * 2 - 1]) {
                    j = i * 2;
                }
            }

            if (i != j) {
                swap(i - 1, j - 1);
            }
        }

        return top;
    }

    int size() const { return m_size; }
    T *data() { return m_data; }
    void clear() { m_size = 0; }

  private:
    void swap(int i, int j) {
        T t = m_data[i];
        m_data[i] = m_data[j];
        m_data[j] = t;

        float f = m_costs[i];
        m_costs[i] = m_costs[j];
        m_costs[j] = f;
    }

    T *m_data = nullptr;
    float *m_costs = nullptr;
    int m_size = 0;
    int m_capacity = 0;
};
