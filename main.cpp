#include <cstdint>
#include <iostream>
#include <unordered_map>

#include "control-signal.hpp"
#include "exmem-reg.hpp"
#include "idex-reg.hpp"
#include "ifid-reg.hpp"
#include "inst-parser.hpp"
#include "memwb-reg.hpp"
#include "utils.hpp"

unsigned int Regs[32];

unsigned int InstructionCache[12] = {
    0xA1020000, 0x810AFFFC, 0x00831820, 0x01263820, 0x01224820, 0x81180000,
    0x81510010, 0x00624022, 0x00000000, 0x00000000, 0x00000000, 0x00000000};

std::unordered_map<unsigned int, unsigned int> ADDDRESS_INSTRUCT_MAP;

unsigned short int Main_Mem[1024];

unsigned int BEGIN_ADDRESS = 0x90000000;

IFIDReg IFID_REGISTER;

IDEXReg IDEX_REGISTER;

EXMEMReg EXMEM_REGISTER;

MEMWBReg MEMWB_REGISTER;

void IF_stage(unsigned int address) {
  // Build metadata for IF/ID register
  IFIDMeta m;
  m.address = address + 4;
  m.instruction = ADDDRESS_INSTRUCT_MAP[address];

  // Update IF/ID register writer version
  IFID_REGISTER.setWriteVersion(m);
}

void ID_stage() {
  // 1. Get IF/ID register read version
  IFIDMeta readReg = IFID_REGISTER.getReadVersion();

  // Nop
  if (readReg.instruction == 0x00000000) {
    IDEXMeta m = initIDEXMeta(); // Fill Nop for the following pipline register.
    IDEX_REGISTER.setWriteVersion(m);
    return;
  }

  // 2. Decode the instruction
  LoadType loadValue = dissemble(readReg.instruction, readReg.address);

  // 3. Update control signal
  unsigned int opcode = loadValue.op;

  // 4. Build metadata for ID/EX register
  //
  IDEXMeta m;
  m.PC = readReg.address;
  m.funcCode = loadValue.func;

  if (opcode == 0x20) {                   // LB
    m.readRegisterA = Regs[loadValue.rs]; // Pass the value of the read register
    m.readRegisterB = Regs[loadValue.rt];
    m.writeReg1Num = loadValue.rt;
    m.writeReg2Num = loadValue.rs;
    m.signExtOffset = loadValue.signExtImm;
    m.signal = getLwSignal();
  } else if (opcode == 0x28) {            // SB
    m.readRegisterA = Regs[loadValue.rs]; // Pass the value of the read register
    m.readRegisterB = Regs[loadValue.rt];
    m.writeReg1Num = loadValue.rt;
    m.writeReg2Num = loadValue.rs;
    m.signExtOffset = loadValue.signExtImm;
    m.signal = getSwSignal();
  } else if (opcode == 0) { // add or sub
    m.readRegisterA = Regs[loadValue.rs];
    m.readRegisterB = Regs[loadValue.rt];
    m.signExtOffset = INT_MIN; // actually for 'X'
    m.writeReg1Num = loadValue.rt;
    m.writeReg2Num = loadValue.rd;
    m.signal = getRTypeSignal();
  }

  IDEX_REGISTER.setWriteVersion(m);
}

void EX_stage() {
  IDEXMeta readReg = IDEX_REGISTER.getReadVersion();

  EXMEMMeta m = initEXMEMMeta();

  // Noop
  if (readReg.PC == 0x00000000) {
    EXMEM_REGISTER.setWriteVersion(m);
    return;
  }

  m.signal = readReg.signal;

  m.PC = readReg.PC;

  if (m.signal.RegDst == '0') { // Handle lw
    m.WriteRegNum = readReg.writeReg1Num;
    m.ALUResult = readReg.readRegisterA + readReg.signExtOffset;
    m.SWValue = readReg.readRegisterB;
  } else if (m.signal.RegDst == 'X') { // Handle sw
    m.WriteRegNum = 'X';
    m.ALUResult = readReg.readRegisterA + readReg.signExtOffset;
    m.SWValue = readReg.readRegisterB;
  } else if (m.signal.RegDst == '1') { // RType
    m.WriteRegNum = readReg.writeReg2Num;
    m.SWValue = readReg.readRegisterB;
    if (readReg.funcCode == 0x20) {
      m.ALUResult = readReg.readRegisterA + readReg.readRegisterB;
    } else if (readReg.funcCode == 0x22) {
      m.ALUResult = readReg.readRegisterA - readReg.readRegisterB;
    }
  }

  //  Write EX/MEM register
  EXMEM_REGISTER.setWriteVersion(m);
}

