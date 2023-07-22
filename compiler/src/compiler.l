/*
 *   This file is part of SIL Compiler.
 *
 *  SIL Compiler is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SIL Compiler is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SIL Compiler.  If not, see <http://www.gnu.org/licenses/>.
 */

%{
	
//#define YYSTYPE NODPTR
#include "../include/AbsSynTree.h"
#include "../include/symbolTable.h"
#include "y.tab.h"
void  yyerror(char *);
int lineno=1;
int 	Lineno;
int     wflag;

%}

%option noyywrap
digit	[0-9]
letter	[a-zA-Z]

%%
main 		{ return MAIN; }	
return 		{ return RETURN; }
begin		{ return BEG; }	
end			{ return END; }	
read		{ return READ; }
write		{ return WRITE; }	
integer		{ return T_INT; }	
boolean		{ return T_BOOL; }	
decl		{ return DECL; }
enddecl		{ return ENDDECL; }
if			{ return IF; }
then		{ return THEN; }	
else		{ return ELSE; }	
endif		{ return ENDIF; }
do			{ return DO; }
while		{ return WHILE; }
endwhile	{ return ENDWHILE; }
for 		{ return FOR; }		
"=="		{ return EQUALEQUAL; }	
"<="		{ return LESSTHANOREQUAL; }
">="		{ return GREATERTHANOREQUAL; }
"!="		{ return NOTEQUAL; }	
AND			{ return LOGICAL_AND; }	
OR			{ return LOGICAL_OR; }
NOT			{ return LOGICAL_NOT; }
true		{ return T; }	
false		{ return F; }
{digit}+ 	{ yylval.iValue = atoi(yytext); return NUM; }
{letter}({letter}|{digit})*		{ char* variableName = strndup(yytext,yyleng); yylval.charPtr = variableName;return VAR; }
[-+*/=()><;,\{\}\[\]\%]         { return yytext[0]; }
["]                             { return yytext[0]; }
"\n"                           { lineno++; }
"\\n"   { char* variableName = strndup(yytext,yyleng); yylval.charPtr = variableName; return VAR; } 
[ \t] ;				
.	  ;	{ yyerror("Unknown symbol\n"); }
%%

