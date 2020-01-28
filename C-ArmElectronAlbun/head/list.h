typedef struct node
{
	char *name;
	char *mem;
	int time;
	char* (*display)(char *s,int x,int y);
	struct node *next;
	struct node *prev;
}linklist;

linklist *creatlist();
linklist *creatnode(char *name);
void insert(linklist *head,linklist *new);
