//
// Created by Елизавета Кравченкова on 22.03.2023.
//
#pragma once

#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <algorithm>
#include <queue>

class Pool {
public:
    size_t size_of_piece;
    size_t count_of_piece;
    size_t sizeof_type;
    uint8_t* ptr = nullptr;
    //bool Iamfree=false;

    Pool(size_t si, size_t ni, size_t siseOf) : size_of_piece(si), count_of_piece(ni), count_free(ni),
                                                sizeof_type(siseOf) {
        //is_free.assign(ni,true);
        for (uint64_t i = 0; i < ni; i++) {
            free_place.push(i);
        }
    }

    void* give_me() {
        if (count_free == 0)
            throw std::bad_alloc();
        uint8_t* ans1 = ptr + free_place.front() * sizeof_type;
        free_place.pop();
        --count_free;
        return static_cast<void*>(ans1);
    }

    void i_will_free(void* p) {
        size_t ind = (static_cast<uint8_t*>(p) - ptr) / sizeof_type;
        free_place.push(ind);
        ++count_free;
        return;
    }

    size_t get_count_of_free_place() {
        return count_free;
    }

    Pool& operator=(const Pool& other) {
        //is_free=other.is_free;
        //next_free_piece=other.next_free_piece;
        count_free = other.count_free;
        ptr = other.ptr;
        size_of_piece = other.size_of_piece;
        count_of_piece = other.count_of_piece;
        free_place = other.free_place;
        return *this;
    }


private:
    size_t count_free;
    std::queue<size_t> free_place;
};

bool PoolCmp(const Pool& a, const Pool& b) {
    return a.size_of_piece < b.size_of_piece;
}

// формат args S1,n1,S2,n2...Sk,nk
// к-количество пулов, Si-размер чанка в i пуле, ni-количество чанков в i пуле
template<typename T, size_t...args>
class MemoryPoolsAllocator {
public:
    typedef T value_type;
    typedef size_t size_type;
    typedef std::ptrdiff_t difference_type;

    MemoryPoolsAllocator() {
        //std::cout<<"MemoryPoolsAllocator<T,args>"<<std::endl;
        auto il = std::initializer_list<size_t>{args...};

        //std::cout<<"Размерр аргументов: "<<il.size()<<std::endl;

        auto it = il.begin();
        this->pools.reserve(il.size() / 2);
        for (uint64_t i = 0; i < (il.size() / 2); i++) {
            size_t s = *it;
            ++it;
            //std::cout<<s<<" "<<*it<<"      ";
            this->pools.emplace_back(s, *it, sizeof(T));
            if (auto p = static_cast<uint8_t*>(std::malloc(
                    this->pools[i].count_of_piece * this->pools[i].size_of_piece * sizeof(T)))) {
                //this->report(nullptr, this->pools[i].count_of_piece*this->pools[i].size_of_piece);
                this->pools[i].ptr = p;
            } else {
                throw std::bad_alloc();
            }
            ++it;
        }
        std::sort(this->pools.begin(), this->pools.end(), PoolCmp);
    }

    size_type max_size() {
        if (pools.empty()) {
            return 0;
        }
        uint64_t ind = pools.size() - 1;
        while (pools[ind].get_count_of_free_place() == 0) {
            --ind;
            if (ind == -1) {
                return 0;
            }
        }
        return pools[ind].size_of_piece;
        //return std::numeric_limits<std::size_t>::max() / sizeof(value_type);
    }

    [[nodiscard]] T* allocate(std::size_t n) {
        if (n > max_size())
            throw std::bad_array_new_length();

        int number_of_pool = left_bin_reseach(n);

        if (number_of_pool != -1) {
            while (pools[number_of_pool].get_count_of_free_place() == 0) {
                ++number_of_pool;
                if (number_of_pool == pools.size()) {
                    throw std::bad_alloc();
                }
            }
            return static_cast<T*>(pools[number_of_pool].give_me());
        } else {
            throw std::bad_alloc();
        }
    }

    void deallocate(T* p, std::size_t n) noexcept {
        int ind = left_bin_reseach(n);
        while (pools[ind].ptr == nullptr) {
            ++ind;
        }
        while (static_cast<void*>(p) < static_cast<void*>(pools[ind].ptr) || static_cast<void*>(p) >
                                                                             static_cast<void*>(pools[ind].ptr +
                                                                                                sizeof(value_type) *
                                                                                                pools[ind].size_of_piece *
                                                                                                pools[ind].count_of_piece)) {
            ++ind;
        }
        pools[ind].i_will_free(static_cast<void*>(p));
    }

    template<typename U>
    struct rebind {
        using other = MemoryPoolsAllocator<U, args...>;
    };

    template<typename U, size_t ...Args>
    MemoryPoolsAllocator(const MemoryPoolsAllocator<U, Args...>& other):pools(other.pools) {
        count_copy++;
    }

    ~MemoryPoolsAllocator() {
        count_copy--;
        if (count_copy == 0) {
            for (uint64_t i = 0; i < pools.size(); i++) {
                std::free(pools[i].ptr);
            }
        }
    }

protected:
    std::vector<Pool> pools;
    size_t count_copy = 1;

    int left_bin_reseach(std::size_t n) {
        int len_vec = pools.size();
        int l = -1;
        int r = len_vec;
        while (l < (r - 1)) {
            int m = (l + r) / 2;
            if (pools[m].size_of_piece < n) {
                l = m;
            } else {
                r = m;
            }
        }
        if (r == len_vec) {
            return -1;
        }
        return r;
    }
};
