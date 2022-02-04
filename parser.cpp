#include<iostream>
#include<map>
#include<string>
#include<cstdio>

unsigned int INPUT_CODE [] = {
  0x032BA020,
  0x8CE90014,
  0x12A90003,
  0x022DA822,
  0xADB30020,
  0x02697824,
  0xAE8FFFF4,
  0x018C6020,
  0x02A4A825,
  0x158FFFF7,
  0x8ECDFFF0
};

std::map<int, std::string> rtypeOps {
  { 0x20, "add" },
  { 0x22, "sub" },
  { 0x24, "and" },
  { 0x25, "or" },
  { 0x2A, "slt" },
};

std::map<int,std::string> itypeOps {
  {0x23, "lw"},
  {0x2B, "sw"},
  {0x4, "beq"},
  {0x5, "bne"}
};

unsigned int getOpcode(unsigned int code) {
  return (code & 0xFC000000) >> 26;
}

// transfer RType funct code to name
std::string getRTypeOp(unsigned int funcValue) {
  return rtypeOps.at(funcValue);
}

// transfer IType opcode to name
std::string getITypeOp(unsigned int code) {
  unsigned int opCode = getOpcode(code);
  return itypeOps.at(opCode);
}

unsigned int parseRD(unsigned int code) {
  return (code & 0x0000F800) >> 11;
}

unsigned int parseRS(unsigned int code) {
  return (code & 0x03E00000) >> 21;
}

unsigned int parseRT(unsigned int code) {
  return (code & 0x001F0000) >> 16;
}

unsigned int getFuncValue(unsigned int code) {
  return code & 0x0000003F;
}

short getSignExtImm(unsigned int code) {
  short signExtImm = code & 0x0000FFFF;
  return signExtImm;
}

void decodeRType(unsigned int code) {
  int funcValue = getFuncValue(code);
  std::string op = getRTypeOp(funcValue);

  unsigned int rd = parseRD(code);
  unsigned int rs = parseRS(code);
  unsigned int rt = parseRT(code);

  printf("%s $%d, $%d, $%d\n",op.c_str(),rd,rs,rt);
}

unsigned int calculateBTA(unsigned int instructionAddr, short signExtImm){
  int distance = (signExtImm << 2) ;
  unsigned int newAddr = instructionAddr + 4 + distance;
  return newAddr;
}

void decodeIType(unsigned int code, unsigned int addr) {
  std::string op = getITypeOp(code); 
  
  unsigned int rt = parseRT(code);
  unsigned int rs = parseRS(code);
  short signExtImm  = getSignExtImm(code);

  if(op.compare("bne") == 0 || op.compare("beq") == 0){
    unsigned int BTA = calculateBTA(addr,signExtImm);
    printf("%s $%d, $%d address %X \n",op.c_str(),rs,rt,BTA);
  } else {
    printf("%s $%d, %d($%d)\n",op.c_str(),rt,signExtImm,rs);
  }
}

void disassemble(unsigned int code,unsigned int addr) {
  unsigned int opcode = getOpcode(code);

  switch(opcode) {
    case 0:
      decodeRType(code);
      break;
    case 4:
    case 5:
    case 0x23:
    case 0x2B:
      decodeIType(code,addr);
      break;
    default:
      std::cout<< "Invalid Code\n";
      break;
  }
}

int main() {
  unsigned int addr = 0x9A040;
  for (const auto& code: INPUT_CODE) {
    printf("%X  ",addr);
    disassemble(code,addr);
    addr += 4;
  }
}
