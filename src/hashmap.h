#pragma once

#include <new>
#include <string.h>

template <typename> struct HashMapTraits;

template <> struct HashMapTraits<int> {
    static int hash(int n) { return n * 37; }
    static bool equal(int a, int b) { return a == b; }
};

template <> struct HashMapTraits<const char *> {
    // djb2
    static int hash(const char *str) {
        int hash = 5381;
        while (*str) {
            hash = ((hash << 5) + hash) + *str;
            str++;
        }
        return hash;
    }

    static bool equal(const char *a, const char *b) {
        return strcmp(a, b) == 0;
    }
};

template <typename K, typename V, typename Traits = HashMapTraits<K>> struct HashMap {
    HashMap() = default;
    HashMap(int capacity) { reserve(capacity); }

    ~HashMap() {
        for (int i = 0; i < m_capacity; i++) {
            if (m_table[i].type == SlotType::Occupied) {
                m_table[i].~KV();
            }
        }

        operator delete(m_table);
    }

    HashMap(const HashMap &other) {
        m_table = (KV *)operator new(sizeof(KV) * other.m_capacity);
        memset(m_table, 0, sizeof(KV) * other.m_capacity);
        m_capacity = other.m_capacity;
        m_size = other.m_size;

        for (int i = 0; i < m_capacity; i++) {
            if (other.m_table[i].type == SlotType::Occupied) {
                new (m_table + i) KV(other.m_table[i]);
            } else {
                m_table[i] = other.m_table[i];
            }
        }
    }

    HashMap &operator=(const HashMap &other) {
        if (this == &other) {
            return *this;
        }

        for (int i = 0; i < m_capacity; i++) {
            if (m_table[i].type == SlotType::Occupied) {
                m_table[i].~KV();
                m_table[i].type = SlotType::Unoccupied;
            } else if (m_table[i].type == SlotType::Tombstone) {
                m_table[i].type = SlotType::Unoccupied;
            }
        }

        if (m_capacity < other.m_capacity) {
            operator delete(m_table);
            m_table = (KV *)operator new(sizeof(KV) * other.m_capacity);
            memset(m_table, 0, sizeof(KV) * other.m_capacity);
            m_capacity = other.m_capacity;
        }

        m_size = other.m_size;

        for (int i = 0; i < m_capacity; i++) {
            if (other.m_table[i].type != SlotType::Occupied) {
                continue;
            }

            int idx = Traits::hash(other.m_table[i].key) & (m_capacity - 1);

            for (int j = idx; j < m_capacity; j++) {
                if (m_table[j].type != SlotType::Occupied) {
                    new (m_table + j) KV(other.m_table[i]);
                    goto next;
                }
            }

            for (int j = 0; j < idx; j++) {
                if (m_table[j].type != SlotType::Occupied) {
                    new (m_table + j) KV(other.m_table[i]);
                    goto next;
                }
            }
        next:;
        }

        return *this;
    }

    HashMap(HashMap &&other) noexcept {
        m_table = other.m_table;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        other.m_table = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    HashMap &operator=(HashMap &&other) noexcept {
        if (this != &other) {
            for (int i = 0; i < m_capacity; i++) {
                if (m_table[i].type == SlotType::Occupied) {
                    m_table[i].~KV();
                }
            }

            operator delete(m_table);

            m_table = other.m_table;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            other.m_table = nullptr;
            other.m_size = 0;
            other.m_capacity = 0;
        }

        return *this;
    }

    void reserve(int capacity) {
        const auto next_pow_of_2 = [](int n) {
            n--;
            n |= n >> 1;
            n |= n >> 2;
            n |= n >> 4;
            n |= n >> 8;
            n |= n >> 16;
            return n + 1;
        };

        if (capacity < m_capacity * LOAD_FACTOR) {
            return;
        }

        capacity = next_pow_of_2(m_capacity + 1);

        KV *table = (KV *)operator new(sizeof(KV) * capacity);
        memset(table, 0, sizeof(KV) * capacity);

        for (int i = 0; i < m_capacity; i++) {
            if (m_table[i].type != SlotType::Occupied) {
                continue;
            }

            int idx = Traits::hash(m_table[i].key) & (capacity - 1);

            for (int j = idx; j < capacity; j++) {
                if (table[j].type != SlotType::Occupied) {
                    new (table + j) KV(std::move(m_table[i]));
                    m_table[i].~KV();
                    goto next;
                }
            }

            for (int j = 0; j < idx; j++) {
                if (table[j].type != SlotType::Occupied) {
                    new (table + j) KV(std::move(m_table[i]));
                    m_table[i].~KV();
                    goto next;
                }
            }
        next:;
        }

        operator delete(m_table);

        m_table = table;
        m_capacity = capacity;
    }

    void insert(const K &k, const V &v) {
        reserve(m_size + 1);

        int idx = Traits::hash(k) & (m_capacity - 1);

        for (int i = idx; i < m_capacity; i++) {
            if (m_table[i].type == SlotType::Unoccupied) {
                new (m_table + i) KV(k, v, SlotType::Occupied);
                m_size++;
                return;
            } else if (m_table[i].type == SlotType::Tombstone) {
                new (m_table + i) KV(k, v, SlotType::Occupied);
                return;
            }
        }

        for (int i = 0; i < idx; i++) {
            if (m_table[i].type == SlotType::Unoccupied) {
                new (m_table + i) KV(k, v, SlotType::Occupied);
                m_size++;
                return;
            } else if (m_table[i].type == SlotType::Tombstone) {
                new (m_table + i) KV(k, v, SlotType::Occupied);
                return;
            }
        }
    }

    V *find(const K &k) {
        KV *kv = real_find(k);
        return kv ? &kv->value : nullptr;
    }

    const V *find(const K &k) const {
        KV *kv = real_find(k);
        return kv ? &kv->value : nullptr;
    }

    void remove(const K &k) {
        KV *kv = real_find(k);
        if (kv) {
            kv->~KV();
            kv->type = SlotType::Tombstone;
        }
    }

    V &operator[](const K &k) {
        reserve(m_size + 1);

        int idx = Traits::hash(k) & (m_capacity - 1);

        for (int i = idx; i < m_capacity; i++) {
            if (m_table[i].type == SlotType::Unoccupied) {
                new (m_table + i) KV(k, V{}, SlotType::Occupied);
                m_size++;
                return m_table[i].value;
            } else if (m_table[i].type == SlotType::Tombstone) {
                new (m_table + i) KV(k, V{}, SlotType::Occupied);
                return m_table[i].value;
            } else if (Traits::equal(m_table[i].key, k)) {
                return m_table[i].value;
            }
        }

        for (int i = 0; i < idx; i++) {
            if (m_table[i].type == SlotType::Unoccupied) {
                new (m_table + i) KV(k, V{}, SlotType::Occupied);
                m_size++;
                return m_table[i].value;
            } else if (m_table[i].type == SlotType::Tombstone) {
                new (m_table + i) KV(k, V{}, SlotType::Occupied);
                return m_table[i].value;
            } else if (Traits::equal(m_table[i].key, k)) {
                return m_table[i].value;
            }
        }

        // unreachable?
        return m_table[0].value;
    }

    int size() const { return m_size; }
    int capacity() const { return m_capacity; }

  private:
    struct KV;

    KV *real_find(const K &k) const {
        if (m_capacity == 0) {
            return nullptr;
        }

        int idx = Traits::hash(k) & (m_capacity - 1);

        for (int i = idx; i < m_capacity; i++) {
            if (m_table[i].type == SlotType::Tombstone) {
                continue;
            } else if (m_table[i].type == SlotType::Unoccupied) {
                return nullptr;
            } else if (Traits::equal(m_table[i].key, k)) {
                return &m_table[i];
            }
        }

        for (int i = 0; i < idx; i++) {
            if (m_table[i].type == SlotType::Tombstone) {
                continue;
            } else if (m_table[i].type == SlotType::Unoccupied) {
                return nullptr;
            } else if (Traits::equal(m_table[i].key, k)) {
                return &m_table[i];
            }
        }

        return nullptr;
    }

    enum struct SlotType : u8 {
        Unoccupied = 0,
        Tombstone,
        Occupied,
    };

    struct KV {
        K key;
        V value;
        SlotType type;
    };

    static constexpr float LOAD_FACTOR = 0.75f;

    KV *m_table = nullptr;
    int m_size = 0;
    int m_capacity = 0;
};
