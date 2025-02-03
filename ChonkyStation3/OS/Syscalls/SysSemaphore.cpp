#include <Syscall.hpp>
#include "PlayStation3.hpp"


MAKE_LOG_FUNCTION(log, sysSemaphore_sc);

u64 Syscall::sysSemaphoreCreate() {
    const u32 sema_id_ptr = ARG0;
    const u32 sema_attr_ptr = ARG1;
    const u32 initial_val = ARG2;
    const u32 max_val = ARG3;
    log("sysSemaphoreCreate(sema_id_ptr: 0x%08x, sema_attr_ptr: 0x%08x, initial_val: %d, max_val: %d)\n", sema_id_ptr, sema_attr_ptr, initial_val, max_val);

    Lv2Semaphore* sema = ps3->lv2_obj.create<Lv2Semaphore>();
    sema->val = initial_val;
    sema->max_val = max_val;
    ps3->mem.write<u32>(sema_id_ptr, sema->handle());

    return Result::CELL_OK;
}

u64 Syscall::sysSemaphoreWait() {
    const u32 sema_id = ARG0;
    const u64 timeout = ARG1;
    log("sysSemaphoreWait(sema_id: %d, timeout: %lld)\n", sema_id, timeout);

    Lv2Semaphore* sema = ps3->lv2_obj.get<Lv2Semaphore>(sema_id);
    sema->wait();

    return Result::CELL_OK;
}

u64 Syscall::sysSemaphorePost() {
    const u32 sema_id = ARG0;
    const u32 val = ARG1;
    log("sysSemaphorePost(sema_id: %d, val: %d)\n", sema_id, val);

    Lv2Semaphore* sema = ps3->lv2_obj.get<Lv2Semaphore>(sema_id);
    sema->post(val);

    return Result::CELL_OK;
}