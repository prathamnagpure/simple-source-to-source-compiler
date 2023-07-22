#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/symbolTable.h"
#include "../include/AbsSynTree.h"
#define ALLOC(t,v) t *v=(t*) malloc(sizeof(t));checkNull((void*)v)
#define VA va_list valist;va_start(valist, num)
#define CVA va_arg(valist,char*)
#define DVA va_arg(valist,dataType)
#define NVA va_arg(valist,node*)
#define IVA va_arg(valist,int)
#define TABS for(int i=0;i<tabs;++i){printf("    ");}
node* start=NULL;
int error=0;

void red() 
{
  fprintf(stderr,"\033[1;31m");
}

void reset() 
{
  fprintf(stderr,"\033[0m");
}

void printre(char *var, int l, int o)
{
	fflush(NULL);
	red();
	fprintf(stderr,"\nerror: ");
	reset();
	fprintf(stderr,"identifier '%s' is redeclared near line number %d which was previously declared near line number %d\n",var,l,o);
	fflush(NULL);
}

void printue(char *var, int l)
{
	fflush(NULL);
	red();
	fprintf(stderr,"\nerror: ");
	reset();
	fprintf(stderr,"Use of undeclared identifier '%s' near line number %d\n",var,l);
	fflush(NULL);
}
/* checks for redeclared variable */
int checkRedeclaredVariableError(int l,char *var,int s,int line)
{
	if(l) return insertlst(var,s,line);
	return insertgst(var,s,line);
}

/* checks for undeclared variable */
int checkUndeclaredVariableError(int l,char * var)
{
	if(l) return(NULL == findlst(var));
	return (NULL == findgst(var));
}

