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
	#include <stdio.h>
	#include "../include/AbsSynTree.h"
	int yylex();
	void yyerror( char* );
	extern int lineno;
%}

%union
{
	int iValue;
	char *charPtr;
	node *nodePtr;
	dataType dt;
};

%token BEG END
%token T_INT T_BOOL
%token READ WRITE
%token DECL ENDDECL
%token <charPtr> VAR 
%token <iValue> NUM
%token IF THEN ELSE ENDIF
%token LOGICAL_AND LOGICAL_NOT LOGICAL_OR
%token EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%token WHILE DO ENDWHILE FOR 
%token T F 
%token MAIN RETURN


%left '<' '>'
%left EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%left '+' '-'
%left '*' '/'
%left '%'
%left LOGICAL_AND LOGICAL_OR
%left LOGICAL_NOT

%type <nodePtr> Gdecl_sec Gdecl_list Gdecl Glist Gid func arg_list arg_list1 read_stmt 
%type <nodePtr> arg var_list Fdef_sec Fdef FargList write_stmt 
%type <nodePtr> ret_stmt MainBlock Ldecl_sec Ldecl_list Lid_list stmt_list statement 
%type <nodePtr> param_list param_list1 para func_stmt expr var_expr Ldecl assign_stmt
%type <nodePtr> cond_stmt str_expr func_call
%type <dt> type ret_type arg_type func_ret_type
%type <charPtr> func_name Lid
%%

	Prog	:	Gdecl_sec Fdef_sec MainBlock { node *p1=$1,*p2=$2;p1->next=p2;p2->next=$3;start=p1; }
		;
		
	Gdecl_sec:	DECL Gdecl_list ENDDECL{ node* p=createNode(lineno,typeGDeclSec,1,$2);$$=p; }
		;
		
	Gdecl_list:                        { $$=NULL; }
		| 	Gdecl Gdecl_list           { node* p1=$1;p1->next=$2;$$=p1; }
		;
		
	Gdecl 	:	ret_type Glist ';' { $$=createNode(lineno,typeGDecl,2,$1,$2); }
		;
		
	ret_type:	T_INT		{ $$=typeInt; }
		|	T_BOOL 		{ $$=typeBool; }
		;
		
	Glist 	:	Gid { $$=$1; }
		| 	func { $$=$1; }
		|	Gid ',' Glist { node* p=$1;p->next=$3;$$=p; }
		|	func ',' Glist { node* p=$1;p->next=$3;$$=p; }
		;

	Gid	:	VAR		{ $$=createNode(lineno,typeGIdDecl,1,$1); }
		|	Gid '[' NUM ']'	{ $$=addDimGid($1,$3); }
		;

	func 	:	VAR '(' arg_list ')' { $$=createNode(lineno,typeFuncDecl,2,$1,$3); }
		;
			
	arg_list:	/* Empty */ { $$=NULL; }
		|	arg_list1 { $$=$1; }
		;
		
	arg_list1:	arg_list1 ';' arg { node *p=$1;p->next=$3;node *p2=$3;p2->arg->head=p->arg->head;$$=p2;}
		|	arg { node *p=$1;p->arg->head=$1;$$=p; }
		;
		
	arg 	:	arg_type var_list	{ $$=createNode(lineno,typeArg,2,$1,$2); }
		;
		
	arg_type:	T_INT		 { $$=typeInt; }
		|	T_BOOL 		 { $$=typeBool; }
		;

	var_list:	VAR 		 { $$=createNode(lineno,typeVarList,1,$1); }
		|	VAR ',' var_list { node *p=createNode(lineno,typeVarList,1,$1); p->next=$3;$$=p;}
		;

	Fdef_sec:	/*Empty*/ { $$=createNode(lineno,typeFDefSec,0); }
		|	Fdef_sec Fdef { node* p=$1;
		                    if(NULL == p->fDefSec->head)
							{
								p->fDefSec->head=$2;
							}
		                    if(p->fDefSec->tail)
							{
								p->fDefSec->tail->next=$2;
							}
						    p->fDefSec->tail=$2;
							$$=p; 
						  }
		;
		
	Fdef	:	func_ret_type func_name '(' FargList ')' '{' Ldecl_sec BEG stmt_list ret_stmt END '}'	{ $$=createNode(lineno,typeFDef,6,$1,$2,$4,$7,$9,$10); }
		;
		
	func_ret_type:	T_INT		{ $$=typeInt; }
		|	T_BOOL		{ $$=typeBool; }
		;
			
	func_name:	VAR		{ $$=$1; }
		;
		
	FargList:	arg_list	{ $$=$1; }
		;

	ret_stmt:	RETURN expr ';'	{ $$=$2; }
		;
			
	MainBlock: 	func_ret_type main '('')''{' Ldecl_sec BEG stmt_list ret_stmt END  '}' { $$=createNode(lineno,typeMainBlock,4,$1,$6,$8,$9); }
					  
		;
		
	main	:	MAIN		{ 					}
		;
		
	Ldecl_sec:	DECL Ldecl_list ENDDECL	{ $$=$2; }
		;

	Ldecl_list:	/*Empty*/ { $$=NULL; }	
		|	Ldecl Ldecl_list { node* p=$1;p->next=$2;$$=p; }
		;

	Ldecl	:	type Lid_list ';' { $$=createNode(lineno,typeLDecl,2,$1,$2); }	
		;

	type	:	T_INT			{ $$=typeInt; }
		|	T_BOOL			{ $$=typeBool; }
		;

	Lid_list:	Lid { $$=createNode(lineno,typeVarList,1,$1); }
		|	Lid ',' Lid_list { node* p=createNode(lineno,typeVarList,1,$1);p->next=$3;$$=p; }
		;
		
	Lid	:	VAR			{ $$=$1; }
		;

	stmt_list:	/* NULL */		{ $$=NULL; }
		|	statement stmt_list	{ node *ptr1=$1;ptr1->next=$2;$$=ptr1; }
		|	error ';' 		{  }
		;

	statement:	assign_stmt  ';'	{ $$=createNode(lineno,typeStmt,1,$1); }
		|	read_stmt ';'			{ $$=createNode(lineno,typeStmt,1,$1); }
		|	write_stmt ';'			{ $$=createNode(lineno,typeStmt,1,$1); }
		|	cond_stmt 				{ $$=createNode(lineno,typeStmt,1,$1); }
		|	func_stmt ';'			{ $$=createNode(lineno,typeStmt,1,$1); }
		| ';'                       { $$=createNode(lineno,typeStmt,1,NULL);}
		;

	read_stmt:	READ '(' var_expr ')' { $$=createNode(lineno,typeRead,1,$3); }
		;

	write_stmt:	WRITE '(' expr ')' 	{ $$=createNode(lineno,typeWrite,1,$3); }
		 | WRITE '(''"' str_expr '"'')'      { $$=createNode(lineno,typeWrite,1,$4); }

		;
	
	assign_stmt:	var_expr '=' expr 	{ $$=createNode(lineno,typeAssignment,2,$1,$3);}
		;

	cond_stmt:	IF expr THEN stmt_list ENDIF 	{ $$=createNode(lineno,typeIfElse,3,$2,$4,NULL); }
		|	IF expr THEN stmt_list ELSE stmt_list ENDIF 	{ $$=createNode(lineno,typeIfElse,3,$2,$4,$6); }
		|	WHILE expr DO stmt_list ENDWHILE ';'{ $$=createNode(lineno,typeWhileLoop,2,$2,$4); }
		|    FOR '(' assign_stmt  ';'  expr ';'  assign_stmt ')' '{' stmt_list '}' { $$=createNode(lineno,typeForLoop,4,$3,$5,$7,$10); }
		;
	
	func_stmt:	func_call 		{ $$=$1; }
		;
		
	func_call:	VAR '(' param_list ')'	{ $$=createNode(lineno,typeFCall,2,$1,$3); }
		;
		
	param_list:	/*Empty*/ { $$=NULL; }			
		|	param_list1	  { $$=$1; }
		;
		
	param_list1:	para	{ $$=$1; }		
		|	para ',' param_list1 { node *p=$1; p->next =$3; $$=p; }	
		;

	para	:	expr			{ $$=$1; }
		;

	expr	:	NUM 						{ $$=createNode(lineno,typeConstant,2,typeInt,$1); }
		|	'-' NUM							{ $$=createNode(lineno,typeConstant,2,typeInt,-$2); }
		|	var_expr						{ $$=$1; }
		|	T								{ $$=createNode(lineno,typeConstant,2,typeBool,1); }
		|	F								{ $$=createNode(lineno,typeConstant,2,typeBool,0); }
		|	'(' expr ')'					{ $$=$2; }
		|	expr '+' expr 					{ $$=createNode(lineno,typeOperator,3,"+",$1,$3); }
		|	expr '-' expr	 				{ $$=createNode(lineno,typeOperator,3,"-",$1,$3); }
		|	expr '*' expr 					{ $$=createNode(lineno,typeOperator,3,"*",$1,$3); }
		|	expr '/' expr 					{ $$=createNode(lineno,typeOperator,3,"/",$1,$3); }
		|	expr '%' expr 					{ $$=createNode(lineno,typeOperator,3,"%",$1,$3); }
		|	expr '<' expr					{ $$=createNode(lineno,typeOperator,3,"<",$1,$3); }
		|	expr '>' expr					{ $$=createNode(lineno,typeOperator,3,">",$1,$3); }
		|	expr GREATERTHANOREQUAL expr	{ $$=createNode(lineno,typeOperator,3,">=",$1,$3); }
		|	expr LESSTHANOREQUAL expr	    { $$=createNode(lineno,typeOperator,3,"<=",$1,$3); }          
		|	expr NOTEQUAL expr			    { $$=createNode(lineno,typeOperator,3,"!=",$1,$3); }        
		|	expr EQUALEQUAL expr	 		{ $$=createNode(lineno,typeOperator,3,"==",$1,$3); }				
		|	LOGICAL_NOT expr	 			{ $$=createNode(lineno,typeOperator,2,"NOT",$2); }		
		|	expr LOGICAL_AND expr	 		{ $$=createNode(lineno,typeOperator,3,"AND",$1,$3); }				
		|	expr LOGICAL_OR expr	 		{ $$=createNode(lineno,typeOperator,3,"OR",$1,$3); }				
		|	func_call						{ $$=$1; }                 

		;
	str_expr :  VAR                       { node *p=createNode(lineno,typeStr,3,$1);p->str->head=p;$$=p; }
                  | str_expr VAR   { node*p=$1;node *p2=createNode(lineno,typeStr,1,$2);p->next=p2;p2->str->head=p->str->head;$$=p2; }
                ;
	
	var_expr:	VAR	{ $$=createNode(lineno,typeVariable,1,$1); }
		|	var_expr '[' expr ']'	{ $$=addDimension($1,$3); }
		;
%%


int main(int argc, char** argv)
{
	yyparse();
	
	checkCode(start);
	if(0 == error)
	{
		printf("/*\n Abstract syntax tree\n\n");
		generateAST(start,0);
		printf("\nC code\n*/\n");
		generateCode(start,0);
	}
}

void warning(char *s, char *t) /* print warning message */
{	
	fflush(NULL);
	red();
	fprintf(stderr, "\nerror: ");
	reset();
	fprintf(stderr," %s",  s);
	if (t)
	{
		fprintf(stderr, " %s", t);
	}
	fprintf(stderr, " near line %d\n", lineno);
	fflush(NULL);
}

void yyerror(char *s) /* called for yacc syntax error */
{
	error=1;	
	warning(s, (char *) 0);
}
