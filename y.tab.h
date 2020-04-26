
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INTEGER = 258,
     FLOAT = 259,
     CHARACTER = 260,
     STRING = 261,
     VARIABLE = 262,
     WHILE = 263,
     IF = 264,
     PRINT = 265,
     FOR = 266,
     DOWHILE = 267,
     INTIDENTIFIER = 268,
     CONSTANT = 269,
     FLOATIDENTIFIER = 270,
     CHARIDENTIFIER = 271,
     STRINGIDENTIFIER = 272,
     IFX = 273,
     ELSE = 274,
     OR = 275,
     AND = 276,
     NE = 277,
     EQ = 278,
     LE = 279,
     GE = 280,
     UMINUS = 281
   };
#endif
/* Tokens.  */
#define INTEGER 258
#define FLOAT 259
#define CHARACTER 260
#define STRING 261
#define VARIABLE 262
#define WHILE 263
#define IF 264
#define PRINT 265
#define FOR 266
#define DOWHILE 267
#define INTIDENTIFIER 268
#define CONSTANT 269
#define FLOATIDENTIFIER 270
#define CHARIDENTIFIER 271
#define STRINGIDENTIFIER 272
#define IFX 273
#define ELSE 274
#define OR 275
#define AND 276
#define NE 277
#define EQ 278
#define LE 279
#define GE 280
#define UMINUS 281




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 39 ".\\parser.y"

    int iValue;                 /* integer value */
    float fValue;               /* float value */
    char cValue;                /* char value */
    char* sValue;              /* string value */
    char* sIndex;                /* symbol table index */
    nodeType *nPtr;             /* node pointer */



/* Line 1676 of yacc.c  */
#line 115 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


