/* codgen.c       Generate Assembly Code for x86         07 May 18   */

/* Copyright (c) 2018 Gordon S. Novak Jr. and The University of Texas at Austin
    */

/* Starter file for CS 375 Code Generation assignment.           */
/* Written by Gordon S. Novak Jr.                  */

/* This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License (file gpl.text) for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "token.h"
#include "symtab.h"
#include "lexan.h"
#include "genasm.h"
#include "codegen.h"
#include "pprint.h"

void genc(TOKEN code);

/* Set DEBUGGEN to 1 for debug printouts of code generation */
#define DEBUGGEN 0

int nextlabel;    /* Next available label number */
int stkframesize;   /* total stack frame size */

int reg_table[32] = {0}; /* keep track of used and unused registers */ 
struct symtbr *contents[32] = {NULL}; /* optimization for registers */

/* Top-level entry for code generator.
   pcode    = pointer to code:  (program foo (output) (progn ...))
   varsize  = size of local storage in bytes
   maxlabel = maximum label number used so far

Add this line to the end of your main program:
    gencode(parseresult, blockoffs[blocknumber], labelnumber);
The generated code is printed out; use a text editor to extract it for
your .s file.
         */

void gencode(TOKEN pcode, int varsize, int maxlabel)
  {  TOKEN name, code;
     name = pcode->operands;
     code = name->link->link;
     nextlabel = maxlabel + 1;
     stkframesize = asmentry(name->stringval,varsize);
     genc(code);
     asmexit(name->stringval);
  }

/* test if there is a function call within code: 1 if true, else 0 */
int funcallin(TOKEN code)
{
  while(code->link != NULL){
    code = code->link;
    if(code->whichval == FUNCALLOP){
      return 1;
    }
  }
  return 0;
}

/* Trivial version: always returns RBASE + 0 */
/* Get a register.   */
/* Need a type parameter or two versions for INTEGER or REAL */
int getreg(int kind)
  {
    /*     ***** fix this *****   */
    if(kind == WORD){
      for(int index = RBASE; index <= RMAX; index++ ){
        if(reg_table[index] == 0){
          reg_table[index] = 1; /* register is now used */
          return index;
        }
      }
    } else if(kind == FLOAT){
      for(int index = FBASE; index <= FMAX; index++){
        if(reg_table[index] == 0){
          reg_table[index] = 1; /* register is now used */
          return index;
        }
      }
    }
    /* otherwise punt */
     return RBASE;
  }

/* Clear register used tables to mark all registers free.  */
void clearreg()
{
  for(int index = 0; index < 32; index++){
    reg_table[index] = 0;
    contents[index] = NULL;
  }
}

/* Mark a register unused */
void unused(int reg)
{
  reg_table[reg] = 0;
}

/* Mark a register used */
void used(int reg)
{
  reg_table[reg] = 1;
}

