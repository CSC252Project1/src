
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
        int32_t temp, data1, data2;
        uint32_t utemp, udata1, udata2;
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
                  uint32_t urs = CurrentInstruction & 0x3E00000; /*masks off rs*/
                  urs = urs >> 21;
                  udata1 = RegFile[urs];
                  printf("rs %i: %i\n",urs, udata1);
                  uint32_t urt = CurrentInstruction & 0xF8000; /*masks off rt*/
                  urt = urt >> 16;
                  udata2 = RegFile[urt];
                  printf("rt %i: %i\n",urt, udata2);
                  uint32_t urd = CurrentInstruction & 0xFC00; /*masks off rd*/
                  urd = urd >> 11;
                  utemp = udata1 + udata2;
                  printf("temp: %i\n",utemp);
                  RegFile[urd] = utemp;
                  printf("rd %i: %i\n",urd,RegFile[urd]);
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
                  urs = CurrentInstruction & 0x3E00000; /*masks off rs*/
                  urs = urs >> 21;
                  udata1 = RegFile[urs];
                  printf("rs %i: %i\n",urs, udata1);
                  urt = CurrentInstruction & 0xF8000; /*masks off rt*/
                  urt = urt >> 16;
                  udata2 = RegFile[urt];
                  printf("rt %i: %i\n",urt, udata2);
                  urd = CurrentInstruction & 0xFC00; /*masks off rd*/
                  urd = urd >> 11;
                  utemp = udata1 - udata2;
                  printf("temp: %i\n",utemp);
                  RegFile[urd] = utemp;
                  printf("rd %i: %i\n",urd,RegFile[urd]);
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
                  printf("product: %f and in hex %x\n",product,product);
              break;

              /*multu*/
              case 0x19:/***multu - Kingsley***/
                  printf("MULTU\n");
                  urs = CurrentInstruction & 0x3E00000; /*masks off rs*/
                  urs = urs >> 21;
                  udata1 = RegFile[urs];
                  printf("rs %i: %i\n",urs, udata1);
                  urt = CurrentInstruction & 0xF8000; /*masks off rt*/
                  urt = urt >> 16;
                  udata2 = RegFile[urt];
                  printf("rt %i: %i\n",urt, udata2);
                  uint64_t uproduct = rs*rt;
                  RegFile[33] = uproduct & 0xFFFFFFFF;
                  RegFile[32] = uproduct & 0xFFFFFFFF00000000;
                  printf("product: %f and in hex %x\n",uproduct,uproduct);
              break;

              case 0x1A:/***div - Jeter**/
                  printf("DIV\n");
                  int32_t rem;
                  int32_t quoitent;
                  rs = CurrentInstruction & 0x3E00000;
                  rs = rs >> 21;
                  printf("rs: %04x\n",rs);
                  rt = CurrentInstruction & 0xF8000;
                  rt = rt >> 16;
                  printf("rt: %04x\n",rt);
                  if(rt != 0){
                      quoitent= RegFile[rs] / RegFile[rt];
                      rem = RegFile[rs] % RegFile[rt];
                  }
                  else{
                      quoitent = NULL; /*cannot divide by 0*/
                      rem = 0;
                      printf("Cannot divide by 0\n");
                  }
                  printf("quoitent: %04x\n",quoitent);
                  printf("remainder: %04x\n",rem );
                  RegFile[34] = quoitent; /*Write to LO*/
                  RegFile[33] = rem; /*Write to HI*/
              break;

              case 0x1B:/***divu - Jeter**/
                  printf("DIVU\n");
                  uint32_t urem, uquoitent;
                  urs = CurrentInstruction & 0x3E00000;
                  urs = urs >> 21;
                  printf("rs: %04x\n",rs);
                  urt = CurrentInstruction & 0xF8000;
                  urt = urt >> 16;
                  printf("rt: %04x\n",rt);
                  if(urt != 0){
                      uquoitent= RegFile[urs] / RegFile[urt];
                      urem = RegFile[urs] % RegFile[urt];
                  }
                  else{
                      uquoitent = NULL; /*cannot divide by 0*/
                      printf("Cannot divide by 0\n");
                  }
                  printf("quoitent: %04x\n",uquoitent);
                  printf("remainder: %04x\n",urem);
                  RegFile[33] = uquoitent; /*Write to LO*/
                  RegFile[32] = urem; /*Write to HI*/
              break;

              /*mfhi*/
              case 0x10:/*mfhi - Kingsley*/
                  printf("MFHI\n");
                  rd = CurrentInstruction & 0xFC00; /*masks off rd*/
                  rd = rd >> 11;
                  RegFile[rd] = RegFile[33];
                  printf("rd %i: %i\n",rd, RegFile[rd]);
              break;

              /*mflo*/
              case 0x12:/*mflo - Kingsley*/
                  printf("MFLO\n");
                  rd = CurrentInstruction & 0xFC00; /*masks off rd*/
                  rd = rd >> 11;
                  RegFile[rd] = RegFile[32];
                  printf("rd %i: %i\n",rd, RegFile[rd]);
              break;

              /*mthi*/
              case 0x11: /*mthi - Jeter*/
                  printf("MTHI\n");
                  rs = CurrentInstruction & 0x3E00000;
                  rs = rs >> 21;
                  RegFile[33] = RegFile[rs];
                  printf("rs %i: %i\n",rs, RegFile[34]);
              break;

              /*mtlo*/
              case 0x13: /*mtlo - Jeter*/
                  printf("MTLO\n");
                  rs = CurrentInstruction & 0x3E00000;
                  rs = rs >> 21;
                  RegFile[32] = RegFile[rs];
                  printf("rs %i: %i\n",rs, RegFile[33]);
              break;

              /*and*/
              case 0x24:
                   printf("AND\n");
                   rs = CurrentInstruction & 0x3E00000;
                   rs = rs >> 21;
                   printf("rs %i: %x\n",rs, RegFile[rs]);
                   rt = CurrentInstruction & 0xF8000;
                   rt = rt >> 16;
                   printf("rt %i: %x\n",rt, RegFile[rt]);
                   rd = CurrentInstruction & 0xFC00;
                   rd = rd >> 11;
                   temp = RegFile[rs] & RegFile[rt];
                   printf("temp: %04x\n",temp);
                   RegFile[rd] = temp;
                   printf("rd %i: %x\n",rd, RegFile[rd]);
              break;

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
/*<<<<<<< Updated upstream

              /*sll / NOP */
              case 0x0: /*sll/nop - Jeter*/
                  printf("SLL\n");
                  urt = CurrentInstruction & 0xF8000; /*masks off rt*/
                  urt = urt >> 16;
                  udata1 = RegFile[urt];
                  printf("rt %i: %i\n",urt, udata2);
                  urd = CurrentInstruction & 0xFC00; /*masks off rd*/
                  urd = urd >> 11;
                  uint32_t usa = CurrentInstruction & 0x7C0; /*masks off sa*/
                  udata2 = RegFile[usa];
                  if(urt == 0 && urd == 0 && usa == 0){
                    printf("NOP\n");
                    break;
                  }
                  else{
                    utemp = udata1 << udata2;
                    printf("temp: %i\n",utemp);
                    RegFile[urd] = utemp;
                    printf("rd %i: %i\n",urd,RegFile[urd]);
                    break;
                  }

