#pragma once

#include <common.hpp>

#include <queue>

#include <Lv2Base.hpp>


// Circular dependency
class PlayStation3;

class Lv2Mutex : public virtual Lv2Base {
public:
    Lv2Mutex(Lv2Object* obj) : Lv2Base(obj) {}

    u32 owner = -1; // -1 (0xffffffff) == free
    std::queue<u64> wait_list;

    bool isFree();
    void lock(PlayStation3* ps3);
    void unlock(PlayStation3* ps3);
};