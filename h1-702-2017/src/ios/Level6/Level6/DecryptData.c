//
//  DecryptData.c
//  Level6
//
//  Created by Christopher Thompson on 6/20/17.
//  Copyright © 2017 Uber. All rights reserved.
//

#include "DecryptData.h"

// The xor key to decrypt the strings is "lookatmeimmmakey123"
size_t xor_key_length = 19;
uint8_t xor_key[] = {0x6c, 0x6f, 0x6f, 0x6b, 0x61, 0x74, 0x6d, 0x65, 0x69, 0x6d, 0x6d, 0x6d, 0x61, 0x6b, 0x65, 0x79, 0x31, 0x32, 0x33};

kern_return_t change_page_protection(mach_port_t task, vm_address_t patch_addr, vm_prot_t new_protection) {
    kern_return_t kret;
    kret = vm_protect(task, patch_addr, (mach_vm_size_t)4096, FALSE, new_protection);
    RETURN_ON_MACH_ERROR("change_page_protection\n", kret);
    return 1;
}

#include "stdlib.h"

void doShit(vm_address_t address) {
    vm_size_t vmsize;
    vm_region_basic_info_64_t info;
    mach_msg_type_number_t info_count = VM_REGION_BASIC_INFO_COUNT;
    memory_object_name_t object;
    
    void* ptr = malloc(100);
    
    vm_address_t addr = (vm_address_t)ptr;
    
    kern_return_t status =   vm_region_64(mach_task_self(), &addr, &vmsize, VM_REGION_BASIC_INFO,
                                       (vm_region_info_64_t)&info, &info_count, &object);
    
    if (status) {
        perror("vm_region");
    }
    
    printf("Memory protection: %c%c%c  %c%c%c\n",
           info->protection & VM_PROT_READ ? 'r' : '-',
           info->protection & VM_PROT_WRITE ? 'w' : '-',
           info->protection & VM_PROT_EXECUTE ? 'x' : '-',
           
           info->max_protection & VM_PROT_READ ? 'r' : '-',
           info->max_protection & VM_PROT_WRITE ? 'w' : '-',
           info->max_protection & VM_PROT_EXECUTE ? 'x' : '-'
           );
}

#include <mach/mach.h>
#include <libkern/OSCacheControl.h>
#include <stdbool.h>

#define kerncall(x) ({ \
kern_return_t _kr = (x); \
if(_kr != KERN_SUCCESS) \
fprintf(stderr, "%s failed with error code: 0x%x\n", #x, _kr); \
_kr; \
})


void
__attribute__((always_inline))
decryptStrings()
{
    size_t i, j, k;
    unsigned char *cmdAddr = NULL, *secAddr;
    unsigned char *cmds = 0;
    struct load_command* loadCommand    = NULL;
    struct segment_command *segCmd = NULL;
    struct segment_command_64 *seg64Cmd = NULL;
    struct section *sectionCmd = NULL;
    struct section_64 *section64Cmd = NULL;
    uint8_t *stringSection;
    mach_port_t task;
    vm_address_t lcOffset = 0;
    
    task = current_task();
    vm_address_t address = getBaseAddress(task);
    
    struct mach_header *head = (struct mach_header *)readMemory(task, address, sizeof(struct mach_header));
    if (head->magic != MH_MAGIC_64 && head->magic != MH_MAGIC) {
        return;
    }
    
    lcOffset = (head->magic == MH_MAGIC) ? sizeof(struct mach_header) : sizeof(struct mach_header_64);
    cmds = (unsigned char *)readMemory(task, address + lcOffset, head->sizeofcmds);
    
    cmdAddr = cmds;
    for (i = 0; i < head->ncmds; i++, cmdAddr += loadCommand->cmdsize){
        loadCommand = (struct load_command *)cmdAddr;
        if (loadCommand->cmd == LC_SEGMENT) {
            segCmd = (struct segment_command *)cmdAddr;
            
            if (memcmp(segCmd->segname, textSection, 16) == 0 ||
                memcmp(segCmd->segname, dataSection, 16) == 0) {
                secAddr = cmdAddr + sizeof(struct segment_command);
                for (j = 0; j < segCmd->nsects; j++){
                    sectionCmd = (struct section *)secAddr;
                    
                    if (memcmp(sectionCmd->sectname, cfstring, 16) == 0 ||
                        memcmp(sectionCmd->sectname, cstring, 16) == 0) {
                        stringSection = (uint8_t *)(sectionCmd->addr - seg64Cmd->vmaddr + address);
                        
                        if (kerncall(vm_protect(mach_task_self(), (vm_address_t)stringSection, section64Cmd->size, false, VM_PROT_READ | VM_PROT_WRITE | VM_PROT_COPY))) return;
                        
                        for (k = 0; k < section64Cmd->size; k++) {
                            stringSection[k] ^= xor_key[k % xor_key_length];
                        }
                        
                        //if (kerncall(vm_protect(mach_task_self(), (vm_address_t)stringSection, section64Cmd->size, false, VM_PROT_READ | VM_PROT_COPY))) return;
                    }
                    secAddr += sizeof(struct section);
                }
            }
        }
        if (loadCommand->cmd == LC_SEGMENT_64) {
            seg64Cmd = (struct segment_command_64 *)cmdAddr;
            
            if (memcmp(seg64Cmd->segname, textSection, 16) == 0 ||
                memcmp(seg64Cmd->segname, dataSection, 16) == 0) {
                secAddr = cmdAddr + sizeof(struct segment_command_64);
                for (j = 0; j < seg64Cmd->nsects; j++){
                    section64Cmd = (struct section_64 *)secAddr;
                    
                    if (memcmp(section64Cmd->sectname, cfstring, 16) == 0 ||
                        memcmp(section64Cmd->sectname, cstring, 16) == 0) {
                        
                        stringSection = (uint8_t *)(section64Cmd->addr - seg64Cmd->vmaddr + address);
                        doShit((vm_address_t)stringSection & 0xffffffffffff0000);
                        
                        if (kerncall(vm_protect(mach_task_self(), (vm_address_t)stringSection & 0xffffffffffff0000, 4096 * 2, false, VM_PROT_READ | VM_PROT_WRITE))) return;
                        
                        doShit((vm_address_t)stringSection & 0xffffffffffff0000);
                        for (k = 0; k < section64Cmd->size; k++) {
                            stringSection[k] ^= xor_key[k % xor_key_length];
                        }
                        
                        //if (kerncall(vm_protect(mach_task_self(), (vm_address_t)stringSection, section64Cmd->size, false, VM_PROT_READ))) return;
                    }
                    secAddr += sizeof(struct section_64);
                }
            }
        }
    }
}
