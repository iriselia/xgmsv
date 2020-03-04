#pragma once

// From Cod
#ifndef ENABLE
#define ENABLE 1
#endif

#ifndef DISABLED
#define DISABLED (-1)
#endif

#ifndef ENABLE_IF
#define ENABLE_IF(x) ((x) ? 1 : -1)
#endif

#ifndef ENABLED
#define ENABLED(x) (1 / (x) == 1)
#endif

#define DEBUG_PROGRAM ENABLE_IF(_DEBUG)