/* Trivial version */
/* Generate code for arithmetic expression, return a register number */
int genarith(TOKEN code)
  {   int num, reg, extra_reg;
      int freg = -1;
      int source, dest;
      float float_num;
      int curr_lab = -1;
      int other_func = 0;

     if (DEBUGGEN)
       { printf("genarith\n");
	 dbugprinttok(code);
       };
      switch ( code->tokentype )
       { case NUMBERTOK:
           switch (code->basicdt)
             { case INTEGER:
		 num = code->intval;
		 reg = getreg(WORD);
		 if ( num >= MINIMMEDIATE && num <= MAXIMMEDIATE )
        asmimmed(MOVL, num, reg);
		 break;
	       case REAL:
    /*     ***** fix this *****   */
            float_num = code->realval;
            curr_lab = nextlabel++;
            makeflit(float_num, curr_lab);
            freg = getreg(FLOAT);
            asmldflit(MOVSD, curr_lab, freg);
            return freg;
		 break;
	       }
	   break;
       case IDENTIFIERTOK:
    /*     ***** fix this *****   */
        switch (code->basicdt)
          {
            case INTEGER:
              /* load variable into a register */  
              num = code->intval;
              reg = getreg(WORD);
              int off = code->symentry->offset - stkframesize; /* net offset of the var   */
              if ( num >= MINIMMEDIATE && num <= MAXIMMEDIATE )
                asmld(MOVL, off, reg, code->stringval);
              break;
            case REAL:
                float_num = code->realval;
                freg = getreg(FLOAT);
                off = code->symentry->offset - stkframesize; /* net offset of the var   */
                asmld(MOVSD, off, freg, code->stringval);
                
                return freg;
              break;
            case POINTER:
              /* asmst(MOVQ, reg, offs, lhs->stringval);*/
              break;
          }
	   break;
       case OPERATOR:
    /*     ***** fix this *****   */
        switch (code->whichval)
        {
        case PLUSOP:
          source = genarith(code->operands);
          dest = genarith(code->operands->link);
          switch (code->basicdt)
          {
          case INTEGER:
              asmrr(ADDL, dest, source);
              unused(dest);
              return source;
            break;
          default:
            break;
          return dest;
          }
          break;
        case TIMESOP:
            dest = genarith(code->operands);
            if(code->operands->whichval == FUNCALLOP){
              other_func = funcallin(code->operands);
            }
            if(other_func){
                asmsttemp(dest);
                unused(dest); 
                source = genarith(code->operands->link);
                /* now to change bakc */
                dest = getreg(FLOAT);
                asmldtemp(dest);
              } else {
                source = genarith(code->operands->link);
             }

            switch (code->basicdt)
            {
            case INTEGER:
              break;
            case REAL:
              /* mulsd case */
              asmrr(MULSD, source, dest);
              unused(source);
              return dest;
              break;
            default:
              break;
            }
          break;
        case EQOP:
        case NEOP:
        case LTOP:
        case LEOP:
        case GEOP: 
        case GTOP:
        dest = genarith(code->operands); /*lhs */
        source = genarith(code->operands->link); /* rhs */
            switch (code->basicdt)
            {
            case INTEGER:
              asmrr(CMPL, source, dest);
              unused(source);
              return dest;
              break;
            case REAL:
              break;
            default:
              break;
            }
          break;
        case FLOATOP:
            reg = genarith(code->operands);
            freg = getreg(FLOAT);
            asmfloat(reg, freg);
            return freg;
          break;
        case FUNCALLOP: 
            /* arguments */
            reg = genarith(code->operands->link); 
            switch (code->basicdt)
            {
            case INTEGER:
              /* code */
              break;
            case REAL:
              asmcall(code->operands->stringval);
                
            default:
              break;
            }
          break;
        case MINUSOP:
            reg = genarith(code->operands);
            switch (code->operands->basicdt)
            {
            case REAL:
                extra_reg = getreg(FLOAT);
                asmfneg(reg, extra_reg);
                unused(extra_reg);
              break;
            
            default:
              break;
            }
            return reg;
          break;
        default:
          break;
        }
	   break;
       };
     return reg;
    }


/* Generate code for a Statement from an intermediate-code form */
void genc(TOKEN code)
  {  /* mark all registers as unused */
     clearreg();
     int first_lab, second_lab;
     int lab;
     int curr_lab = 0;

     TOKEN tok, lhs, rhs;
     int reg, offs;
     SYMBOL sym;
     if (DEBUGGEN)
       { printf("genc\n");
	 dbugprinttok(code);
       };
     if ( code->tokentype != OPERATOR )
        { printf("Bad code token");
	  dbugprinttok(code);
	};
     switch ( code->whichval )
       { case PROGNOP:
	   tok = code->operands;
	   while ( tok != NULL )
	     {  genc(tok);
		tok = tok->link;
	      };
	   break;
	 case ASSIGNOP:                   /* Trivial version: handles I := e */
	   lhs = code->operands;
	   rhs = lhs->link;
	   reg = genarith(rhs);              /* generate rhs into a register */
	   sym = lhs->symentry;              /* assumes lhs is a simple var  */
	   offs = sym->offset - stkframesize; /* net offset of the var   */
           switch (code->basicdt)            /* store value into lhs  */
             { case INTEGER:
                 asmst(MOVL, reg, offs, lhs->stringval);
                 break;
                 /* ...  */
               case REAL:
                asmst(MOVSD, reg, offs, lhs->stringval);
                break;
             };
           break;
	 case FUNCALLOP:
    /*     ***** fix this *****   */
    if(code->operands->link->tokentype == STRINGTOK){
      /* treat as a literal */
      lab = nextlabel++;
      makeblit(code->operands->link->tokenval.tokenstring, lab);
      asmlitarg(lab, 5);
    }
     asmcall(code->operands->stringval);
	   break;
	 case GOTOOP:
    /*     ***** fix this *****   */
      asmjump(0, curr_lab);
	   break;
	 case LABELOP:
    /*     ***** fix this *****   */
      curr_lab = code->operands->intval;
      asmlabel(curr_lab);
	   break;
	 case IFOP:
    /*     ***** fix this *****   */
      genarith(code->operands);
      first_lab = nextlabel++;
      second_lab = nextlabel++;
      switch (code->operands->whichval)
      {
      case LEOP:
        asmjump(JLE, first_lab);
        break;
      default:
        break;
      }
      asmjump(0, second_lab);
      asmlabel(first_lab);
      genc(code->operands->link);
      asmlabel(second_lab);
	   break;
	 };
  }
