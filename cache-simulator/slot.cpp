#include<stdint.h>
#include<stdio.h>

#include "slot.hpp"

void Slot::setV(bool v)
{
  vFlag = v;
}

bool Slot::getV()
{
  return vFlag;
}

void Slot::setDirtyBit(bool dirty)
{
  dirtyBit = dirty;
}

bool Slot::isDirty()
{
  return dirtyBit;
}

void Slot::setTag(uint8_t t)
{
  tag = t;
}

uint8_t Slot::getTag()
{
  return tag;
}

void Slot::setBlock(block &v)
{
  // FIXME: Any Alternatives to copy data from one arr to another?
  for (int i = 0; i < BLOCK_SIZE; i++) {
    data[i] = v[i];
  }
}

uint8_t Slot::getValueFromBlock(uint8_t offset)
{
  return data[offset];
}

void Slot::display()
{
  printf("%-6d %-6X  ", vFlag, tag);

  for (int i = 0 ; i < BLOCK_SIZE; i++) {
    printf("%-3X", data[i]);
  }

  printf("\n");
}

void Slot::setBlockValue(uint8_t value, uint8_t offset) {
  data[offset] = value;
}