void checkNull(void *p)
{
	if(NULL == p)
	{
		fprintf(stderr,"Out of memory\n");
		exit(1);
	}
}
node* createNode(int lineno, nodeType type, int num, ...)
{
	ALLOC(node,newNode);
	newNode->next=NULL;
	newNode->lineno=lineno;
	switch(type)
	{
		case typeGDeclSec:
		{
			ALLOC(gDeclSecNode,p);
			VA;
			p->gDeclHead=NVA;
			newNode->type=typeGDeclSec;
			newNode->gDeclSec=p;
			return newNode;
		}
		case typeStmt:
		{
			ALLOC(stmtNode,p);
			VA;
			p->stmt=NVA;
			newNode->type=typeStmt;
			newNode->stmt=p;
			return newNode;
		}
		case typeGDecl:
		{
			ALLOC(gDeclNode,p);
			VA;
			p->type=DVA;
			p->gListHead=NVA;
			newNode->type=typeGDecl;
			newNode->gDecl=p;
			return newNode;
		}
		case typeGIdDecl:
		{
			ALLOC(gIdDeclNode,p);
			VA;
			p->name=CVA;
			p->head=NULL;
			newNode->type=typeGIdDecl;
			newNode->gIdDecl=p;
			return newNode;
		}
		case typeFuncDecl:
		{
			ALLOC(funcDeclNode,p);
			VA;
			p->name=CVA;
			p->argListHead=NVA;
			newNode->type=typeFuncDecl;
			newNode->funcDecl=p;
			return newNode;
		}
		case typeArg:
		{
			ALLOC(argNode,p);
			VA;
			p->type=DVA;
			p->head=NULL;
			p->varListHead=NVA;
			newNode->type=typeArg;
			newNode->arg=p;
			return newNode;
		}
		case typeVarList:
		{
			ALLOC(varListNode,p);
			VA;
			p->name=CVA;
			newNode->type=typeVarList;
			newNode->varList=p;
			return newNode;
		}
		case typeFDefSec:
		{
			ALLOC(fDefSecNode,p);
			p->head=NULL;
			p->tail=NULL;
			newNode->type=typeFDefSec;
			newNode->fDefSec=p;
			return newNode;
		}
		case typeFDef:
		{
			ALLOC(fDefNode,p);
			VA;
			p->retType=DVA;
			p->name=CVA;
			p->argList=NVA;
			p->lDeclSec=NVA;
			p->stmtList=NVA;
			p->retStmt=NVA;
			newNode->type=typeFDef;
			newNode->fDef=p;
			return newNode;
		}
		case typeMainBlock:
		{
			ALLOC(mainBlockNode,p);
			VA;
			p->retType=DVA;
			p->lDeclSec=NVA;
			p->stmtList=NVA;
			p->retStmt=NVA;
			newNode->type=typeMainBlock;
			newNode->mainBlock=p;
			return newNode;
		}
		case typeLDecl:
		{
			ALLOC(lDeclNode,p);
			VA;
			p->type=DVA;
			p->varListHead=NVA;
			newNode->next=NVA;
			newNode->type=typeLDecl;
			newNode->lDecl=p;
			return newNode;
		}
		case typeRead:
		{
			ALLOC(readNode,p);
			VA;
			p->var=NVA;
			newNode->type=typeRead;
			newNode->read=p;
			return newNode;
		}
		case typeWrite:
		{
			ALLOC(writeNode,p);
			VA;
			p->data=NVA;
			newNode->type=typeWrite;
			newNode->write=p;
			return newNode;
		}
		case typeAssignment:
		{
			ALLOC(assignmentNode,p);
			VA;
			p->id=NVA;
			p->expr=NVA;
			newNode->type=typeAssignment;
			newNode->assignment=p;
			return newNode;
		}
		case typeIfElse:
		{
			ALLOC(ifElseNode,p);
			VA;
			p->cond=NVA;
			p->iStmtList=NVA;
			p->eStmtList=NVA;
			newNode->type=typeIfElse;
			newNode->ifElse=p;
			return newNode;
		}
		case typeWhileLoop:
		{
			ALLOC(whileLoopNode,p);
			VA;
			p->cond=NVA;
			p->stmtList=NVA;
			newNode->type=typeWhileLoop;
			newNode->whileLoop=p;
			return newNode;
		}
		case typeForLoop:
		{
			ALLOC(forLoopNode,p);
			VA;
			p->init=NVA;
			p->cond=NVA;
			p->adv=NVA;
			p->stmtList=NVA;
			newNode->type=typeForLoop;
			newNode->forLoop=p;
			return newNode;
		}
		case typeFCall:
		{
			ALLOC(fCallNode,p);
			VA;
			p->id=CVA;
			p->params=NVA;
			newNode->type=typeFCall;
			newNode->fCall=p;
			return newNode;
		}
		case typeConstant:
		{
			ALLOC(constantNode,p);
			VA;
			p->type=DVA;
			p->value=IVA;
			newNode->type=typeConstant;
			newNode->con=p;
			return newNode;
		}
		case typeOperator:
		{
			ALLOC(operatorNode,p);
			VA;
			strcpy(p->sym,CVA);
			for(int i=0;i<num-1;++i){
				p->child[i]=NVA;
			}
			newNode->type=typeOperator;
			newNode->op=p;
			return newNode;
		}
		case typeStr:
		{
			ALLOC(strNode,p);
			VA;
			p->str=CVA;
			newNode->type=typeStr;
			newNode->str=p;
			return newNode;
		}
		case typeVariable:
		{
			ALLOC(variableNode,p);
			VA;
			p->variableName=CVA;
			p->dimCount=0;
			p->dimListHead=NULL;
			p->dimListTail=NULL;
			newNode->type=typeVariable;
			newNode->var=p;
			return newNode; 
		}
	}
	return newNode;
}

