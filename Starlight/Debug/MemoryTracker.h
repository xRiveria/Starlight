#pragma once

// Traces all user new and delete calls for memory tracing. We will store the line and file where new/delete are called.
// Note that this is meant to track user calls only. 

#ifdef AURORA_DEBUG

#include <cstddef>

// Usurp the "new" operator (scalar and array versions).
void* operator new(std::size_t, const char*, long);
void* operator new[](std::size_t, const char*, long);
#define new new(__FILE__, __LINE__)

extern bool g_TraceFlag;
#define TRACE_ON()  g_TraceFlag = true
#define TRACE_OFF() g_TraceFlag = false

extern bool g_ActiveFlag;
#define MEMORY_ON()  g_ActiveFlag = true
#define MEMORY_OFF() g_ActiveFlag = false

#else


#endif