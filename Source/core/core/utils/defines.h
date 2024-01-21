#pragma once

// From Cod
#if !defined(ENABLE) // qqtech also defines these and include order may cause redef errors
#define ENABLE &&
#define DISABLE &&!
#define ENABLE_IF(x) &&((x)?1:0)&&
#define ENABLED(x) (1 x 1)
#endif // #if !defined(ENABLE)

#define DEBUG_PROGRAM ENABLE_IF(_DEBUG)