void checkCode(node* ptr)
{
	if(NULL == ptr)
	{
		return;
	}
	switch(ptr->type)
	{
		case typeGDeclSec:
		{
			node *p=ptr->gDeclSec->gDeclHead;
			while(NULL != p)
			{
				checkCode(p);
				p=p->next;
			}
			checkCode(ptr->next);
			break;
		}
		case typeGDecl:
		{
			node *p=ptr->gDecl->gListHead;
			while(NULL != p)
			{
				checkCode(p);
				p=p->next;
			}
			break;
		}
		case typeGIdDecl:
		{
			dimListNode *p=ptr->gIdDecl->head;
			int ct=0;
			while(NULL != p)
			{
				ct++;
				p=p->next;
			}
			int rv=checkRedeclaredVariableError(0,ptr->gIdDecl->name,ct,ptr->lineno);
			if(rv) 
			{
				mapNode *temp=findgst(ptr->gIdDecl->name);
				printre(ptr->gIdDecl->name,ptr->lineno,temp->lineno);
				error=1;
			}
			break;
		}
		case typeFuncDecl:
		{
			int rv=checkRedeclaredVariableError(0,ptr->funcDecl->name,-1,ptr->lineno);
			if(rv) 
			{
				mapNode *temp=findgst(ptr->funcDecl->name);
				printre(ptr->funcDecl->name,ptr->lineno,temp->lineno);
				error=1;
			}
			else
			{
				node *p=ptr->funcDecl->argListHead->arg->head;
				int ct=0;
				while(NULL != p)
				{
					node *vlp=p->arg->varListHead;
					while(NULL != vlp)
					{
						ct++;
						vlp=vlp->next;
					}
					p=p->next;	
				} 
				insertfst(ptr->funcDecl->name,ct,ptr->lineno);
			}
			freelst();	
			checkCode(ptr->funcDecl->argListHead);
			freelst();
			break;
		}
		case typeArg:
		{
			node *p=ptr->arg->head;
			int comma=0;
			while(NULL != p)
			{
				node *vlp=p->arg->varListHead;
				while(NULL != vlp)
				{
					int rv=checkRedeclaredVariableError(1,vlp->varList->name,0,ptr->lineno);
					mapNode *temp=findlst(vlp->varList->name);
					if(rv) 
					{
						printre(vlp->varList->name,ptr->lineno,temp->lineno);
						error=1;
						temp->lineno;
					}
					vlp=vlp->next;
				}
				p=p->next;	
			}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
			break;
		}
		case typeFDefSec:
		{
			node *p=ptr->fDefSec->head;
			while(NULL != p)
			{
				freelst();
				checkCode(p);
				freelst();
				p=p->next;
			}
			checkCode(ptr->next);
			break;
		}
		case typeFDef:
		{
			int rv=checkUndeclaredVariableError(0,ptr->fDef->name);
			if(rv)
			{
				error=1;
				printue(ptr->fDef->name,ptr->lineno);
			}
			else
			{
				node *p=ptr->fDef->argList->arg->head;
				int ct=0;
				while(NULL != p)
				{
					node *vlp=p->arg->varListHead;
					while(NULL != vlp)
					{
						ct++;
						vlp=vlp->next;
					}
					p=p->next;	
				}
				mapNode *rv=findfst(ptr->fDef->name);
				if(NULL == rv)
				{
					error=1;
					red();fprintf(stderr,"\nerror");reset();
					fprintf(stderr,": '%s' is not a function near line number %d\n",ptr->fDef->name,ptr->lineno);
				}
				else if(rv->data != ct)
				{
					error=1;
					red();fprintf(stderr,"\nerror");reset();
					fprintf(stderr,": too %s arguments for function '%s' near line number %d\n",(rv->data>ct)?"less":"many",ptr->fDef->name,ptr->lineno);
				}
			}
			checkCode(ptr->fDef->argList);
			checkCode(ptr->fDef->lDeclSec);
			checkCode(ptr->fDef->stmtList);
			checkCode(ptr->fDef->retStmt);
			break;
		}
		case typeMainBlock:
		{
			freelst();
			checkCode(ptr->mainBlock->lDeclSec);
			checkCode(ptr->mainBlock->stmtList);
			checkCode(ptr->mainBlock->retStmt);
			freelst();
			break;
		}
		case typeLDecl:
		{
			node *vlp=ptr->lDecl->varListHead;
			while(NULL != vlp)
			{
				int rv=checkRedeclaredVariableError(1,vlp->varList->name,0,ptr->lineno);
				if(rv)
				{
					error=1;
					mapNode *temp=findlst(vlp->varList->name);
					printre(vlp->varList->name,ptr->lineno,temp->lineno);
				}
				vlp=vlp->next;
			}
			checkCode(ptr->next);
			break;
		}
		case typeStmt:
		{
			checkCode(ptr->stmt->stmt);
			checkCode(ptr->next);
			break;
		}
		case typeRead:
		{
			checkCode(ptr->read->var);
			break;
		}
		case typeWrite:
		{
			checkCode(ptr->write->data);
			break;
		}
		case typeAssignment:
		{
			checkCode(ptr->assignment->id);
			checkCode(ptr->assignment->expr);
			break;
		}
		case typeIfElse:
		{
			checkCode(ptr->ifElse->cond);
			checkCode(ptr->ifElse->iStmtList);
			if(NULL != ptr->ifElse->eStmtList)
			{
				checkCode(ptr->ifElse->eStmtList);
			}
			break;
		}
		case typeWhileLoop:
		{
			checkCode(ptr->whileLoop->cond);
			checkCode(ptr->whileLoop->stmtList);
			break;
		}
		case typeForLoop:
		{
			checkCode(ptr->forLoop->init);
			checkCode(ptr->forLoop->cond);
			checkCode(ptr->forLoop->adv);
			checkCode(ptr->forLoop->stmtList);
			break;
		}
		case typeFCall:
		{
			int rv=checkUndeclaredVariableError(0,ptr->fCall->id);
			if(rv)
			{
				error=1;
				printue(ptr->fCall->id,ptr->lineno);
			}
			else
			{
				node *p=ptr->fCall->params;
				int ct=0;
				while(NULL != p)
				{
					ct++;
					p=p->next;
				}
				mapNode *rv=findfst(ptr->fCall->id);
				if(NULL == rv)
				{
					error=1;
					red();fprintf(stderr,"\nerror");reset();
					fprintf(stderr,": '%s' is not a function near line number %d\n",ptr->fCall->id,ptr->lineno);
				}
				else if(rv->data != ct)
				{
					error=1;
					red();fprintf(stderr,"\nerror");reset();
					fprintf(stderr,": too %s arguments for function '%s' near line number %d\n",(rv->data>ct)?"less":"many",ptr->fCall->id,ptr->lineno);
				}
			}
			node *p=ptr->fCall->params;
			while(NULL != p)
			{
				checkCode(p);
				p=p->next;
			}
			break;
		}
		case typeConstant:
		{
			break;
		}
		case typeOperator:
		{
			if(0 == strcmp(ptr->op->sym,"NOT"))
			{
				checkCode(ptr->op->child[0]);
			}
			else
			{
				checkCode(ptr->op->child[0]);
				checkCode(ptr->op->child[1]);
			}
			break;
		}
		case typeStr:
		{
			node *p=ptr->str->head;
			while(NULL != p)
			{
				p=p->next;
			}
			break;
		}
		case typeVariable:
		{
			int rv1=checkUndeclaredVariableError(0,ptr->var->variableName);
			int rv2=checkUndeclaredVariableError(1,ptr->var->variableName);
			if(rv1 && rv2)
			{
				error=1;
				printue(ptr->var->variableName,ptr->lineno);
			}
			int dim;
			if(0 == rv2)
			{
				mapNode *temp=findlst(ptr->var->variableName);
				dim=temp->data;
			}
			else
			{
				mapNode *temp=findgst(ptr->var->variableName);
				dim=temp->data;
			}
			if(dim != ptr->var->dimCount)
			{
				error=1;
				red();
				fprintf(stderr,"\nerror");
				reset();
				fprintf(stderr,": Too %s dimensions for array refrence '%s' near line number %d\n",(dim>ptr->var->dimCount)?"less":"many",ptr->var->variableName,ptr->lineno);
			}
			if(0 != ptr->var->dimCount)
			{
				node* p=ptr->var->dimListHead;
				while(NULL != p)
				{
					checkCode(p);
					p=p->next;
				}
			}
			break;
		}
	}
}

