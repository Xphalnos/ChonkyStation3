#include "Syscall.hpp"
#include <PlayStation3.hpp>


Syscall::Syscall(PlayStation3* ps3) {
    this->ps3 = ps3;
}

void Syscall::todo(std::string name) {
    unimpl((name + " UNIMPLEMENTED\n").c_str());
    ps3->ppu->state.gprs[3] = Result::CELL_OK;;
}

void Syscall::doSyscall(bool decrement_pc_if_module_call) {
    const auto syscall_num = ps3->ppu->state.gprs[11];

    const u16 stub = ps3->mem.read<u32>(ps3->ppu->state.pc) & 0xffff;
    switch (stub) {
    // Module call
    case 0x10: {
        // Normally the stub does this to jump to the function (mtctr + bctr), I don't think it matters but to be sure I added this here
        ps3->ppu->state.ctr = ps3->ppu->state.gprs[0];
        // import addr is stored in r12
        ps3->module_manager.call(ps3->module_manager.imports[ps3->ppu->state.gprs[12]]);
        return;
    }
    case 0x2010: {
        // Normally the stub does this to jump to the function (mtctr + bctr), I don't think it matters but to be sure I added this here
        ps3->ppu->state.ctr = ps3->ppu->state.gprs[0];

        const u32 stub_addr = ps3->mem.read<u32>(ps3->ppu->state.gprs[2]);
        ps3->module_manager.call(ps3->module_manager.imports[stub_addr]);
        return;
    }
    case 0x1010: {
        ps3->module_manager.lle(ps3->module_manager.imports[ps3->ppu->state.gprs[12]]);
        //ps3->ppu->state.pc -= 4;
        return;
    }
    }

    switch (syscall_num) {
    
    // cellGcmCallback
    case 0x06: ps3->module_manager.cellGcmSys.cellGcmCallback(); break;

    case 1: {
        log_misc("sys_process_getpid()\n");
        ps3->ppu->state.gprs[3] = 1;
        break;
    }
    case 14:    todo("sys_process_is_spu_lock_line_reservation_address()");   break;
    case 25: {
        const u32 pid = ARG0;
        const u32 ver_ptr = ARG1;
        // TODO: Get this from the elf's PROC_PARAM
        unimpl("sys_process_get_sdk_version() STUBBED\n");
        ps3->mem.write<u32>(ver_ptr, 0x00360001);
        ps3->ppu->state.gprs[3] = Result::CELL_OK;
        break;
    }
    case 30:    todo("_sys_process_get_paramsfo()");    break;
    case 43: {
        log_misc("sys_ppu_thread_yield() STUBBED\n");
        ps3->thread_manager.getCurrentThread()->reschedule();
        ps3->ppu->state.gprs[3] = Result::CELL_OK;
        break;
    }
    case 47:    todo("sys_ppu_thread_set_priority()");  break;
    case 48: {
        log_misc("sys_ppu_thread_get_priority() STUBBED\n");
        ps3->mem.write<u32>(ARG1, 1);
        ps3->ppu->state.gprs[3] = Result::CELL_OK;
        break;
    }
    case 82:    todo("sys_event_flag_create()");                            break;
    case 90:    ps3->ppu->state.gprs[3] = sys_semaphore_create();           break;
    case 91:    todo("sys_semaphore_destroy()");                            break;
    case 92:    ps3->ppu->state.gprs[3] = sys_semaphore_wait();             break;
    case 94:    ps3->ppu->state.gprs[3] = sys_semaphore_post();             break;
    case 95:    todo("_sys_lwmutex_create()");                              break;
    case 96:    todo("_sys_lwmutex_destroy()");                             break;
    case 97:    todo("_sys_lwmutex_lock()");                                break;
    case 98:    todo("_sys_lwmutex_unlock()");                              break;
    case 100:   ps3->ppu->state.gprs[3] = sys_mutex_create();               break;
    case 101:   todo("sys_mutex_destroy()");                                break;
    case 102:   ps3->ppu->state.gprs[3] = sys_mutex_lock();                 break;
    case 104:   ps3->ppu->state.gprs[3] = sys_mutex_unlock();               break;
    case 105:   ps3->ppu->state.gprs[3] = sys_cond_create();                break;
    case 106:   todo("sys_cond_destroy()");                                 break;
    case 109:   todo("sys_cond_signal_all()");                              break;
    case 114:   ps3->ppu->state.gprs[3] = sys_semaphore_get_value();        break;
    case 120:   todo("sys_rwlock_create()");                                break;
    case 124:   todo("sys_rwlock_runlock()");                               break;
    case 125:   todo("sys_rwlock_wlock()");                                 break;
    case 127:   todo("sys_rwlock_wunlock()");                               break;
    case 128:   ps3->ppu->state.gprs[3] = sys_event_queue_create();         break;
    case 130:   ps3->ppu->state.gprs[3] = sys_event_queue_receive();        break;
    case 134:   ps3->ppu->state.gprs[3] = sys_event_port_create();          break;
    case 136:   ps3->ppu->state.gprs[3] = sys_event_port_connect_local();   break;
    case 141:   ps3->ppu->state.gprs[3] = sys_timer_usleep();               break;
    case 142:   ps3->ppu->state.gprs[3] = sys_timer_sleep();                break;
    case 144: {
        log_misc("sys_time_get_timezone()\n");
        ps3->mem.write<u32>(ARG0, 60);  // timezone (60 == UTC+1 I think)
        ps3->mem.write<u32>(ARG1, 0);   // summertime
        ps3->ppu->state.gprs[3] = Result::CELL_OK;
        break;
    }
    case 145: {
        log_misc("sysTimeGetCurrentTime() STUBBED\n");
        ps3->mem.write<u64>(ARG0, 1000);
        ps3->mem.write<u64>(ARG1, 1000);
        ps3->ppu->state.gprs[3] = Result::CELL_OK;
        break;
    }
    case 147: {
        log_misc("sys_time_get_timebase_frequency() UNIMPLEMENTED\n");
        ps3->ppu->state.gprs[3] = 80000000ull;
        break;
    }
    case 160:   todo("sys_raw_spu_create()");                                                   break;
    case 169:   todo("sys_spu_initialize()");                                                   break;
    case 170:   todo("sys_spu_thread_group_create()");                                          break;
    case 172:   todo("sys_spu_thread_initialize()");                                            break;
    case 173:   todo("sys_spu_thread_group_start()");                                           break;
    case 174:   todo("sys_spu_thread_group_suspend()");                                         break;
    case 178:   todo("sys_spu_thread_group_join()");                                            break;
    case 182:   todo("sys_spu_thread_read_ls()");                                               break;
    case 185:   todo("sys_spu_thread_group_connect_event()");                                   break;
    case 190:   todo("sys_spu_thread_write_spu_mb()");                                          break;
    case 250:   todo("sys_spu_thread_group_set_cooperative_victims()");                         break;
    case 251:   ps3->ppu->state.gprs[3] = sys_spu_thread_group_connect_event_all_threads();     break;
    case 253:   todo("sys_spu_thread_group_syscall_253()");                                     break;
    case 254:   todo("sys_spu_thread_group_log()");                                             break;
    case 330:   ps3->ppu->state.gprs[3] = sys_mmapper_allocate_address();                       break;
    case 324:   todo("sys_memory_container_create()");                                          break;
    case 331:   todo("sys_mmapper_free_address()");                                             break;
    case 337:   ps3->ppu->state.gprs[3] = sys_mmapper_search_and_map();                         break;
    case 341:   todo("sys_memory_container_create()");                                          break;
    case 348:   ps3->ppu->state.gprs[3] = sys_memory_allocate();                                break;
    case 352:   ps3->ppu->state.gprs[3] = sys_memory_get_user_memory_size();                    break;
    case 403: {   // puts
        std::string str;
        u8* ptr = ps3->mem.getPtr(ARG1);
        for (int i = 0; i < ARG2; i++)
            str += *ptr++;
        tty("%s", str.c_str());
        ps3->ppu->state.gprs[3] = Result::CELL_OK;
        break;
    }
    case 462:   ps3->ppu->state.gprs[3] = Result::CELL_OK;  break;  // Debug syscall unavailable on retail consoles (liblv2 tries to use this)
    case 480: {
        const u32 name_ptr = ARG0;
        const u64 flags = ARG1;
        const u32 opt_ptr = ARG2;
        std::string name = Helpers::readString(ps3->mem.getPtr(name_ptr));
        unimpl("_sys_prx_load_module(name_ptr: 0x%08x, flags: 0x%016llx, opt_ptr: 0x%08x) [name: %s] UNIMPLEMENTED\n", name_ptr, flags, opt_ptr, name.c_str());

        ps3->ppu->state.gprs[3] = ps3->handle_manager.request();
        break;
    }
    case 481:   ps3->ppu->state.gprs[3] = Result::CELL_OK;    unimpl("_sys_prx_start_module() UNIMPLEMENTED\n");  break;
    case 484: {
        const u32 name_ptr = ARG0;
        const u32 opt_ptr = ARG1;
        std::string name = Helpers::readString(ps3->mem.getPtr(name_ptr));
        unimpl("_sys_prx_register_module(name_ptr: 0x%08x, opt_ptr: 0x%08x) [name: %s] UNIMPLEMENTED\n", name_ptr, opt_ptr, name.c_str());

        ps3->ppu->state.gprs[3] = Result::CELL_OK;
        break;
    }
    case 486:   todo("_sys_prx_register_library()\n");                              break;
    case 801:   ps3->ppu->state.gprs[3] = ps3->module_manager.cellFs.cellFsOpen();  break;
    case 802:   ps3->ppu->state.gprs[3] = ps3->module_manager.cellFs.cellFsRead();  break;
    case 804:   ps3->ppu->state.gprs[3] = ps3->module_manager.cellFs.cellFsClose(); break;
    case 808:   ps3->ppu->state.gprs[3] = ps3->module_manager.cellFs.cellFsStat();  break;
    case 809: {
        const u32 fd = ARG0;
        const u32 stat_ptr = ARG1;

        if (fd == 1) {  // stdout?
            CellFsStat* stat = (CellFsStat*)ps3->mem.getPtr(stat_ptr);
            stat->mode = CELL_FS_S_IRUSR | CELL_FS_S_IWUSR | CELL_FS_S_IXUSR |
                         CELL_FS_S_IRGRP | CELL_FS_S_IWGRP | CELL_FS_S_IXGRP |
                         CELL_FS_S_IROTH | CELL_FS_S_IWOTH | CELL_FS_S_IXOTH |
                         CELL_FS_S_IFREG;
            stat->uid = 0;
            stat->gid = 0;
            stat->atime = 0;
            stat->mtime = 0;
            stat->ctime = 0;
            stat->size = 0;
            stat->blksize = 4096;
            ps3->ppu->state.gprs[3] = Result::CELL_BADF;
        }
        else {
            ps3->ppu->state.gprs[3] = ps3->module_manager.cellFs.cellFsFstat();
        }
        break;
    }
    case 818:   ps3->ppu->state.gprs[3] = ps3->module_manager.cellFs.cellFsLseek();  break;
    case 872:   todo("sys_ss_get_open_psid()"); break;
    case 988:   ps3->ppu->state.gprs[3] = Result::CELL_OK;  break;

    default:
        Helpers::panic("Unimplemented syscall number 0x%02x (%d) @ 0x%016llx\n", syscall_num, syscall_num, ps3->ppu->state.pc);
    }
}