void MEM_stage() {
  EXMEMMeta readReg = EXMEM_REGISTER.getReadVersion();

  MEMWBMeta m = initMEMWBMeta();

  if (readReg.PC == 0x00000000) {
    MEMWB_REGISTER.setWriteVersion(m);
    return;
  }

  m.signal = readReg.signal;
  m.PC = readReg.PC;

  m.ALUResult = readReg.ALUResult;
  m.SWValue = readReg.SWValue;
  m.WriteRegNum = readReg.WriteRegNum;

  if (m.signal.MemRead == '1') { // Handle Lw
    m.LWDataValue = Main_Mem[m.ALUResult];
  } else if (m.signal.MemWrite == '1') { // Handle sw
    Main_Mem[m.ALUResult] = m.SWValue;
  }

  MEMWB_REGISTER.setWriteVersion(m);
}

void WB_stage() {
  MEMWBMeta readReg = MEMWB_REGISTER.getReadVersion();

  if (readReg.signal.RegWrite != '1') {
    return;
  }

  if (readReg.signal.MemToReg == '1') {
    // Lw
    Regs[readReg.WriteRegNum] = readReg.LWDataValue;
  } else if (readReg.signal.MemToReg == '0') {
    // add, sub ... (rtype)
    Regs[readReg.WriteRegNum] = readReg.ALUResult;
  }
}

void Print_out_everything(int cycle) {
  std::cout << "\n==================Cycle[" << cycle
            << "]==================\n\n";
  std::cout << "Regs Value: \n";
  for (int i = 0; i < 32; i++) {
    printf("Regs: %d, Value: %X\n", i, Regs[i]);
  }
  std::cout << "\n\n";
  std::cout << IFID_REGISTER << "\n\n"
            << IDEX_REGISTER << "\n\n"
            << EXMEM_REGISTER << "\n\n"
            << MEMWB_REGISTER << "\n\n";
}

void Copy_write_to_read() {
  IFID_REGISTER.copyFromWriteToRead();
  IDEX_REGISTER.copyFromWriteToRead();
  EXMEM_REGISTER.copyFromWriteToRead();
  MEMWB_REGISTER.copyFromWriteToRead();
}

void init() {
  // Initialize Registers
  Regs[0] = 0;
  for (int i = 1; i < 32; i++) {
    Regs[i] = 0x100 + i;
  }

  // Initialize Main memory
  for (int i = 0; i < 1024; i++) {
    Main_Mem[i] = i % 0x100;
  }

  // Initialize instruction map to address
  for (int i = 0; i < 12; i++) {
    unsigned int addr = BEGIN_ADDRESS + i * 4;
    ADDDRESS_INSTRUCT_MAP[addr] = InstructionCache[i];
  }
}

int main() {
  init();

  for (int i = 0; i < 12; i++) {
    unsigned int currentAddr = BEGIN_ADDRESS + 4 * i;
    // IF stage: Load instruction from address by PC
    IF_stage(currentAddr);

    // ID stage: Decode instruction, set values to ID/EX register
    ID_stage();

    // EX stage: Do calculation
    EX_stage();

    // MEM stage: Memory I/O
    MEM_stage();

    // WB stage: Write value back to register
    WB_stage();

    Print_out_everything(i);
    Copy_write_to_read();
  }
}
