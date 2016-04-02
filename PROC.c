
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

        if ((funct == 0x20) && (op == 0)){/***add - Kingsley***/
            printf("ADD\n");
            int32_t rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
            rs = rs >> 21;
            data1 = readWord(rs, 0);
            printf("rs %i: %i\n",rs, data1);
            int32_t rt = CurrentInstruction & 0xF8000; /*masks off rt*/
            rt = rt >> 16;
            data2 = readWord(rt, 0);
            printf("rt %i: %i\n",rt, data2);
            int32_t rd = CurrentInstruction & 0xFC00; /*masks off rd*/
            rd = rd >> 11;
            temp = data1 + data2;
            printf("temp: %i\n",temp);
            writeWord(rd,temp,0);
            printf("rd %i: %i\n",rd,readWord(rd,0));
        }

        if (op == 0x8){/***addi - Kingsley***/
            printf("ADDI\n");
            int32_t rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
            rs = rs >> 21;
            printf("rs: %04x\n",readWord(rs,0));
            int32_t rt = CurrentInstruction & 0xF8000; /*masks off rt*/
            rt = rt >> 16;
            printf("rt: %04x\n",readWord(rs,0));
            int32_t immediate = CurrentInstruction & 0xFFFF;
            printf("immediate: %04x\n",immediate);
            temp = readWord(rs,0) + immediate;
            printf("temp: %04x\n",temp);
            writeWord(rt,temp,false);
            printf("rt: %04x\n",readWord(rt,0));
        }

        if (op == 0x9){/***addiu - Kingsley***/
            printf("ADDIU\n");
            uint32_t rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
            rs = rs >> 21;
            printf("rs: %04x\n",readWord(rs,0));
            uint32_t rt = CurrentInstruction & 0xF8000; /*masks off rt*/
            rt = rt >> 16;
            printf("rt: %04x\n",readWord(rs,0));
            uint32_t immediate = CurrentInstruction & 0xFFFF;
            printf("immediate: %04x\n",immediate);
            temp = readWord(rs,0) + immediate;
            printf("temp: %04x\n",temp);
            writeWord(rt,temp,false);
            printf("rt: %04x\n",readWord(rt,0));
        }

        if ((funct == 0x21) && (op == 0)){/***addu - Kingsley***/
            printf("ADDU\n");
            uint32_t rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
            rs = rs >> 21;
            printf("rs: %04x\n",readWord(rs,0));
            uint32_t rt = CurrentInstruction & 0xF8000; /*masks off rt*/
            rt = rt >> 16;
            printf("rt: %04x\n",readWord(rs,0));
            uint32_t rd = CurrentInstruction & 0xFC00; /*masks off rd*/
            rd = rd >> 11;
            temp = readWord(rs,0) + readWord(rt,0);
            printf("temp: %04x\n",temp);
            writeWord(rd,temp,false);
            printf("rd: %04x\n",readWord(rd,0));
        }

        if(funct == 0x22 && (op == 0)){/***sub - Jeter**/
            printf("SUB\n");
<<<<<<< HEAD
	        int32_t rs, rt, rd;
	        rs = CurrentInstruction & 0x3E00000;
	        rs = rs >> 21;
	        printf("rs: %04x\n",rs);
	        rt = CurrentInstruction & 0xF8000;
	        rt = rt >> 16;
	        printf("rt: %04x\n",rt);
	        rd = CurrentInstruction & 0xFC00;
	        rd = rd >> 11;
	        printf("rd: %04x\n",rd);
	        temp = readWord(rs,0) - readWord(rt,0);
	        printf("temp: %04x\n",temp);
	        writeWord(rd, temp, 0);
=======
	       int32_t rs, rt, rd;
	       rs = CurrentInstruction & 0x3E00000;
	       rs = rs >> 21;
	       printf("rs: %04x\n",rs);
	       rt = CurrentInstruction & 0xF8000;
	       rt = rt >> 16;
	       printf("rt: %04x\n",rt);
	       rd = CurrentInstruction & 0xFC00;
	       rd = rd >> 11;
	       printf("rd: %04x\n",rd);
	       temp = readWord(rs,0) - readWord(rt,0);
	       printf("temp: %04x\n",temp);
	       writeWord(rd, temp, 0);
>>>>>>> origin/master
	   }

       if(funct == 0x23 && (op == 0)){/***subu - Jeter**/
            printf("SUBU\n");
<<<<<<< HEAD
            uint32_t rs, rt, rd;
            rs = CurrentInstruction & 0x3E00000;
            rs = rs >> 21;
            printf("rs: %04x\n",rs);
            rt = CurrentInstruction & 0xF8000;
            rt = rt >> 16;
            printf("rt: %04x\n",rt);
            rd = CurrentInstruction & 0xFC00;
            rd = rd >> 11;
            printf("rd: %04x\n",rd);
            temp = readWord(rs,0) - readWord(rt,0);
            printf("temp: %04x\n",temp);
            writeWord(rd, temp, 0);
       }
       if(funct == 0x1A && (op == 0)){/***div - Jeter**/
            printf("DIV\n");
            int32_t rs, rt, quoitent, remainder;
            rs = CurrentInstruction & 0x3E00000;
            rs = rs >> 21;
            printf("rs: %04x\n",rs);
            rt = CurrentInstruction & 0xF8000;
            rt = rt >> 16;
            printf("rt: %04x\n",rt);
            if(rt != 0){
                quoitent= readWord(rs,0) / readWord(rt,0);
                remainder = readWord(rs, 0) % readWord(rt, 0);
                }
            else
                quoitent = NULL; /*cannot divide by 0*/
                printf("Cannot divide by 0\n");
            }
            printf("quoitent: %04x\n",quoitent);
            printf("remainder: %04x\n",remainder );
            writeWord(LO, quoitent, 0); /*Write to LO (probably not right syntax)*/
            writeWord(HI, remainder, 0); /*Write to HI (probably not right syntax)*/
       }
       if(funct == 0x1B && (op == 0)){/***divu - Jeter**/
            printf("DIVU\n");
            uint32_t rs, rt, quoitent, remainder;
            rs = CurrentInstruction & 0x3E00000;
            rs = rs >> 21;
            printf("rs: %04x\n",rs);
            rt = CurrentInstruction & 0xF8000;
            rt = rt >> 16;
            printf("rt: %04x\n",rt);
            if(rt != 0){
                quoitent= readWord(rs,0) / readWord(rt,0);
                remainder = readWord(rs, 0) % readWord(rt, 0);
                }
            else
                quoitent = NULL; /*cannot divide by 0*/
                printf("Cannot divide by 0\n");
            }
            printf("quoitent: %04x\n",quoitent);
            printf("remainder: %04x\n",remainder );
            writeWord(LO, quoitent, 0); /*Write to LO (probably not right syntax)*/
            writeWord(HI, remainder, 0); /*Write to HI (probably not right syntax)*/
       }
       if(funct == 0x24 && (op == 0)){/***and - Jeter**/
            printf("AND\n");
            int32_t rs, rt, rd;
            rs = CurrentInstruction & 0x3E00000;
            rs = rs >> 21;
            printf("rs: %04x\n",rs);
            rt = CurrentInstruction & 0xF8000;
            rt = rt >> 16;
            printf("rt: %04x\n",rt);
            rd = CurrentInstruction & 0xFC00;
            rd = rd >> 11;
            printf("rd: %04x\n",rd);
            temp = readWord(rs,0) & readWord(rt,0);
            printf("temp: %04x\n",temp);
            writeWord(rd, temp, 0);
       }
       if(funct == 0x24 && (op == 0)){/***andi - Jeter**/
            printf("ANDI\n");
            int32_t rs, rt, immediate;
            rs = CurrentInstruction & 0x3E00000;
            rs = rs >> 21;
            printf("rs: %04x\n",rs);
            rt = CurrentInstruction & 0xF8000;
            rt = rt >> 16;
            printf("rt: %04x\n",rt);
            immediate = CurrentInstruction & 0xFFFF;
            printf("immediate: %04x\n",immediate);
            temp = readWord(rs,0) & readWord(rt,0);
            printf("temp: %04x\n",temp);
            writeWord(rt, temp, 0);
=======
           uint32_t rs, rt, rd;
           rs = CurrentInstruction & 0x3E00000;
           rs = rs >> 21;
           printf("rs: %04x\n",rs);
           rt = CurrentInstruction & 0xF8000;
           rt = rt >> 16;
           printf("rt: %04x\n",rt);
           rd = CurrentInstruction & 0xFC00;
           rd = rd >> 11;
           printf("rd: %04x\n",rd);
           temp = readWord(rs,0) - readWord(rt,0);
           printf("temp: %04x\n",temp);
           writeWord(rd, temp, 0);
>>>>>>> origin/master
       }

       if(funct == 0x25 && (op == 0)){/***OR - Kingsley**/
            printf("OR\n");
           uint32_t rs, rt, rd;
           rs = CurrentInstruction & 0x3E00000;
           rs = rs >> 21;
           printf("rs: %04x\n",rs);
           rt = CurrentInstruction & 0xF8000;
           rt = rt >> 16;
           printf("rt: %04x\n",rt);
           rd = CurrentInstruction & 0xFC00;
           rd = rd >> 11;
           printf("rd: %04x\n",rd);
           temp = readWord(rs,0) | readWord(rt,0);
           printf("temp: %04x\n",temp);
           writeWord(rd, temp, 0);
       }

       if (op == 0xD){/***ORI - Kingsley***/
            printf("ORI\n");
            int32_t rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
            rs = rs >> 21;
            printf("rs: %04x\n",readWord(rs,0));
            int32_t rt = CurrentInstruction & 0xF8000; /*masks off rt*/
            rt = rt >> 16;
            printf("rt: %04x\n",readWord(rs,0));
            int32_t immediate = CurrentInstruction & 0xFFFF;
            printf("immediate: %04x\n",immediate);
            temp = readWord(rs,0) | immediate;
            printf("temp: %04x\n",temp);
            writeWord(rt,temp,false);
            printf("rt: %04x\n",readWord(rt,0));
        }

         if(funct == 0x27 && (op == 0)){/***NOR - Kingsley**/
            printf("NOR\n");
           uint32_t rs, rt, rd;
           rs = CurrentInstruction & 0x3E00000;
           rs = rs >> 21;
           printf("rs: %04x\n",rs);
           rt = CurrentInstruction & 0xF8000;
           rt = rt >> 16;
           printf("rt: %04x\n",rt);
           rd = CurrentInstruction & 0xFC00;
           rd = rd >> 11;
           printf("rd: %04x\n",rd);
           temp = readWord(rs,0) | readWord(rt,0);
           temp = ~temp;
           printf("temp: %04x\n",temp);
           writeWord(rd, temp, 0);
       }

       if(funct == 0x26 && (op == 0)){/***XOR - Kingsley**/
            printf("XOR\n");
           uint32_t rs, rt, rd;
           rs = CurrentInstruction & 0x3E00000;
           rs = rs >> 21;
           printf("rs: %04x\n",rs);
           rt = CurrentInstruction & 0xF8000;
           rt = rt >> 16;
           printf("rt: %04x\n",rt);
           rd = CurrentInstruction & 0xFC00;
           rd = rd >> 11;
           printf("rd: %04x\n",rd);
           temp = readWord(rs,0) ^ readWord(rt,0);
           printf("temp: %04x\n",temp);
           writeWord(rd, temp, 0);
       }

       if (op == 0xE){/***XORI - Kingsley***/
            printf("XORI\n");
            int32_t rs = CurrentInstruction & 0x3E00000; /*masks off rs*/
            rs = rs >> 21;
            printf("rs: %04x\n",readWord(rs,0));
            int32_t rt = CurrentInstruction & 0xF8000; /*masks off rt*/
            rt = rt >> 16;
            printf("rt: %04x\n",readWord(rs,0));
            int32_t immediate = CurrentInstruction & 0xFFFF;
            printf("immediate: %04x\n",immediate);
            temp = readWord(rs,0) ^ immediate;
            printf("temp: %04x\n",temp);
            writeWord(rt,temp,false);
            printf("rt: %04x\n",readWord(rt,0));
        }

        /*printRegFile();*/m
        PC = PC + 4;
    } //end fori
    
    
    //Close file pointers & free allocated Memory
    closeFDT();
    CleanUp();
    return 1;
}


