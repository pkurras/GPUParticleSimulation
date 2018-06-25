#pragma once

// EXTERNAL INCLUDES
#include <comdef.h>
#include <cstdio>
// INTERNAL INCLUDES
#include "math/vec2.h"

#define SAFE_DELETE(x) if (x) { delete x; x = nullptr; }
#define SAFE_RELEASE(x) if (x) { x->Release(); x = nullptr; }

#if defined(_DEBUG)
#define LOG(x, ...) { char string[128]; snprintf(string, 128, x, __VA_ARGS__); printf("[INFO]: [%s]\n", string); }
#else
#define LOG(x, ...)
#endif

#define WARN(x, ...) { char string[128]; snprintf(string, 128, x, __VA_ARGS__); printf("[WARNING]: [%s] (%s #%i)\n", string, __FILE__, __LINE__); }
#define ERR(x, ...) { char string[128]; snprintf(string, 128, x, __VA_ARGS__); printf("[ERROR]: [%s] (%s #%i)\n", string, __FILE__, __LINE__); }

#define V_RETURN(x) hr = x; if (hr != S_OK) { _com_error err(hr); LPCTSTR errMsg = err.ErrorMessage(); ERR("%s", errMsg); throw; }
