//
//  MachMemoryGrabber.h
//  Level6
//
//  Created by Christopher Thompson on 6/14/17.
//  Copyright © 2017 Uber. All rights reserved.
//

#ifndef MachMemoryGrabber_h
#define MachMemoryGrabber_h

#include <stdio.h>
#include <mach/mach.h>
#include <mach/vm_map.h>
#include <mach-o/loader.h>
#include "util.h"
#include "KnockItOff.h"

vm_address_t getBaseAddress(mach_port_t task);
vm_offset_t readMemory(mach_port_t task, vm_address_t address, size_t size);
void collectMachoChecksums(mach_port_t task, vm_address_t address);

#endif /* MachMemoryGrabber_h */
