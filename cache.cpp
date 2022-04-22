#include "stdint.h"
#include "stdio.h"
#include <string>

#include "cache.hpp"
#include "memory.hpp"

int Cache::getSlotIndex(unsigned int address) {
  return (address & 0x000000F0) >> 4;
}

int Cache::getTag(unsigned int address) {
  return (address & 0x00000700) >> 8;
}

int Cache::getBlockOffset(unsigned int address) {
  return (address & 0x0F);
}

unsigned int Cache::getSlotAddress(int slotIdx) {
  uint8_t tagBit = slots[slotIdx].getTag();
  return (((tagBit << 4) + slotIdx) << 4);
}

bool Cache::checkHit(unsigned int address, int slotIdx){
  // If slot index out of range
  if (slotIdx < 0 || slotIdx >15) {
    return false;
  }

  // If v flag is false
  Slot &targetSlot = slots[slotIdx]; 
  if(targetSlot.getV() == false){
    return false;
  }

  // If tag is not matched
  u_int8_t tag = getTag(address);
  if(tag != targetSlot.getTag()){
    return  false;
  }

  return true;
}

std::string Cache::confirmCache(unsigned int address, int slotIdx) {
  bool hit = checkHit(address, slotIdx);

  if (hit == true) {
    return "Cache Hit";
  }

  loadValueFromMemory(address, slotIdx);
  return "Cache Miss";
}

void Cache::writeSlotDataToMemory(int slotIdx, Slot &targetSlot) {
  // Calculate main memory address from slot
  unsigned int slotAddress = getSlotAddress(slotIdx);
  for (int i = 0; i < BLOCK_SIZE; i++) {
    MAIN_MEMORY[slotAddress + i] = targetSlot.getValueFromBlock(i);
  }
}

void Cache::flushDirty(Slot &targetSlot, int slotIdx) {
  if (targetSlot.isDirty() == false) return;

  // Sync memory data with cache if dirty.
  writeSlotDataToMemory(slotIdx, targetSlot);

  targetSlot.setDirtyBit(false); // Data on both side are synced.
}

void Cache::loadValueFromMemory(unsigned int address, int slotIdx) {
  // First target slot
  Slot &targetSlot = slots[slotIdx];

  // Then, if slot dirty flag is true, update its mapped memory.
  flushDirty(targetSlot, slotIdx);

  // Once dirty bit removed (memory updated),
  // write data from current memory address to target slot. 

  // Get begin address
  unsigned int block_begin_addr = (address & 0xFFFFFFF0);

  // Load data from the given address to slot.
  for (int i = 0; i < BLOCK_SIZE; i++) {
    targetSlot.setBlockValue(MAIN_MEMORY[block_begin_addr + i], i);
  }

  // And update tag, v flag, dirty bit flag.
  uint8_t tag = getTag(address);
  targetSlot.setTag(tag);

  targetSlot.setV(true);

  // Data from cache are sync with mapped memory.
  targetSlot.setDirtyBit(false);
}

void Cache::readAddress(unsigned int address)
{
  int slotIndex = getSlotIndex(address); 

  std::string hitResult = confirmCache(address, slotIndex);

  uint8_t blockOffset = getBlockOffset(address);

  uint8_t value =  slots[slotIndex].getValueFromBlock(blockOffset);

  printf("Get [%X] value: %X. (%s)\n", address, value, hitResult.c_str());
}

void Cache::writeAddress(u_int8_t value, unsigned int address){
  // First get mapped slot
  int slotIdx = getSlotIndex(address);

  // Check if write-hit
  bool hit = checkHit(address, slotIdx);

  Slot &targetSlot = slots[slotIdx];

  // If hit, just write;
  // (Caution: no need to paste data from slot to memory even if dirty flag true when hit)
  if (hit) {
    printf("(Cache hit)");
  }
  else  {
    printf("(Cache miss)");
    // If dirty flag, write data from Slot to memory
    if (targetSlot.isDirty() == true) {
      writeSlotDataToMemory(slotIdx, targetSlot);
    }
    // If not dirty, load data from main memory to cache;
    else {
      loadValueFromMemory(address,slotIdx);
    }
  }

  // Write value to cache
  uint8_t offset = getBlockOffset(address);
  targetSlot.setBlockValue(value, offset);
  targetSlot.setDirtyBit(true);

  // Update Slot info
  uint8_t tag = getTag(address);
  targetSlot.setTag(tag);
  targetSlot.setV(true);
  printf("Write %X to address %X\n", value, address);
}

void Cache::display() {
  printf("%-6s %-6s %-6s  %-48s\n", "Slot", "Valid", "Tag", "Data");
  for (int i = 0; i < 16; i++) {
    printf("%-7X", i);
    slots[i].display();
  }
}
