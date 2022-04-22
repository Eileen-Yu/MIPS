#ifndef SLOT_H
#define SLOT_H

#define BLOCK_SIZE 16
typedef uint8_t block[BLOCK_SIZE];

class Slot
{
private:
  // FIXME: Dump in taking much memory, though dead simple
  bool vFlag = 0;
  bool dirtyBit = false;
  uint8_t tag = 0;
  block data = {0}; // FIXME: Is it a good pracice here to define an array?

public:
  void setV(bool v);

  bool getV();

  void setDirtyBit(bool dirty);

  bool isDirty();

  void setTag(uint8_t t);

  uint8_t getTag();

  void setBlock(block &v);

  uint8_t getValueFromBlock(uint8_t offset);

  void display();

  void setBlockValue(u_int8_t value, uint8_t offset);
};

#endif /*SLOT_H*/