void generateAST(node* ptr,int tabs)
{
	if(NULL == ptr)
	{
		return;
	}
	switch(ptr->type)
	{
		case typeGDeclSec:
		{
			node *p=ptr->gDeclSec->gDeclHead;
			while(NULL != p)
			{
				generateAST(p,tabs);
				printf("\n");
				p=p->next;
			}
			generateAST(ptr->next,tabs);
			break;
		}
		case typeGDecl:
		{
			if(typeInt == ptr->gDecl->type)
			{
				printf("GDECL INT ");
			}
			else
			{
				printf("GDECL BOOL ");
			}
			node *p=ptr->gDecl->gListHead;
			while(NULL != p)
			{
				generateAST(p,tabs);
				p=p->next;
			}
			break;
		}
		case typeGIdDecl:
		{
			printf("VAR ");
			dimListNode *p=ptr->gIdDecl->head;
			while(NULL != p)
			{
				printf("NUM ");
				p=p->next;
			}
			break;
		}
		case typeFuncDecl:
		{
			printf("FUNDECL ");
			generateAST(ptr->funcDecl->argListHead,0);
			printf(" ");
			break;
		}
		case typeArg:
		{
			node *p=ptr->arg->head;
			int comma=0;
			printf("%s",(ptr->arg->type==typeInt)?"INT ":" BOOL ");
			while(NULL != p)
			{
				node *vlp=p->arg->varListHead;
				while(NULL != vlp)
				{
					if(comma)
					{
						printf(" ");
					}
					comma=1;
					printf("VAR");
					vlp=vlp->next;
				}
				p=p->next;	
			}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
			break;
		}
		case typeFDefSec:
		{
			node *p=ptr->fDefSec->head;
			while(NULL != p)
			{
				generateAST(p,tabs);
				p=p->next;
			}
			generateAST(ptr->next,tabs);
			break;
		}
		case typeFDef:
		{
			printf("%s FDEF ",(ptr->fDef->retType==typeInt)?"INT":"BOOL");
			generateAST(ptr->fDef->argList,0);
			printf(" \n");
			tabs++;
			generateAST(ptr->fDef->lDeclSec,tabs);
			generateAST(ptr->fDef->stmtList,tabs);
			TABS
			printf("RETURN ");
			generateAST(ptr->fDef->retStmt,0);
			printf("\n");
			tabs--;
			break;
		}
		case typeMainBlock:
		{
			printf("%s MAIN \n",(ptr->mainBlock->retType)?"INT":"BOOL");
			tabs++;
			generateAST(ptr->mainBlock->lDeclSec,tabs);
			generateAST(ptr->mainBlock->stmtList,tabs);
			TABS
			printf("RETURN ");
			generateAST(ptr->mainBlock->retStmt,0);
			printf("\n");
			tabs--;
			break;
		}
		case typeLDecl:
		{
			node *vlp=ptr->lDecl->varListHead;
			while(NULL != vlp)
			{
				TABS
				printf("LDECL %s VAR\n",(ptr->lDecl->type==typeInt)?"INT":"BOOL");
				vlp=vlp->next;
			}
			generateAST(ptr->next,tabs);
			break;
		}
		case typeStmt:
		{
			generateAST(ptr->stmt->stmt,tabs);
			if(NULL == ptr->stmt->stmt){ break; }
			switch(ptr->stmt->stmt->type)
			{
				case typeForLoop: break;
				case typeWhileLoop: break;
				case typeIfElse: break;
				default: printf("\n");
			}
			generateAST(ptr->next,tabs);
			break;
		}
		case typeRead:
		{
			TABS
			printf("READ ");
			generateAST(ptr->read->var,0);
			break;
		}
		case typeWrite:
		{
			TABS
			printf("WRITE ");
			if(ptr->write->data->type == typeStr)
			{
				generateAST(ptr->write->data,0);
			}
			else
			{
				generateAST(ptr->write->data,0);
			}
			printf(" ");
			break;
		}
		case typeAssignment:
		{
			TABS
			printf("ASSIGN ");
			generateAST(ptr->assignment->id,0);
			printf(" ");
			generateAST(ptr->assignment->expr,0);
			break;
		}
		case typeIfElse:
		{
			TABS
			printf("IF ");
			generateAST(ptr->ifElse->cond,0);
			printf(" \n");
			tabs++;
			generateAST(ptr->ifElse->iStmtList,tabs);
			tabs--;
			if(NULL != ptr->ifElse->eStmtList)
			{
				TABS
				printf("ELSE\n");
				tabs++;
				generateAST(ptr->ifElse->eStmtList,tabs);
				tabs--;
			}
			break;
		}
		case typeWhileLoop:
		{
			TABS
			printf("WHILE ");
			generateAST(ptr->whileLoop->cond,0);
			printf(" \n");
			tabs++;
			generateAST(ptr->whileLoop->stmtList,tabs);
			tabs--;
			break;
		}
		case typeForLoop:
		{
			TABS
			printf("FOR ");
			generateAST(ptr->forLoop->init,0);
			printf(" ");
			generateAST(ptr->forLoop->cond,0);
			printf(" ");
			generateAST(ptr->forLoop->adv,0);
			printf(" \n");
			tabs++;
			generateAST(ptr->forLoop->stmtList,tabs);
			tabs--;
			break;
		}
		case typeFCall:
		{
			TABS
			printf("FCALL ");
			node *p=ptr->fCall->params;
			while(NULL != p)
			{
				generateAST(p,0);
				p=p->next;
			}
			break;
		}
		case typeConstant:
		{
			TABS
			printf("NUM ");
			break;
		}
		case typeOperator:
		{
			if(0 == strcmp(ptr->op->sym,"NOT"))
			{
				printf("NOT ");
				generateAST(ptr->op->child[0],0);
			}
			else if(0 == strcmp(ptr->op->sym,"+"))
			{
				printf("PLUS ");
				generateAST(ptr->op->child[0],0);
				generateAST(ptr->op->child[1],0);
			}
			else if(0 == strcmp(ptr->op->sym,"-"))
			{
				printf("MINUS ");
				generateAST(ptr->op->child[0],0);
				generateAST(ptr->op->child[1],0);
			}
			else if(0 == strcmp(ptr->op->sym,"*"))
			{
				printf("MUL ");
				generateAST(ptr->op->child[0],0);
				generateAST(ptr->op->child[1],0);
			}
			else if(0 == strcmp(ptr->op->sym,"/"))
			{
				printf("DIV ");
				generateAST(ptr->op->child[0],0);
				generateAST(ptr->op->child[1],0);
			}
			else if(0 == strcmp(ptr->op->sym,"%"))
			{
				printf("MOD ");
				generateAST(ptr->op->child[0],0);
				generateAST(ptr->op->child[1],0);
			}
			else if(0 == strcmp(ptr->op->sym,"<"))
			{
				printf("LESSTHAN ");
				generateAST(ptr->op->child[0],0);
				generateAST(ptr->op->child[1],0);
			}
			else if(0 == strcmp(ptr->op->sym,">"))
			{
				printf("GREATERTHAN ");
				generateAST(ptr->op->child[0],0);
				generateAST(ptr->op->child[1],0);
			}
			else if(0 == strcmp(ptr->op->sym,">="))
			{
				printf("GREATERTHANOREQUAL ");
				generateAST(ptr->op->child[0],0);
				generateAST(ptr->op->child[1],0);
			}
			else if(0 == strcmp(ptr->op->sym,"<="))
			{
				printf("LESSTHANOREQUAL ");
				generateAST(ptr->op->child[0],0);
				generateAST(ptr->op->child[1],0);
			}
			else if(0 == strcmp(ptr->op->sym,"!="))
			{
				printf("NOTEQUAL ");
				generateAST(ptr->op->child[0],0);
				generateAST(ptr->op->child[1],0);
			}
			else if(0 == strcmp(ptr->op->sym,"=="))
			{
				printf("EQUALEQUAL ");
				generateAST(ptr->op->child[0],0);
				generateAST(ptr->op->child[1],0);
			}
			else if(0 == strcmp(ptr->op->sym,"AND"))
			{
				printf("AND ");
				generateAST(ptr->op->child[0],0);
				generateAST(ptr->op->child[1],0);
			}
			else if(0 == strcmp(ptr->op->sym,"OR"))
			{
				printf("OR ");
				generateAST(ptr->op->child[0],0);
				generateAST(ptr->op->child[1],0);
			}
			break;
		}
		case typeStr:
		{
			node *p=ptr->str->head;
			int space=0;
			while(NULL != p)
			{
				if(space) printf(" ");
				space=1;
				printf("VAR");
				p=p->next;
			}
			break;
		}
		case typeVariable:
		{
			TABS
			if(0 != ptr->var->dimCount)
			{
				printf("ARRREF VAR");
				node* p=ptr->var->dimListHead;
				while(NULL != p)
				{
					printf(" ");
					generateAST(p,0);
					printf(" ");
					p=p->next;
				}
			}
			else
			{
				printf("VAR ");
			}
			break;
		}
	}
}