/*>>>>>>> Stashed changes
              /*sllv*/
              case 0x4: /*sllv - Jeter*/
                  printf("SLLV\n");
                  urs = CurrentInstruction & 0x3E00000; /*masks off rs*/
                  urs = urs >> 21;
                  udata1 = RegFile[urs];
                  printf("rs %i: %i\n",urs, udata1);
                  urt = CurrentInstruction & 0xF8000; /*masks off rt*/
                  urt = urt >> 16;
                  udata2 = RegFile[urt];
                  printf("rt %i: %i\n",urt, udata2);
                  urd = CurrentInstruction & 0xFC00; /*masks off rd*/
                  urd = urd >> 11;
                  utemp = udata2 << udata1;
                  printf("temp: %i\n",utemp);
                  RegFile[urd] = utemp;
                  printf("rd %i: %i\n",urd,RegFile[urd]);
              break;

              /*slt*/
              case 0x2A: /*slt - Jeter*/
                  printf("SLT\n");
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
                  if(RegFile[rs] < RegFile[rt])
                    temp = 1;
                  else
                    temp = 0;
                  printf("temp: %i\n",temp);
                  RegFile[rd] = temp;
                  printf("rd %i: %i\n",rd,RegFile[rd]);
              break;
              
              case 0x2B: /*sltu - Jeter*/
                  printf("SLTU\n");
                  urs = CurrentInstruction & 0x3E00000; /*masks off rs*/
                  urs = urs >> 21;
                  udata1 = RegFile[urs];
                  printf("rs %i: %i\n",urs, udata1);
                  urt = CurrentInstruction & 0xF8000; /*masks off rt*/
                  urt = urt >> 16;
                  udata2 = RegFile[urt];
                  printf("rt %i: %i\n",urt, udata2);
                  urd = CurrentInstruction & 0xFC00; /*masks off rd*/
                  urd = urd >> 11;
                  if(RegFile[urs] < RegFile[urt])
                    utemp = 1;
                  else
                    utemp = 0;
                  printf("temp: %i\n",utemp);
                  RegFile[urd] = utemp;
                  printf("rd %i: %i\n",urd,RegFile[urd]);
              break;

              case 0x3: /*sra - Jeter*/
                  printf("SRA\n");
                  rt = CurrentInstruction & 0xF8000; /*masks off rt*/
                  rt = rt >> 16;
                  data1 = RegFile[rt];
                  printf("rt %i: %i\n",rt, data2);
                  rd = CurrentInstruction & 0xFC00; /*masks off rd*/
                  rd = rd >> 11;
                  int32_t sa = CurrentInstruction & 0x7C0; /*masks off sa*/
                  data2 = RegFile[sa];
                  temp = data1 >> data2;
                  printf("temp: %i\n",temp);
                  RegFile[rd] = temp;
                  printf("rd %i: %i\n",rd,RegFile[rd]);
              break;

              case 0x7: /*srav - Jeter*/
                  printf("SLLV\n");
                  urs = CurrentInstruction & 0x3E00000; /*masks off rs*/
                  urs = urs >> 21;
                  udata1 = RegFile[urs];
                  printf("rs %i: %i\n",urs, udata1);
                  urt = CurrentInstruction & 0xF8000; /*masks off rt*/
                  urt = urt >> 16;
                  udata2 = RegFile[urt];
                  printf("rt %i: %i\n",urt, udata2);
                  urd = CurrentInstruction & 0xFC00; /*masks off rd*/
                  urd = urd >> 11;
                  utemp = udata2 >> udata1;
                  printf("temp: %i\n",utemp);
                  RegFile[urd] = utemp;
                  printf("rd %i: %i\n",urd,RegFile[urd]);
              break;

              /*srl*/
              case 0x2: /*srl - Jeter*/
                  printf("SRL\n");
                  urt = CurrentInstruction & 0xF8000; /*masks off rt*/
                  urt = urt >> 16;
                  udata1 = RegFile[urt];
                  printf("rt %i: %i\n",urt, udata2);
                  urd = CurrentInstruction & 0xFC00; /*masks off rd*/
                  urd = urd >> 11;
                  usa = CurrentInstruction & 0x7C0; /*masks off sa*/
                  udata2 = RegFile[usa];
                  utemp = udata1 >> udata2;
                  printf("temp: %i\n",utemp);
                  RegFile[urd] = utemp;
                  printf("rd %i: %i\n",urd,RegFile[urd]);
              break;

              /*srlv*/
              case 0x6: /*srlv - Jeter*/
                  printf("SRLV\n");
                  urs = CurrentInstruction & 0x3E00000; /*masks off rs*/
                  urs = urs >> 21;
                  udata1 = RegFile[urs];
                  printf("rs %i: %i\n",urs, udata1);
                  urt = CurrentInstruction & 0xF8000; /*masks off rt*/
                  urt = urt >> 16;
                  udata2 = RegFile[urt];
                  printf("rt %i: %i\n",rt, data2);
                  urd = CurrentInstruction & 0xFC00; /*masks off rd*/
                  urd = urd >> 11;
                  utemp = udata2 >> udata1;
                  printf("temp: %i\n",utemp);
                  RegFile[urd] = utemp;
                  printf("rd %i: %i\n",urd,RegFile[urd]);
              break;

              /*
<<<<<<< Updated upstream
=======
              *sltu
              *sra
              *srav
              /*srlv
>>>>>>> Stashed changes
              *jalr
              *jr
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
              uint32_t urs = CurrentInstruction & 0x3E00000; /*masks off rs*/
              urs = urs >> 21;
              printf("rs: %04x\n",urs);
              uint32_t urt = CurrentInstruction & 0xF8000; /*masks off rt*/
              urt = urt >> 16;
              printf("rt: %04x\n",urt);
              uint32_t uimmediate = CurrentInstruction & 0xFFFF;
              printf("immediate: %04x\n",uimmediate);
              utemp = RegFile[urs] + uimmediate;
              printf("temp: %04x\n",utemp);
              RegFile[urt] = utemp;
              printf("rt: %04x\n",RegFile[urt]);
          break;

          case 0xC:/*andi - Jeter*/
              printf("ANDI\n");
              rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
              rs = rs >> 21;
              printf("rs: %04x\n",rs);
              rt = CurrentInstruction & 0xF8000; /*masks off rt*/
              rt = rt >> 16;
              printf("rt: %04x\n",rt);
              immediate = CurrentInstruction & 0xFFFF;
              printf("immediate: %04x\n",immediate);
              temp = RegFile[rs] & immediate;
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

          case 0xA:/*slti - Jeter*/
              printf("SLTI\n");
              rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
              rs = rs >> 21;
              printf("rs: %04x\n",rs);
              rt = CurrentInstruction & 0xF8000; /*masks off rt*/
              rt = rt >> 16;
              printf("rt: %04x\n",rt);
              immediate = CurrentInstruction & 0xFFFF;
              printf("immediate: %04x\n",immediate);
              if(RegFile[rs] < immediate)
                    temp = 1;
              else
                    temp = 0;
              printf("temp: %04x\n",temp);
              RegFile[rt] = temp;
              printf("rt: %04x\n",RegFile[rt]);
          break;

          case 0xB:/*sltiu - Jeter*/
              printf("SLTIU\n");
              urs = CurrentInstruction & 0x3E00000; /*masks off rs*/
              urs = urs >> 21;
              printf("rs: %04x\n",urs);
              urt = CurrentInstruction & 0xF8000; /*masks off rt*/
              urt = urt >> 16;
              printf("rt: %04x\n",urt);
              uimmediate = CurrentInstruction & 0xFFFF;
              printf("immediate: %04x\n",uimmediate);
              if(RegFile[urs] < uimmediate)
                    utemp = 1;
              else
                    utemp = 0;
              printf("temp: %04x\n",utemp);
              RegFile[urt] = utemp;
              printf("rt: %04x\n",RegFile[urt]);
          break;

        /*TO DO:
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


