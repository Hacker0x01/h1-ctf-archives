//
//  DecryptData.h
//  Level6
//
//  Created by Christopher Thompson on 6/20/17.
//  Copyright © 2017 Uber. All rights reserved.
//

#ifndef DecryptData_h
#define DecryptData_h

#include <stdio.h>
#include <mach/mach.h>
#include <mach/vm_map.h>
#include <mach/mach_error.h>

#include <mach-o/loader.h>
#include "MachMemoryGrabber.h"
#include "util.h"

#define EXIT_ON_MACH_ERROR(msg, retval) \
if (kret != KERN_SUCCESS) {mach_error(msg ":" , kret); exit((retval)); }

#define RETURN_ON_MACH_ERROR(msg, retval) \
if (kret != KERN_SUCCESS) {\
return 0;\
}

void decryptStrings();

#endif /* DecryptData_h */
