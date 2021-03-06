
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
    int delaySlot = 0;
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
        if(newPC != 0){
          delaySlot++;
          if(delaySlot == 2){
            printf("PC: %x (JUMPED)\n", newPC);
            PC = newPC;
            CurrentInstruction = readWord(PC,false);
            newPC = 0;
            delaySlot = 0;
          }else{
            printf("PC: %x (DELAY SLOT)\n", PC);
            CurrentInstruction = readWord(PC,false);
          }
        }else{
            printf("PC: %x\n", PC);
            CurrentInstruction = readWord(PC,false);
        }       
        /*CurrentInstruction = 0x410820;*/ 
        printf("CURRENT INSTRUCTION: %04x\n",CurrentInstruction); 
    /********************************/
    //Add your implementation here
    /********************************/
        /******arith*******/
        int32_t temp, data1, data2;
        uint32_t utemp, udata1, udata2;
        uint32_t funct = CurrentInstruction & 0x3F; /*masks off funct*/
        uint32_t cond = CurrentInstruction & 0x3E0000; /*masks off condition for relative conditional branch*/
	cond = cond >> 16;
        printf("funct: %x\n",funct);
        uint32_t op = CurrentInstruction & 0xFC000000; /*masks off funct*/
        op = op >> 26;
        /*if((0x80000000 & op) == 0x80000000){  positifies
          /*op = op & 0x3F;
        }*/
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
                  int32_t rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
                  rt = rt >> 16;
                  data2 = RegFile[rt];
                  printf("rt %i: %i\n",rt, data2);
                  int32_t rd = CurrentInstruction & 0xF800; /*masks off rd*/
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
                  uint32_t urt = CurrentInstruction & 0x1F0000; /*masks off rt*/
                  urt = urt >> 16;
                  udata2 = RegFile[urt];
                  printf("rt %i: %i\n",urt, udata2);
                  uint32_t urd = CurrentInstruction & 0xF800; /*masks off rd*/
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
                  rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
                  rt = rt >> 16;
                  data2 = RegFile[rt];
                  printf("rt %i: %i\n",rt, data2);
                  rd = CurrentInstruction & 0xF800; /*masks off rd*/
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
                  urt = CurrentInstruction & 0x1F0000; /*masks off rt*/
                  urt = urt >> 16;
                  udata2 = RegFile[urt];
                  printf("rt %i: %i\n",urt, udata2);
                  urd = CurrentInstruction & 0xF800; /*masks off rd*/
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
                  rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
                  rt = rt >> 16;
                  data2 = RegFile[rt];
                  printf("rt %i: %i\n",rt, data2);
                  int64_t product = rs*rt;
                  RegFile[33] = product & 0xFFFFFFFF;
                  RegFile[32] = product & 0xFFFFFFFF00000000;
                  printf("LOW: %08x\n",product & 0xFFFFFFFF);
                  printf("HI: %08x\n",product & 0xFFFFFFFF00000000);
              break;

              /*multu*/
              case 0x19:/***multu - Kingsley***/
                  printf("MULTU\n");
                  urs = CurrentInstruction & 0x3E00000; /*masks off rs*/
                  urs = urs >> 21;
                  udata1 = RegFile[urs];
                  printf("rs %i: %i\n",urs, udata1);
                  urt = CurrentInstruction & 0x1F0000; /*masks off rt*/
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
                  printf("rs%i: %i\n",rs,RegFile[rs]);
                  rt = CurrentInstruction & 0x1F0000;
                  rt = rt >> 16;
                  printf("rt%i: %i\n",rt,RegFile[rt]);
                  if(RegFile[rt] != 0){
                      quoitent= RegFile[rs] / RegFile[rt];
                      printf("quotient: %i\n",quoitent);
                      rem = RegFile[rs] % RegFile[rt];
                      printf("rem: %i\n",rem);
                  }
                  else{
                      quoitent = NULL; /*cannot divide by 0*/
                      rem = 0;
                      printf("Cannot divide by 0\n");
                  }
                  /*printf("quoitent: %04x\n",quoitent);*/
                  /*printf("remainder: %04x\n",rem );*/
                  RegFile[33] = quoitent; /*Write to LO*/
                  RegFile[32] = rem; /*Write to HI*/
              break;

              case 0x1B:/***divu - Jeter**/
                  printf("DIVU\n");
                  uint32_t urem, uquoitent;
                  urs = CurrentInstruction & 0x3E00000;
                  urs = urs >> 21;
                  printf("rs: %04x\n",rs);
                  urt = CurrentInstruction & 0x1F0000;
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
                  rd = CurrentInstruction & 0xF800; /*masks off rd*/
                  rd = rd >> 11;
                  RegFile[rd] = RegFile[32];
                  printf("rd %i: %i\n",rd, RegFile[rd]);
              break;

              /*mflo*/
              case 0x12:/*mflo - Kingsley*/
                  printf("MFLO\n");
                  rd = CurrentInstruction & 0xF800; /*masks off rd*/
                  rd = rd >> 11;
                  RegFile[rd] = RegFile[33];
                  printf("rd %i: %i\n",rd, RegFile[rd]);
              break;

              /*mthi*/
              case 0x11: /*mthi - Jeter*/
                  printf("MTHI\n");
                  rs = CurrentInstruction & 0x3E00000;
                  rs = rs >> 21;
                  printf("RS %i: %i\n",rs, RegFile[rs]);
                  RegFile[32] = RegFile[rs];
                  printf("HI: %i\n",rs, RegFile[32]);
              break;

              /*mtlo*/
              case 0x13: /*mtlo - Jeter*/
                  printf("MTLO\n");
                  rs = CurrentInstruction & 0x3E00000;
                  rs = rs >> 21;
                  printf("RS %i: %i\n",rs, RegFile[rs]);
                  RegFile[33] = RegFile[rs];
                  printf("LO: %i\n",rs, RegFile[33]);
              break;

              /*and*/
              case 0x24:
                   printf("AND\n");
                   rs = CurrentInstruction & 0x3E00000;
                   rs = rs >> 21;
                   printf("rs %i: %x\n",rs, RegFile[rs]);
                   rt = CurrentInstruction & 0x1F0000;
                   rt = rt >> 16;
                   printf("rt %i: %x\n",rt, RegFile[rt]);
                   rd = CurrentInstruction & 0xF800;
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
                   rt = CurrentInstruction & 0x1F0000;
                   rt = rt >> 16;
                   printf("rt %i: %x\n",rt, RegFile[rt]);
                   rd = CurrentInstruction & 0xF800;
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
                   rt = CurrentInstruction & 0x1F0000;
                   rt = rt >> 16;
                   printf("rt %i: %i\n",rt, RegFile[rt]);
                   rd = CurrentInstruction & 0xF800;
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
                   rt = CurrentInstruction & 0x1F0000;
                   rt = rt >> 16;
                   printf("rt %i: %i\n",rt, RegFile[rt]);
                   rd = CurrentInstruction & 0xF800;
                   rd = rd >> 11;
                   printf("rs %i: %i\n",rd, RegFile[rd]);
                   temp = RegFile[rs] | RegFile[rt];
                   printf("temp: %04x\n",temp);
                   RegFile[rd] = temp;
              break;

              /*sll / NOP */
              case 0x0: /*sll/nop - Jeter*/
                  urt = CurrentInstruction & 0x1F0000; /*masks off rt*/
                  urt = urt >> 16;
                  udata1 = RegFile[urt];
                  printf("rt %i: %i\n",urt, udata2);
                  urd = CurrentInstruction & 0xF800; /*masks off rd*/
                  urd = urd >> 11;
                  uint32_t usa = CurrentInstruction & 0x7C0; /*masks off sa*/
                  usa = usa >> 6;
                  printf("rt %04x: %i\n",usa, RegFile[usa]);
                  udata2 = RegFile[usa];
                  if(urt == 0 && urd == 0 && usa == 0){
                    printf("NOP\n");
                    break;
                  }
                  else{
		     printf("SLL\n");
                    utemp = udata1 << udata2;
                    printf("temp: %i\n",utemp);
                    RegFile[urd] = utemp;
                    printf("rd %i: %i\n",urd,RegFile[urd]);
                    break;
                  }

              /*sllv*/
              case 0x4: /*sllv - Jeter*/
                  printf("SLLV\n");
                  urs = CurrentInstruction & 0x3E00000; /*masks off rs*/
                  urs = urs >> 21;
                  udata1 = RegFile[urs];
                  printf("rs %i: %i\n",urs, udata1);
                  urt = CurrentInstruction & 0x1F0000; /*masks off rt*/
                  urt = urt >> 16;
                  udata2 = RegFile[urt];
                  printf("rt %i: %i\n",urt, udata2);
                  urd = CurrentInstruction & 0xF800; /*masks off rd*/
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
                  rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
                  rt = rt >> 16;
                  data2 = RegFile[rt];
                  printf("rt %i: %i\n",rt, data2);
                  rd = CurrentInstruction & 0xF800; /*masks off rd*/
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
                  urt = CurrentInstruction & 0x1F0000; /*masks off rt*/
                  urt = urt >> 16;
                  udata2 = RegFile[urt];
                  printf("rt %i: %i\n",urt, udata2);
                  urd = CurrentInstruction & 0xF800; /*masks off rd*/
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
                  rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
                  rt = rt >> 16;
                  data1 = RegFile[rt];
                  printf("rt %i: %i\n",rt, data2);
                  rd = CurrentInstruction & 0xF800; /*masks off rd*/
                  rd = rd >> 11;
                  int32_t sa = CurrentInstruction & 0x7C0; /*masks off sa*/
                  sa = sa >> 6;
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
                  urt = CurrentInstruction & 0x1F0000; /*masks off rt*/
                  urt = urt >> 16;
                  udata2 = RegFile[urt];
                  printf("rt %i: %i\n",urt, udata2);
                  urd = CurrentInstruction & 0xF800; /*masks off rd*/
                  urd = urd >> 11;
                  utemp = udata2 >> udata1;
                  printf("temp: %i\n",utemp);
                  RegFile[urd] = utemp;
                  printf("rd %i: %i\n",urd,RegFile[urd]);
              break;

              /*srl*/
              case 0x2: /*srl - Jeter*/
                  printf("SRL\n");
                  urt = CurrentInstruction & 0x1F0000; /*masks off rt*/
                  urt = urt >> 16;
                  udata1 = RegFile[urt];
                  printf("rt %i: %i\n",urt, udata2);
                  urd = CurrentInstruction & 0xF800; /*masks off rd*/
                  urd = urd >> 11;
                  usa = CurrentInstruction & 0x7C0; /*masks off sa*/
                  usa = usa >> 6;
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
                  urt = CurrentInstruction & 0x1F0000; /*masks off rt*/
                  urt = urt >> 16;
                  udata2 = RegFile[urt];
                  printf("rt %i: %i\n",rt, data2);
                  urd = CurrentInstruction & 0xF800; /*masks off rd*/
                  urd = urd >> 11;
                  utemp = udata2 >> udata1;
                  printf("temp: %i\n",utemp);
                  RegFile[urd] = utemp;
                  printf("rd %i: %i\n",urd,RegFile[urd]);
              break;

              case 0x9: /*jalr - Jeter (PROBABLY DOESN"T WORK???)*/
                  printf("JALR\n");
                  rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
                  rs = rs >> 21;
                  data1 = RegFile[rs];
                  printf("rs %i: %i\n",rs, data1);
                  rd = CurrentInstruction & 0xF800; /*masks off rd*/
                  rd = rd >> 11;
		   printf("rd %i\n",rd);
                  if(rd == 0)
                    rd = 0x1F; /*selects register 31 (implied)*/
                  if(RegFile[rs] != RegFile[rd]){
                    RegFile[rd] = PC + 8; /*next address following the branch*/
                    newPC = data1; /*branch delay slot*/
                  }
                  printf("rd %i: %i\n",rd,RegFile[rd]);
              break;

              case 0x8: /*jr - Jeter*/
                  printf("JR\n");
                  rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
                  rs = rs >> 21;
                  printf("rs %i: %i\n",rs, RegFile[rs]);
                  newPC = RegFile[rs];
              break;

              case 0xC: /*syscall - Jeter*/
                  SyscallExe(RegFile[2]); //probably not right
              break;
            }
          break; /*op 0*/

          case 0x8:/*addi Kingsley*/
              printf("ADDI\n");
              int rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
              rs = rs >> 21;
              printf("rs %i: %i\n",rs,RegFile[rs]);
              int32_t rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
              rt = rt >> 16;
              int16_t immediate = CurrentInstruction & 0xFFFF;
              printf("immediate: %i\n",immediate);
              temp = RegFile[rs] + immediate;
              printf("temp: %i\n",temp);
              RegFile[rt] = temp;
              printf("rt %i: %i\n",rt,RegFile[rt]);
          break;

          case 0x9:/*addiu - Kingsley*/
              printf("ADDIU\n");
              uint32_t urs = CurrentInstruction & 0x3E00000; /*masks off rs*/
              urs = urs >> 21;
              printf("rs: %04x\n",urs);
              uint32_t urt = CurrentInstruction & 0x1F0000; /*masks off rt*/
              urt = urt >> 16;
              printf("rt: %04x\n",urt);
              uint16_t uimmediate = CurrentInstruction & 0xFFFF;
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
              rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
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
              rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
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
              rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
              rt = rt >> 16;
              immediate = CurrentInstruction & 0xFFFF;
              printf("immediate: %04x\n",immediate);
              temp = RegFile[rs] ^ immediate;
              printf("temp: %04x\n",temp);
              RegFile[rt] = temp;
              printf("rt %i: %i\n",rt,RegFile[rt]);
          break;

          case 0xA:/*slti - Jeter*/
              printf("SLTI\n");
              rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
              rs = rs >> 21;
              printf("rs: %04x\n",rs);
              rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
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
              urt = CurrentInstruction & 0x1F0000; /*masks off rt*/
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

          /*beq*/
          case 0x4:/*beq - Kingsley*/
              printf("BEQ\n");
              rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
              rs = rs >> 21;
              printf("rs %i: %i\n",rs,RegFile[rs]);
              rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
              rt = rt >> 16;
              printf("rt %i: %i\n",rt,RegFile[rt]);
              int16_t target_offset = CurrentInstruction & 0xFFFF;
              printf("target_offset: %04x\n",target_offset);
              target_offset = target_offset << 2;
              printf("target_offset shifted: %04x\n",target_offset);
              if(RegFile[rs] == RegFile[rt]){
		newPC = PC + target_offset + 4;/*THIS IS PROBABLY WRONNNNGGG*/
              }
          break;

          case 0x14:/*beql*/
              printf("BEQL\n");
              rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
              rs = rs >> 21;
              printf("rs %i: %i\n",rs,RegFile[rs]);
              rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
              rt = rt >> 16;
              printf("rt %i: %i\n",rt,RegFile[rt]);
              target_offset = CurrentInstruction & 0xFFFF;
              target_offset = target_offset << 2;
              printf("target_offset: %i\n",target_offset);
              if(RegFile[rs] == RegFile[rt]){
                newPC = PC + target_offset+4;
              }
              else
                PC = PC + 8; /*skip to next instruction*/
          break;

          case 0x1:
	      printf("cond: %x\n", cond);
              switch(cond){
                case 0x1: /*bgez*/
                  printf("BGEZ\n");
                  rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
                  rs = rs >> 21;
                  printf("rs %i: %i\n",rs,RegFile[rs]);
                  target_offset = CurrentInstruction & 0xFFFF;
                  target_offset = target_offset << 2;
                  printf("target_offset: %i\n",target_offset);
                  if(RegFile[rs] >= 0){
                    newPC = PC + target_offset+4;
                  }
                break;

                case 0x11: /*bgezal*/
                  printf("BGEZAL\n");
                  rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
                  rs = rs >> 21;
                  printf("rs %i: %i\n",rs,RegFile[rs]);
                  target_offset = CurrentInstruction & 0xFFFF;
                  target_offset = target_offset << 2;
                  printf("target_offset: %i\n",target_offset);
                  RegFile[31] = PC + 8;
                  if(RegFile[rs] == RegFile[rt]){
                    newPC = PC + target_offset +4;
                  }
                break;

                case 0x0:/*bltz*/
                  printf("BLTZ\n");
                  rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
                  rs = rs >> 21;
                  printf("rs %i: %i\n",rs,RegFile[rs]);
                  target_offset = CurrentInstruction & 0xFFFF;
                  target_offset = target_offset << 2;
                  printf("target_offset: %i\n",target_offset);
                  if(RegFile[rs] < 0){
                    newPC = PC + target_offset + 4;
                  }
                break;

                case 0x10: /*bltzal*/
                  printf("BLTZAL\n");
                  rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
                  rs = rs >> 21;
                  printf("rs %i: %i\n",rs,RegFile[rs]);
                  target_offset = CurrentInstruction & 0xFFFF;
                  target_offset = target_offset << 2;
                  printf("target_offset: %i\n",target_offset);
                  RegFile[31] = PC + 8;
                  if(RegFile[rs] <=0){
		     RegFile[31] = PC + 8;
                    newPC = PC + target_offset + 4;
                  }
                break;
              }
          break;

          case 0x7: /*bgtz*/
              printf("BGTZ\n");
              rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
              rs = rs >> 21;
              printf("rs %i: %i\n",rs,RegFile[rs]);
              target_offset = CurrentInstruction & 0xFFFF;
              target_offset = target_offset << 2;
              printf("target_offset: %i\n",target_offset);
              if(RegFile[rs] > 0){newPC = PC + 4;
                newPC = PC + target_offset+4;
              }
          break;

          case 0x6: /*blez*/
              printf("BLEZ\n");
              rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
              rs = rs >> 21;
              printf("rs %i: %i\n",rs,RegFile[rs]);
              target_offset = CurrentInstruction & 0xFFFF;
              target_offset = target_offset << 2;
              printf("target_offset: %i\n",target_offset);
              if(RegFile[rs] <= 0){
                newPC = PC + target_offset+4;
              }
          break;

          case 0x16: /*blezl*/
              printf("BLEZL\n");
              rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
              rs = rs >> 21;
              printf("rs %i: %i\n",rs,RegFile[rs]);
              target_offset = CurrentInstruction & 0xFFFF;
              target_offset = target_offset << 2;
              printf("target_offset: %i\n",target_offset);
              if(RegFile[rs] <= 0){
                newPC = PC + target_offset+4;
              }
              else
                PC = PC + 4; /*skips to next instruction*/
          break;

          case 0x5: /*bne*/
              printf("BNE\n");
              rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
              rs = rs >> 21;
              printf("rs %i: %i\n",rs,RegFile[rs]);
              rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
              rt = rt >> 16;
              printf("rt %i: %i\n",rt,RegFile[rt]);
              target_offset = CurrentInstruction & 0xFFFF;
              target_offset = target_offset << 2;
              printf("target_offset: %i\n",target_offset);
              if(RegFile[rs] != RegFile[rt]){
                newPC = PC + target_offset+4;
              }
          break;

          case 0x15: /*bnel*/
              printf("BNEL\n");
              rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
              rs = rs >> 21;
              printf("rs %i: %i\n",rs,RegFile[rs]);
              rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
              rt = rt >> 16;
              printf("rt %i: %i\n",rt,RegFile[rt]);
              target_offset = CurrentInstruction & 0xFFFF;
              target_offset = target_offset << 2;
              printf("target_offset: %i\n",target_offset);
              if(RegFile[rs] != RegFile[rt]){
                newPC = PC + target_offset+4;
              }
              else
                PC = PC + 4;
          break;
         
          case 0x2: /*j - Jeter*/
              printf("J\n");
              int32_t instr = CurrentInstruction & 0x3FFFFFF; /*masks off instr_index*/
              PC = PC | instr | 0; //no idea if this works
              printf("instr: %04x\n",instr);
          break;

          case 0x3: /*jal - Jeter*/
              printf("JAL\n");
              instr = CurrentInstruction & 0x3FFFFFF; /*masks off instr_index*/
              RegFile[31] = PC + 8; //return address
              newPC = PC | instr | 0; //no idea if this works
              printf("instr: %04x\n",instr);
          break;

          case 0x20: /*lb - Jeter*/
              printf("LB\n");
              int32_t base = CurrentInstruction & 0x3E00000; /*masks off base*/
              base = base >> 21;
              printf("base %i: %i\n",base,RegFile[base]);
              rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
              rt = rt >> 16;
              printf("rt %i: %i\n",rt,RegFile[rt]);
              target_offset = CurrentInstruction & 0xFFFF;
              printf("target_offset: %i\n",target_offset);
              RegFile[rt] = readWord((RegFile[base] + target_offset),0);
          break;

          case 0x24: /*lbu - Jeter*/
              printf("LBU\n");
              uint32_t ubase = CurrentInstruction & 0x3E00000; /*masks off base*/
              ubase = ubase >> 21;
              printf("base %i: %i\n",ubase,RegFile[ubase]);
              urt = CurrentInstruction & 0x1F0000; /*masks off rt*/
              urt = urt >> 16;
              printf("rt %i: %i\n",urt,RegFile[urt]);
              uint32_t utarget_offset = CurrentInstruction & 0xFFFF;
              printf("target_offset: %i\n",utarget_offset);
              RegFile[urt] = readWord((RegFile[ubase] + utarget_offset),0);
          break;
    
          case 0x21: /*lh - Jeter*/
              printf("LH\n");
              base = CurrentInstruction & 0x3E00000; /*masks off base*/
              base = base >> 21;
              printf("base %i: %i\n",base,RegFile[base]);
              rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
              rt = rt >> 16;
              printf("rt %i: %i\n",rt,RegFile[rt]);
              target_offset = CurrentInstruction & 0xFFFF;
              printf("target_offset: %i\n",target_offset);
              data1 = readWord((RegFile[base] + target_offset),0) >> 16; /*sign extend half word*/
              RegFile[rt] = data1;
          break;

          case 0x25: /*lhu - Jeter*/
              printf("LHU\n");
              ubase = CurrentInstruction & 0x3E00000; /*masks off base*/
              ubase = ubase >> 21;
              printf("base %i: %i\n",ubase,RegFile[ubase]);
              urt = CurrentInstruction & 0x1F0000; /*masks off rt*/
              urt = urt >> 16;
              printf("rt %i: %i\n",urt,RegFile[urt]);
              utarget_offset = CurrentInstruction & 0xFFFF;
              printf("target_offset: %i\n",utarget_offset);
              udata1 = readWord((RegFile[ubase] + target_offset),0) >> 16; /*0 extended half word*/
              RegFile[urt] = udata1;
          break;

          case 0xF: /*lui - Jeter*/
              printf("LUI\n");
              rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
              rt = rt >> 16;
              printf("rt: %04x\n",rt);
              immediate = CurrentInstruction & 0xFFFF;
	       printf("immediate: %x\n",immediate);
              int32_t imm = immediate << 16; /*shift left 16 bits to load into upper half of a word*/
              printf("imm: %x\n",imm);
              RegFile[rt] = imm;
              printf("rt: %x\n",RegFile[rt]);
          break;
          
          /*LW*/
          case 0x23:/*lw - Kingsley*/
              printf("LW\n");
              rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
              rt = rt >> 16;
              int16_t offset = CurrentInstruction & 0xFFFF;
              printf("offset: %x\n",offset);
              rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
              rs = rs >> 21;
              printf("rs %i: %x\n",rs,RegFile[rs]);
              RegFile[rt] = readWord((RegFile[rs] + offset),0);
              printf("rt %i: %i\n",rt,RegFile[rt]);
          break;

          case 0x22:/*lwl*/
              printf("LWL\n");
              base = CurrentInstruction & 0x3E00000; /*masks off base*/
              base = base >> 21;
              printf("base %i: %i\n",base,RegFile[base]);
              rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
              rt = rt >> 16;
              offset = CurrentInstruction & 0xFFFF;
              printf("offset: %x\n",offset);
              data1 = readWord((RegFile[base] + offset),0) >> 16;
              data1 = data1 << 16;
              RegFile[rt] |= data1;
              printf("rt %i: %i\n",rt,RegFile[rt]);
          break;

          case 0x26:/*lwr*/
              printf("LWR\n");
              base = CurrentInstruction & 0x3E00000; /*masks off base*/
              base = base >> 21;
              printf("base %i: %i\n",base,RegFile[base]);
              rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
              rt = rt >> 16;
              offset = CurrentInstruction & 0xFFFF;
              printf("offset: %x\n",offset);
              udata1 = readWord((RegFile[base] + offset),0) << 16;
              udata1 = udata1 >> 16; /*unsigned so when shifted right, it won't sign extend*/
              RegFile[rt] |= udata1;
              printf("rt %i: %i\n",rt,RegFile[rt]);
          break;

          case 0x28:/*sb - Jeter*/
              printf("SB\n");
              base = CurrentInstruction & 0x3E00000; /*masks off base*/
              base = base >> 21;
              printf("base %i: %i\n",base,RegFile[base]);
              rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
              rt = rt >> 16;
              printf("rt %i: %i\n",rt,RegFile[rt]);
              data1 = RegFile[rt];
              target_offset = CurrentInstruction & 0xFFFF;
              printf("target_offset: %i\n",target_offset);
              writeWord((RegFile[base] + target_offset),data1,0);
          break;

          case 0x29: /*sh - Jeter*/
              printf("SH\n");
              base = CurrentInstruction & 0x3E00000; /*masks off base*/
              base = base >> 21;
              printf("base %i: %i\n",base,RegFile[base]);
              rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
              rt = rt >> 16;
              printf("rt %i: %i\n",rt,RegFile[rt]);
              target_offset = CurrentInstruction & 0xFFFF;
              printf("target_offset: %i\n",target_offset);
              data1 = RegFile[rt] << 16;
              data1 = data1 >> 16;
              writeWord((RegFile[base] + target_offset),data1,0);
          break;

          case 0x2B:/*sw - Jeter*/
              printf("SW\n");
              base = CurrentInstruction & 0x3E00000; /*masks off base*/
              base = base >> 21;
              printf("base %i: %i\n",base,RegFile[base]);
              rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
              rt = rt >> 16;
              printf("rt %i: %i\n",rt,RegFile[rt]);
              target_offset = CurrentInstruction & 0xFFFF;
              printf("target_offset: %i\n",target_offset);
              data1 = RegFile[rt];
              writeWord((RegFile[base] + target_offset),data1,0);
          break;

          case 0x2A:/*swl - Jeter*/
              printf("SWL\n");
              base = CurrentInstruction & 0x3E00000; /*masks off base*/
              base = base >> 21;
              printf("base %i: %i\n",base,RegFile[base]);
              rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
              rt = rt >> 16;
              printf("rt %i: %i\n",rt,RegFile[rt]);
              target_offset = CurrentInstruction & 0xFFFF;
              printf("target_offset: %i\n",target_offset);
              data1 = RegFile[rt] >> 16;
              data1 = data1 << 16;
              writeWord((RegFile[base] + target_offset),(data1 | (RegFile[base] + target_offset)),0);
          break;

          case 0x2E:/*swr - Jeter*/
              printf("SWR\n");
              base = CurrentInstruction & 0x3E00000; /*masks off base*/
              base = base >> 21;
              printf("base %i: %i\n",base,RegFile[base]);
              rt = CurrentInstruction & 0x1F0000; /*masks off rt*/
              rt = rt >> 16;
              printf("rt %i: %i\n",rt,RegFile[rt]);
              target_offset = CurrentInstruction & 0xFFFF;
              printf("target_offset: %i\n",target_offset);
              udata1 = RegFile[rt] << 16;
              udata1 = udata1 >> 16; /*unsigned so when shifted, it won't sign extend*/
              writeWord((RegFile[base] + target_offset),(udata1 | (RegFile[base] + target_offset)),0);
          break;
      }
    printRegFile();
    PC = PC + 4;
    }
}