void generateCode(node* ptr,int tabs)
{
	if(NULL == ptr)
	{
		return;
	}
	switch(ptr->type)
	{
		case typeGDeclSec:
		{
			printf("#include <stdio.h>\n#include <stdbool.h>\n");
			node *p=ptr->gDeclSec->gDeclHead;
			while(NULL != p)
			{
				generateCode(p,tabs);
				printf("\n");
				p=p->next;
			}
			generateCode(ptr->next,tabs);
			break;
		}
		case typeGDecl:
		{
			if(typeInt == ptr->gDecl->type)
			{
				printf("int ");
			}
			else
			{
				printf("bool ");
			}
			node *p=ptr->gDecl->gListHead;
			int comma=0;
			while(NULL != p)
			{
				if(comma)
				{
					printf(",");
				}
				comma=1;
				generateCode(p,tabs);
				p=p->next;
			}
			printf(";");
			break;
		}
		case typeGIdDecl:
		{
			printf("%s",ptr->gIdDecl->name);
			dimListNode *p=ptr->gIdDecl->head;
			while(NULL != p)
			{
				printf("[%d]",p->dim);
				p=p->next;
			}
			break;
		}
		case typeFuncDecl:
		{
			printf("%s(",ptr->funcDecl->name);
			generateCode(ptr->funcDecl->argListHead,0);
			printf(")");
			break;
		}
		case typeArg:
		{
			node *p=ptr->arg->head;
			int comma=0;
			while(NULL != p)
			{
				node *vlp=p->arg->varListHead;
				while(NULL != vlp)
				{
					if(comma)
					{
						printf(",");
					}
					comma=1;
					printf("%s %s",(ptr->arg->type==typeInt)?"int":"bool",vlp->varList->name);
					vlp=vlp->next;
				}
				p=p->next;	
			}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
			break;
		}
		case typeFDefSec:
		{
			node *p=ptr->fDefSec->head;
			while(NULL != p)
			{
				generateCode(p,tabs);
				p=p->next;
			}
			generateCode(ptr->next,tabs);
			break;
		}
		case typeFDef:
		{
			printf("%s %s(",(ptr->fDef->retType==typeInt)?"int":"bool",ptr->fDef->name);
			generateCode(ptr->fDef->argList,0);
			printf(")\n");
			printf("{\n");
			tabs++;
			generateCode(ptr->fDef->lDeclSec,tabs);
			generateCode(ptr->fDef->stmtList,tabs);
			TABS
			printf("return ");
			generateCode(ptr->fDef->retStmt,0);
			printf(";\n");
			tabs--;
			printf("}\n");
			break;
		}
		case typeMainBlock:
		{
			printf("%s main()\n",(ptr->mainBlock->retType)?"int":"bool");
			printf("{\n");
			tabs++;
			generateCode(ptr->mainBlock->lDeclSec,tabs);
			generateCode(ptr->mainBlock->stmtList,tabs);
			TABS
			printf("return ");
			generateCode(ptr->mainBlock->retStmt,0);
			printf(";\n");
			tabs--;
			TABS
			printf("}\n");
			break;
		}
		case typeLDecl:
		{
			node *vlp=ptr->lDecl->varListHead;
			while(NULL != vlp)
			{
				TABS
				printf("%s %s;\n",(ptr->lDecl->type==typeInt)?"int":"bool",vlp->varList->name);
				vlp=vlp->next;
			}
			generateCode(ptr->next,tabs);
			break;
		}
		case typeStmt:
		{
			generateCode(ptr->stmt->stmt,tabs);
			if(NULL == ptr->stmt->stmt){ break; }
			switch(ptr->stmt->stmt->type)
			{
				case typeForLoop: break;
				case typeWhileLoop: break;
				case typeIfElse: break;
				default: printf(";\n");
			}
			generateCode(ptr->next,tabs);
			break;
		}
		case typeRead:
		{
			TABS
			printf("scanf(\"%%d\",&");
			generateCode(ptr->read->var,0);
			printf(")");
			break;
		}
		case typeWrite:
		{
			TABS
			printf("printf(\"");
			if(ptr->write->data->type == typeStr)
			{
				generateCode(ptr->write->data,0);
				printf("\"");	
			}
			else
			{
				printf("%%d\",");
				generateCode(ptr->write->data,0);
			}
			printf(")");
			break;
		}
		case typeAssignment:
		{
			generateCode(ptr->assignment->id,tabs);
			printf("=");
			generateCode(ptr->assignment->expr,0);
			break;
		}
		case typeIfElse:
		{
			TABS
			printf("if(");
			generateCode(ptr->ifElse->cond,0);
			printf(")\n");
			TABS
			printf("{\n");
			tabs++;
			generateCode(ptr->ifElse->iStmtList,tabs);
			tabs--;
			TABS
			printf("}\n");
			
			if(NULL != ptr->ifElse->eStmtList)
			{
				TABS
				printf("else\n");
				TABS
				printf("{\n");
				tabs++;
				generateCode(ptr->ifElse->eStmtList,tabs);
				tabs--;
				TABS
				printf("}\n");
			}
			break;
		}
		case typeWhileLoop:
		{
			TABS
			printf("while(");
			generateCode(ptr->whileLoop->cond,0);
			printf(")\n");
			TABS
			printf("{\n");
			tabs++;
			generateCode(ptr->whileLoop->stmtList,tabs);
			tabs--;
			TABS
			printf("}\n");
			break;
		}
		case typeForLoop:
		{
			TABS
			printf("for(");
			generateCode(ptr->forLoop->init,0);
			printf(";");
			generateCode(ptr->forLoop->cond,0);
			printf(";");
			generateCode(ptr->forLoop->adv,0);
			printf(")");
			TABS
			printf("{\n");
			tabs++;
			generateCode(ptr->forLoop->stmtList,tabs);
			tabs--;
			TABS
			printf("}\n");
			break;
		}
		case typeFCall:
		{
			TABS
			printf("%s(",ptr->fCall->id);
			node *p=ptr->fCall->params;
			int comma=0;
			while(NULL != p)
			{
				if(comma) printf(",");
				comma=1;
				generateCode(p,0);
				p=p->next;
			}
			printf(")");
			break;
		}
		case typeConstant:
		{
			TABS
			printf("%d",ptr->con->value);
			break;
		}
		case typeOperator:
		{
			printf("(");
			if(0 == strcmp(ptr->op->sym,"NOT"))
			{
				TABS
				printf("!");
				generateCode(ptr->op->child[0],0);
			}
			else if(0 == strcmp(ptr->op->sym,"AND"))
			{
				generateCode(ptr->op->child[0],tabs);
				printf("&&");
				generateCode(ptr->op->child[1],0);
			}
			else if(0 == strcmp(ptr->op->sym,"OR"))
			{
				generateCode(ptr->op->child[0],tabs);
				printf("||");
				generateCode(ptr->op->child[1],0);
			}
			else
			{
				generateCode(ptr->op->child[0],tabs);
				printf("%s",ptr->op->sym);
				generateCode(ptr->op->child[1],0);
			}
			printf(")");
			break;
		}
		case typeStr:
		{
			node *p=ptr->str->head;
			int space=0;
			while(NULL != p)
			{
				if(space) printf(" ");
				space=1;
				printf("%s",p->str->str);
				p=p->next;
			}
			break;
		}
		case typeVariable:
		{
			TABS
			printf("%s",ptr->var->variableName);
			if(0 != ptr->var->dimCount)
			{
				node* p=ptr->var->dimListHead;
				while(NULL != p)
				{
					printf("[");
					generateCode(p,0);
					printf("]");
					p=p->next;
				}
			}
			break;
		}
	}
}

node* addDimension(node *p,node* expr)
{
	p->var->dimCount++;
	if(NULL == p->var->dimListHead)
	{
		p->var->dimListHead=expr;
		p->var->dimListTail=expr;
	}
	else
	{
		p->var->dimListTail->next=expr;
		p->var->dimListTail=expr;
	}
	return p;
}

node* addDimGid(node *p,int n)
{
	ALLOC(dimListNode,newNode);
	newNode->dim=n;
	newNode->next=NULL;
	if(NULL == p->gIdDecl->head)
	{
		p->gIdDecl->head=newNode;
	}
	else
	{
		p->gIdDecl->tail->next=newNode;
	}
	p->gIdDecl->tail=newNode;
	return p;
}
