#include "arena.h"

u64 ArenaPush(Arena* arena, u64 size)
{
  if (arena->ptr + size > arena->maxSize)
  {
    return 0;
  }
  u64 out = arena->memory + arena->ptr;
  arena->ptr += size;
  return out;
}
void ArenaPop(Arena* arena, u64 size)
{
  arena->ptr -= size;
}
