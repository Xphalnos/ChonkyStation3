#pragma once

#include <common.hpp>


class PlayStation3;

class Lv2Object {
public:
    Lv2Object(PlayStation3* ps3, u64 handle) : ps3(ps3), handle(handle) {}
    PlayStation3* ps3;
    u64 handle;

    template<typename T>
    void create() {
        data = (void*)(new T(this));
    }

    template<typename T>
    T* get() { return (T*)data; }
    
    void* data;
};