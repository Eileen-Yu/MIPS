#ifndef CACHE_H
#define CACHE_H

#include "slot.hpp"

class Cache
{
private:
  Slot slots[16] = {Slot()};

  int getSlotIndex(unsigned int address);

  int getTag(unsigned int address);

  int getBlockOffset(unsigned int address);

  void loadValueFromMemory(unsigned int address, int slotIdx);

  bool checkHit(unsigned int address, int slotIdx);

  std::string confirmCache(unsigned int address, int slotIdx);

  void writeSlotDataToMemory(int slotIdx, Slot &targetSlot);

  void flushDirty(Slot &targetSlot, int slotIdx);

  unsigned int getSlotAddress(int slotIdx);

public:
  void readAddress(unsigned int address);

  void writeAddress(uint8_t value, unsigned int address);

  void display();
};

#endif /*CACHE_H*/
