#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<stdio.h>

#include "cache.hpp"
#include "memory.hpp"

// Initialize main memory
unsigned int BYTE_MASK = 0x100;

void initMainMemory()
{
  for (int i = 0; i < 2048; i++) {
    MAIN_MEMORY[i] = i % BYTE_MASK;
  }
}

void interactive(Cache &cache) {
  char rslt = 'y';
  char op;

  while (rslt == 'y' || rslt == 'Y') {
    printf("(R)ead, (W)rite or (D)isplay Cache?\n");
    std::cin >> op;

    if(op == 'R' || op == 'r'){
      printf("What address would you like to read?\n");
      unsigned int readAddr;
      std::cin>>std::hex>>readAddr;

      cache.readAddress(readAddr);
    }

    else if(op == 'W' || op =='w'){
      printf("What address would you like to write to?\n");
      unsigned int writeAddr;
      std::cin>>std::hex>>writeAddr;


      printf("What data would you like to write at that address?\n");
      unsigned int data;
      std::cin>>std::hex>>data;

      cache.writeAddress(data,writeAddr);
      printf("Value %X has been written to address %X.\n", data, writeAddr);
    }

    else if(op == 'D' || op == 'd'){
      cache.display();
    }

    else{
      printf("Invalid input.\n");
    }

    printf("Continue?(y/n)\n");
    std::cin>>rslt;
  }
}

unsigned int getHexFromFileLine(std::fstream &file) {
  std::string tp, hexString;
  std::stringstream hexStream;
  unsigned int hexValue;

  std::getline(file, hexString);
  hexStream << hexString;
  hexStream >> std::hex >> hexValue;

  return hexValue;
}

void handleFileInput(std::string const& infile, Cache & cache) {
  std::fstream file;
  file.open(infile, std::ios::in);

  if (!file.is_open()) {
    std::cout<< "Failed to open file: " << infile << "\n";
    return;
  }

  std::string tp;
  while(std::getline(file, tp)) {
    if (tp.compare("R") == 0) {
      unsigned int readAddr = getHexFromFileLine(file);
      cache.readAddress(readAddr);

    } else if (tp.compare("W") == 0) {
      unsigned int writeAddr = getHexFromFileLine(file);
      unsigned int writeData = getHexFromFileLine(file);
      cache.writeAddress(writeData, writeAddr);

    } else if (tp.compare("D") == 0) {
      cache.display();
    } else {
      std::cout<< "File contains bad format:\n";
      std::cout<< tp << "\n";

      std::cout<< "Quit." << "\n";
      return;
    }
  }
  file.close();

  std::cout<< "All cmds from file input is executed.\n";
}

int main(){
  initMainMemory();

  Cache cache = Cache();

  std::cout<<"Interative(i) or File Input(f)?\n";
  char selection;
  std::cin>>selection;

  if (selection == 'i' || selection == 'I') {
    interactive(cache);
  }
  else if (selection == 'f' || selection == 'F') {
    std::string filename;
    std::cout<<"Please input file name:\n";
    std::cin>>filename;

    handleFileInput(filename, cache);
  }
  else {
    std::cout<< "Invalid input.\n";
  }
}
