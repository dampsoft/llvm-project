// RUN: %check_clang_tidy %s dampsoft-header-stdafx %t -- -- -I %S/Inputs/dampsoft-header-stdafx

#include "stdafx.h"
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: #stdafx.h mustn't be included in headers! [dampsoft-header-stdafx]
#include <stdafx.h>
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: #stdafx.h mustn't be included in headers! [dampsoft-header-stdafx]
#include "otherHeader.h"
