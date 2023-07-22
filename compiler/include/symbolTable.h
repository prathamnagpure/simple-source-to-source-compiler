typedef struct MapNode /* node for the symbol table which is implimented as a linked list */
{
    char* key;
    int data;
    int lineno;
    struct MapNode *next;
} mapNode;

mapNode* findlst(char *variableName); /* find if variable is present in symbol table */
int insertlst(char *variableName, int dim, int line); /* Insert variable in symbol table */
mapNode* findgst(char *variableName);
int insertgst(char *variableNode, int dim, int line);
void freelst();
int insertfst(char* variableName,int dim, int line);
mapNode* findfst(char *variableName);
extern mapNode *gst;
extern mapNode *lst;
extern mapNode *fst;
