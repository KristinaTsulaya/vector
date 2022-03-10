#include <iostream>
using namespace std;
template <typename T>
class Vector {
private:
    T* array;
    size_t size;
    size_t capacity;
    
public:
    Vector() : array(nullptr), size(0), capacity(0) {}
    
    
    Vector(const Vector& other) : size(other.size), capacity(other.capacity) {     // Глубокое копирование
        array = reinterpret_cast<T*>(new int8_t[other.capacity * sizeof(T)]);
        for (int i = 0; i < size; ++i) {
            new (array + i) T(other.array[i]);
        }
    }
    
    Vector(Vector&& other) : size(other.size), array(other.array), capacity(other.capacity) {
        other.array = nullptr;
        other.size = 0;
    }
    
    Vector& operator=(Vector&& other) {
        for (size_t i = 0; i < size; ++i) {
            (array + i)->~T();
        }
        delete[] reinterpret_cast<int8_t*>(array);
        
        size = other.size;
        capacity = other.capacity;
        
        array = other.array;
        other.array = nullptr;
        return *this;
    }
    
    Vector& operator=(const Vector& other) {
        
        if (this == &other) {
            return *this;
        }
        
        for (size_t i = 0; i < size; ++i) {
            (array + i)->~T();
        }
        delete reinterpret_cast<int8_t*>(array);
        
        size = other.size;
        capacity = other.capacity;
        
        array = new int8_t[capacity * sizeof(T)];
        for (int i = 0; i < size; ++i) {
            new (array + i) T(other.array[i]);
        }
        
        return *this;
    }

    void reserve(size_t n) {
        if (capacity >= n) {
            return;
        }
        
        T* newArray = reinterpret_cast<T*>(new int8_t[n * sizeof(T)]);
        
        size_t i = 0;
        try {
            for (; i < size; ++i) {
                new(newArray + i) T(array[i]); // placement new
            }
        } catch (...) {
            for (size_t j = 0; j < i; ++j) {
                (newArray + i)->~T();
            }
            delete[] reinterpret_cast<int8_t*>(array);
            throw;
        }
        
        for (size_t i = 0; i < size; ++i) {
            (array + i)->~T();
        }
        
        delete[] reinterpret_cast<int8_t*>(array);
        array = newArray;
        capacity = n;
    }
    
    
    void resize(size_t n, const T& value = T()) {
        if (n > capacity) {
            reserve(n);
        }
        for (size_t i = size; i < n; ++i) {
            new (array + i) T(value);
        }
        size = n;
    }
    
    void push_back(const T& value) {
        if (capacity == size) {
            reserve(2 * size);
        }
        new (array + size) T(value);
    }
    
    void pop_back() {
        --size;
    }
    
    int get_capacity() {
        return capacity;
    }
    
    int get_size() {
        return size;
    }
    
    ~Vector() {
        for (size_t i = 0; i < size; ++i) {
            (array + i)->~T();
        }
        delete[] reinterpret_cast<int8_t*>(array);
    }
    
    
};

int main() {
    Vector<int> v;
        v.reserve(16);
        for (int i = 0; i < 16; ++i) {
            int value;
            cin >> value;
            v.push_back(value);
        }
    return 0;
}



