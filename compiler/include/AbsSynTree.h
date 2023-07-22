#include<stdarg.h>

typedef enum { typeConstant, typeVariable, typeOperator, typeMainBlock, typeGDeclSec,
               typeLDecl, typeFunction, typeAssignment, typeGDecl , typeGIdDecl, 
			   typeFuncDecl, typeArg, typeArgList, typeVarList, typeFDefSec, typeFDef,
			   typeRead, typeWrite, typeIfElse, typeWhileLoop, typeForLoop, 
			   typeFCall, typeStr, typeStmt } nodeType;

typedef enum { typeBool, typeInt } dataType;

typedef struct ConstantNode /* node for a constant number */
{
	dataType type;
	int value;
} constantNode;

typedef struct StmtNode
{
	struct Node* stmt;
} stmtNode;

typedef struct OperatorNode /* node for an operator */
{
	char sym[4];
	struct Node* child[2];
} operatorNode;


typedef struct ListNode
{
	union
	{
		int val;
		struct Node *ptr;
	};
} listNode;

typedef struct VariableNode /* node for a variable */
{
	char *variableName;
	int dimCount;
	struct Node *dimListHead;
	struct Node *dimListTail;
} variableNode;

typedef struct AssignmentNode /* node for assignment statement */
{
	struct Node* id;
	struct Node *expr;
} assignmentNode;

typedef struct FunctionNode /* node for function statement */
{
	char* name;
	struct Node* list;
} functionNode;

typedef struct DeclarationNode /* node for declaration statement */
{
	int type;
	struct Node* list;
} declarationNode;

typedef struct ReturnValue /* used in return of expression evaluation for undefined variables */
{
	int defined;
	int value;
} returnValue;

typedef struct MainBlockNode
{
	dataType retType;
	struct Node *lDeclSec;
	struct Node *stmtList;
	struct Node *retStmt;
} mainBlockNode;

typedef struct VarListNode
{
	char *name;
} varListNode;

typedef struct ArgNode
{
	dataType type;
	struct Node *head;
	struct Node *varListHead;
} argNode;

typedef struct FuncDeclNode
{
	char *name;
	struct Node *argListHead;
} funcDeclNode;

typedef struct DimListNode
{
	int dim;
	struct DimListNode *next;
} dimListNode;

typedef struct GIdDeclNode
{
	char *name;
	struct DimListNode *head;
	struct DimListNode *tail;
} gIdDeclNode;

typedef struct GDeclNode	
{
	dataType type;
	struct Node *gListHead;
} gDeclNode;

typedef struct GDeclSecNode
{
	struct Node *gDeclHead;
} gDeclSecNode;


typedef struct FDefNode
{
	dataType retType;
	char *name;
	struct Node* argList;
	struct Node* lDeclSec;
	struct Node* stmtList;
	struct Node* retStmt;
} fDefNode;

typedef struct FDefSecNode
{
	struct Node* head;
	struct Node* tail;
} fDefSecNode;


typedef struct FCallNode
{
	char* id;
	struct Node* params;
} fCallNode;

typedef struct LDeclNode
{
	dataType type;
	struct Node *varListHead;
} lDeclNode;

typedef struct WriteNode
{
	struct Node *data;
} writeNode;

typedef struct ReadNode
{
	struct Node *var;
} readNode;

typedef struct IfElseNode
{
	struct Node* cond;
	struct Node* iStmtList;
	struct Node* eStmtList;
} ifElseNode;

typedef struct WhileLoopNode
{
	struct Node* cond;
	struct Node* stmtList;
} whileLoopNode;

typedef struct ForLoopNode
{
	struct Node* init;
	struct Node* cond;
	struct Node* adv;
	struct Node* stmtList;
} forLoopNode;

typedef struct StrListNode
{
	struct Node* head;
	struct Node* tail;
} strListNode;


typedef struct StrNode
{
	char *str;
	struct Node *head;
} strNode;

typedef struct Node
{
	nodeType type;
	struct Node* next;
	int lineno;
	union
	{
		struct ConstantNode *con;
		struct VariableNode *var;
		struct OperatorNode *op;
		struct MainBlockNode *mainBlock;
		struct GDeclNode *gDecl;
		struct GDeclSecNode *gDeclSec;
		struct LDeclNode *lDecl;
		struct FunctionNode *fun;
		struct AssignmentNode *assignment;
		struct ReadNode *read;
		struct WriteNode *write;
		struct IfElseNode *ifElse;
		struct WhileLoopNode *whileLoop;
		struct ForLoopNode *forLoop;
		struct GIdDeclNode *gIdDecl;
		struct FuncDeclNode *funcDecl;
		struct ArgNode *arg;
		struct ArgListNode *argList;
		struct VarListNode *varList;
		struct FDefSecNode *fDefSec;
		struct FDefNode *fDef;
		struct FCallNode *fCall;
		struct StrListNode *strList;
		struct StrNode *str;
		struct StmtNode * stmt;
	};
} node;
int evaluateStmt(); /* evaluate the expression tree to get the value */
void red(); /* red color in terminal */
void reset(); /* reset color to normal */
node* createNode(int lineno, nodeType type, int num, ...);
node* addDimension(node *p,node* expr);
node* addDimGid(node *p,int n);
void generateCode(node*,int);
void generateAST(node*,int);
void checkCode(node*);
extern node* start;
extern int error;