#ifndef UTILS_H
#define UTILS_H

#include "control-signal.hpp"
#include "exmem-reg.hpp"
#include "idex-reg.hpp"
#include "memwb-reg.hpp"

static SignalMeta initControlSignal() {
  SignalMeta newSig;
  newSig.ALUOp = "00";
  newSig.ALUSrc = '0';
  newSig.Branch = '0';
  newSig.MemRead = '0';
  newSig.MemWrite = '0';
  newSig.RegDst = '0';
  newSig.MemToReg = '0';
  newSig.RegWrite = '0';
  return newSig;
}

static IDEXMeta initIDEXMeta() {
  IDEXMeta m;
  m.PC = 0x00000000;
  m.inst = 0x00000000;
  m.readRegisterA = 0x00000000;
  m.readRegisterB = 0x00000000;
  m.signExtOffset = 0;
  m.writeReg1Num = 0x00;
  m.writeReg2Num = 0x00;
  m.funcCode = 0x00000000;
  m.signal = initControlSignal();
  return m;
}

static EXMEMMeta initEXMEMMeta() {
  EXMEMMeta m;

  m.PC = 0x00000000;
  m.ALUResult = 0x00000000;
  m.SWValue = 0x00000000;
  m.WriteRegNum = 0x00000000;

  m.signal = initControlSignal();
  return m;
}

static MEMWBMeta initMEMWBMeta() {
  MEMWBMeta m;

  m.PC = 0x00000000;
  m.ALUResult = 0x00000000;
  m.SWValue = 0x00000000;
  m.WriteRegNum = 0x00000000;

  m.LWDataValue = 0x00000000;

  m.signal = initControlSignal();
  return m;
}

static SignalMeta getLwSignal() {
  SignalMeta lwSignal;

  lwSignal.ALUOp = "00";
  lwSignal.ALUSrc = '1';
  lwSignal.Branch = '0';
  lwSignal.MemRead = '1';
  lwSignal.MemWrite = '0';
  lwSignal.RegDst = '0';
  lwSignal.MemToReg = '1';
  lwSignal.RegWrite = '1';

  return lwSignal;
}

static SignalMeta getSwSignal() {
  SignalMeta swSignal;

  swSignal.ALUOp = "00";
  swSignal.ALUSrc = '1';
  swSignal.Branch = '0';
  swSignal.MemRead = '0';
  swSignal.MemWrite = '1';
  swSignal.RegDst = 'X';
  swSignal.MemToReg = 'X';
  swSignal.RegWrite = '0';

  return swSignal;
}

static SignalMeta getRTypeSignal() {
  SignalMeta signal;

  signal.ALUOp = "10";
  signal.ALUSrc = '0';
  signal.Branch = '0';
  signal.MemRead = '0';
  signal.MemWrite = '0';
  signal.RegDst = '1';
  signal.MemToReg = '0';
  signal.RegWrite = '1';

  return signal;
}

#endif /*UTILS_H*/
