
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "RegFile.h"
#include "Syscall.h"
#include "utils/heap.h"
#include "elf_reader/elf_reader.h"

//Stats

uint32_t DynInstCount = 0;

void write_initialization_vector(uint32_t sp, uint32_t gp, uint32_t start) {
        printf("\n ----- BOOT Sequence ----- \n");
        printf("Initializing sp=0x%08x; gp=0x%08x; start=0x%08x\n", sp, gp, start);
        RegFile[28] = gp;
        RegFile[29] = sp;
        RegFile[31] = start;
        printRegFile();

    }


int main(int argc, char * argv[]) {
  
    int MaxInst = 0;
    int status = 0;
    uint32_t i; 
    uint32_t PC,newPC; //This is the starting address of the program in the memory
    uint32_t CurrentInstruction;

    if (argc < 2) {
      printf("Input argument missing \n");
      return -1;
    }
    sscanf (argv[2], "%d", &MaxInst);

    //Open file pointers & initialize Heap & Regsiters
    initHeap();
    initFDT();
    initRegFile(0);
    
    //Load required code portions into Emulator Memory
    status =  LoadOSMemory(argv[1]);
    if(status <0) { return status; }
    
    //set Global & Stack Pointers for the Emulator
    // & provide startAddress of Program in Memory to Processor
    write_initialization_vector(exec.GSP, exec.GP, exec.GPC_START);

    printf("\n ----- Execute Program ----- \n");
    printf("Max Instruction to run = %d \n",MaxInst);
    PC = exec.GPC_START;
    for(i=0; i<MaxInst ; i++) {
        DynInstCount++;
        CurrentInstruction = readWord(PC,false);
        /*CurrentInstruction = 0x410820;*/ 
        printf("CURRENT INSTRUCTION: %04x\n",CurrentInstruction); 
    /********************************/
    //Add your implementation here
    /********************************/
        /******arith*******/
        int temp;
        int32_t data1, data2;
        int funct = CurrentInstruction & 0x3F; /*masks off funct*/
        printf("funct: %x\n",funct);
        int op = CurrentInstruction & 0xFC000000; /*masks off funct*/
        op = op >> 26;
        printf("op: %x\n",op);

        switch(op){

          case 0:
            switch(funct){
              case 0x20:/***add- Kingsley***/
                  printf("ADD\n");
                  int32_t rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
                  rs = rs >> 21;
                  data1 = RegFile[rs];
                  printf("rs %i: %i\n",rs, data1);
                  int32_t rt = CurrentInstruction & 0xF8000; /*masks off rt*/
                  rt = rt >> 16;
                  data2 = RegFile[rt];
                  printf("rt %i: %i\n",rt, data2);
                  int32_t rd = CurrentInstruction & 0xFC00; /*masks off rd*/
                  rd = rd >> 11;
                  temp = data1 + data2;
                  printf("temp: %i\n",temp);
                  RegFile[rd] = temp;
                  printf("rd %i: %i\n",rd,RegFile[rd]);
              break;

              case 0x21:/***addu - Kingsley***/
                  printf("ADDU\n");
                  rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
                  rs = rs >> 21;
                  data1 = RegFile[rs];
                  printf("rs %i: %i\n",rs, data1);
                  rt = CurrentInstruction & 0xF8000; /*masks off rt*/
                  rt = rt >> 16;
                  data2 = RegFile[rt];
                  printf("rt %i: %i\n",rt, data2);
                  rd = CurrentInstruction & 0xFC00; /*masks off rd*/
                  rd = rd >> 11;
                  temp = data1 + data2;
                  printf("temp: %i\n",temp);
                  RegFile[rd] = temp;
                  printf("rd %i: %i\n",rd,RegFile[rd]);
              break;

              case 0x22:/***sub- Jeter***/
                  printf("SUB\n");
                  rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
                  rs = rs >> 21;
                  data1 = RegFile[rs];
                  printf("rs %i: %i\n",rs, data1);
                  rt = CurrentInstruction & 0xF8000; /*masks off rt*/
                  rt = rt >> 16;
                  data2 = RegFile[rt];
                  printf("rt %i: %i\n",rt, data2);
                  rd = CurrentInstruction & 0xFC00; /*masks off rd*/
                  rd = rd >> 11;
                  temp = data1 - data2;
                  printf("temp: %i\n",temp);
                  RegFile[rd] = temp;
                  printf("rd %i: %i\n",rd,RegFile[rd]);
              break;

              case 0x23:/***subu - Jeter***/
                  printf("SUBU\n");
                  rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
                  rs = rs >> 21;
                  data1 = RegFile[rs];
                  printf("rs %i: %i\n",rs, data1);
                  rt = CurrentInstruction & 0xF8000; /*masks off rt*/
                  rt = rt >> 16;
                  data2 = RegFile[rt];
                  printf("rt %i: %i\n",rt, data2);
                  rd = CurrentInstruction & 0xFC00; /*masks off rd*/
                  rd = rd >> 11;
                  temp = data1 - data2;
                  printf("temp: %i\n",temp);
                  RegFile[rd] = temp;
                  printf("rd %i: %i\n",rd,RegFile[rd]);
              break;
              
              /*mult*/
              case 0x18:/***mult - Kingsley***/
                  printf("MULT\n");
                  rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
                  rs = rs >> 21;
                  data1 = RegFile[rs];
                  printf("rs %i: %i\n",rs, data1);
                  rt = CurrentInstruction & 0xF8000; /*masks off rt*/
                  rt = rt >> 16;
                  data2 = RegFile[rt];
                  printf("rt %i: %i\n",rt, data2);
                  int64_t product = rs*rt;
                  RegFile[33] = product & 0xFFFFFFFF;
                  RegFile[32] = product & 0xFFFFFFFF00000000;
                  printf("product: %i and in hex %x\n",product,product);
              break;
              /*multu*/
              /*mfhi*/
              /*mflo*/
              case 0x12:/*mflo - Kingsley*/
                  printf("MFLO\n");
                  rd = CurrentInstruction & 0xFC00; /*masks off rd*/
                  rd = rd >> 11;
                  RegFile[rd] = RegFile[33];
                  printf("rd %i: %i\n",rd, RegFile[rd]);
              break;
              /*mthi*/
              /*mtlo*/
              /*and*/
              /*xor*/
              case 0x26:
                   printf("XOR\n");
                   rs = CurrentInstruction & 0x3E00000;
                   rs = rs >> 21;
                   printf("rs %i: %x\n",rs, RegFile[rs]);
                   rt = CurrentInstruction & 0xF8000;
                   rt = rt >> 16;
                   printf("rt %i: %x\n",rt, RegFile[rt]);
                   rd = CurrentInstruction & 0xFC00;
                   rd = rd >> 11;
                   temp = RegFile[rs] ^ RegFile[rt];
                   printf("temp: %04x\n",temp);
                   RegFile[rd] = temp;
                   printf("rd %i: %x\n",rd, RegFile[rd]);
              break;
              /*nor*/
              case 0x27:
                   printf("NOR\n");
                   rs = CurrentInstruction & 0x3E00000;
                   rs = rs >> 21;
                   printf("rs %i: %i\n",rs, RegFile[rs]);
                   rt = CurrentInstruction & 0xF8000;
                   rt = rt >> 16;
                   printf("rt %i: %i\n",rt, RegFile[rt]);
                   rd = CurrentInstruction & 0xFC00;
                   rd = rd >> 11;
                   printf("rs %i: %i\n",rd, RegFile[rd]);
                   temp = RegFile[rs] | RegFile[rt];
                   temp = ~temp;
                   printf("temp: %04x\n",temp);
                   RegFile[rd] = temp;
              break;
              /*or*/
              case 0x25:
                   printf("OR\n");
                   rs = CurrentInstruction & 0x3E00000;
                   rs = rs >> 21;
                   printf("rs %i: %i\n",rs, RegFile[rs]);
                   rt = CurrentInstruction & 0xF8000;
                   rt = rt >> 16;
                   printf("rt %i: %i\n",rt, RegFile[rt]);
                   rd = CurrentInstruction & 0xFC00;
                   rd = rd >> 11;
                   printf("rs %i: %i\n",rd, RegFile[rd]);
                   temp = RegFile[rs] | RegFile[rt];
                   printf("temp: %04x\n",temp);
                   RegFile[rd] = temp;
              break;
              /*sll*/
              /*sllv*/
              /*slt*/
              /*
              *sltu
              *sra
              *srav
              *srl
              *srlv
              *jalr
              *jr
              *NOP
              */
            }
          break; /*op 0*/

          case 0x8:/*addi Kingsley*/
              printf("ADDI\n");
              int32_t rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
              rs = rs >> 21;
              printf("rs: %04x\n",rs);
              int32_t rt = CurrentInstruction & 0xF8000; /*masks off rt*/
              rt = rt >> 16;
              printf("rt: %04x\n",rt);
              int32_t immediate = CurrentInstruction & 0xFFFF;
              printf("immediate: %04x\n",immediate);
              temp = RegFile[rs] + immediate;
              printf("temp: %04x\n",temp);
              RegFile[rt] = temp;
              printf("rt: %04x\n",RegFile[rt]);
          break;

          case 0x9:/*addiu - Kingsley*/
              printf("ADDIU\n");
              rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
              rs = rs >> 21;
              printf("rs: %04x\n",rs);
              rt = CurrentInstruction & 0xF8000; /*masks off rt*/
              rt = rt >> 16;
              printf("rt: %04x\n",rt);
              immediate = CurrentInstruction & 0xFFFF;
              printf("immediate: %04x\n",immediate);
              temp = RegFile[rs] + immediate;
              printf("temp: %04x\n",temp);
              RegFile[rt] = temp;
              printf("rt: %04x\n",RegFile[rt]);
          break;

          case 0xD:/*ori - Kingsley*/
              printf("ORI\n");
              rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
              rs = rs >> 21;
              printf("rs: %04x\n",rs);
              rt = CurrentInstruction & 0xF8000; /*masks off rt*/
              rt = rt >> 16;
              printf("rt: %04x\n",rt);
              immediate = CurrentInstruction & 0xFFFF;
              printf("immediate: %04x\n",immediate);
              temp = RegFile[rs] | immediate;
              printf("temp: %04x\n",temp);
              RegFile[rt] = temp;
              printf("rt: %04x\n",RegFile[rt]);
          break;

          case 0xE:/*xori - Kingsley*/
              printf("XORI\n");
              rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
              rs = rs >> 21;
              printf("rs: %04x\n",rs);
              rt = CurrentInstruction & 0xF8000; /*masks off rt*/
              rt = rt >> 16;
              printf("rt: %04x\n",rt);
              immediate = CurrentInstruction & 0xFFFF;
              printf("immediate: %04x\n",immediate);
              temp = RegFile[rs] ^ immediate;
              printf("temp: %04x\n",temp);
              RegFile[rt] = temp;
              printf("rt: %04x\n",RegFile[rt]);
          break;

        /*TO DO:
          * andi
          * xori
          * slti
          * sltiu
          beq
          beql
          bgez
          bgezal
          bgtz
          blez
          blezl
          bltz
          bltzal
          bne
          bnel
          j
          jal
          LB
          LBU
          LH
          LHU
          LUI
          LW
          LWL
          LWR
          SB
          SH
          SW
          SWL
          SWR
          */
      }
      /*syscall*/
    printRegFile();
    PC = PC + 4;

    }
}


