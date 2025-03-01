#include <Syscall.hpp>
#include "PlayStation3.hpp"
#include <SPU/SPULoader.hpp>
#include <Lv2Objects/Lv2SPUThreadGroup.hpp>
#include "sys_spu.hpp"


MAKE_LOG_FUNCTION(log_sys_spu, sys_spu);

using namespace sys_spu;

u64 Syscall::sys_raw_spu_create() {
    const u32 id_ptr = ARG0;
    const u32 attr_ptr = ARG1;
    log_sys_spu("sys_raw_spu_create(id_ptr: 0x%08x, attr_ptr: 0x%08x) STUBBED\n", id_ptr, attr_ptr);

    ps3->mem.write<u32>(id_ptr, 0);
    ps3->mem.spu.alloc(SPU_MEM_SIZE);
    
    return Result::CELL_OK;
}

u64 Syscall::sys_spu_thread_group_create() {
    const u32 id_ptr = ARG0;
    const u32 num = ARG1;
    const u32 prio = ARG2;
    const u32 attr_ptr = ARG3;
    log_sys_spu("sys_spu_thread_group_create(id_ptr: 0x%08x, num: %d, prio: %d, attr_ptr: 0x%08x)\n", id_ptr, num, prio, attr_ptr);

    Lv2SPUThreadGroup* group = ps3->lv2_obj.create<Lv2SPUThreadGroup>();
    sys_spu_thread_group_attribute* attr = (sys_spu_thread_group_attribute*)ps3->mem.getPtr(attr_ptr);
    group->attr = attr;

    log_sys_spu("Created SPU thread group \"%s\"\n", group->getName(ps3).c_str());
    ps3->mem.write<u32>(id_ptr, group->handle());

    return Result::CELL_OK;
}

u64 Syscall::sys_spu_thread_initialize() {
    const u32 id_ptr = ARG0;
    const u32 group_id = ARG1;
    const u32 spu_num = ARG2;
    const u32 img_ptr = ARG3;
    const u32 attr_ptr = ARG4;
    const u32 arg_ptr = ARG5;
    log_sys_spu("sys_spu_thread_initialize(id_ptr: 0x%08x, group_id: %d, spu_num: %d, img_ptr: 0x%08x, attr_ptr: 0x%08x, arg_ptr: 0x%08x)\n", id_ptr, group_id, spu_num, img_ptr, attr_ptr, arg_ptr);

    sys_spu_image* img = (sys_spu_image*)ps3->mem.getPtr(img_ptr);
    sys_spu_thread_attribute* attr = (sys_spu_thread_attribute*)ps3->mem.getPtr(attr_ptr);
    const auto name = Helpers::readString(ps3->mem.getPtr(attr->name_ptr));
    
    auto thread = ps3->spu_thread_manager.createThread(name);
    thread->loadImage(img);

    return Result::CELL_OK;
}

u64 Syscall::sys_spu_image_import() {
    const u32 image_ptr = ARG0;
    const u32 src = ARG1;
    const u32 size = ARG2;
    const u32 arg4 = ARG3;
    log_sys_spu("sys_spu_image_import(image_ptr: 0x%08x, src: 0x%08x, size: 0x%08x, arg4: 0x%08x) UNIMPLEMENTED\n", image_ptr, src, size);

    Helpers::panic("TODO: sys_spu_image_import syscall\n");
    return Result::CELL_OK;
}

u64 Syscall::sys_spu_thread_group_join() {
    log_sys_spu("sys_spu_thread_group_join() UNIMPLEMENTED\n");

    ps3->thread_manager.getCurrentThread()->status = Thread::ThreadStatus::Sleeping;   // Never wake up
    ps3->thread_manager.getCurrentThread()->reschedule();

    return Result::CELL_OK;
}

u64 Syscall::sys_spu_thread_group_connect_event_all_threads() {
    const u32 id = ARG0;
    const u32 eq = ARG1;
    const u64 req = ARG2;
    const u32 spup_ptr = ARG3;  // spup is u8
    log_sys_spu("sys_spu_thread_group_connect_event_all_threads(id: %d, eq: %d, req: %d, spup_ptr: 0x%08x) UNIMPLEMENTED\n", id, eq, req, spup_ptr);

    ps3->mem.write<u8>(spup_ptr, 0);
    return Result::CELL_OK;
}