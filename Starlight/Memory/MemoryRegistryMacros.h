#pragma once
#include "MemoryRegistry.h"

#define REGISTER_MEMORY_BLOCK(type, size) Memory::MemoryPoolRegistry::GetInstance().AllocateMemoryBlock(type, size);