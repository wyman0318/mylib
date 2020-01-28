#ifndef __LIST_H_
#define __LIST_H_

typedef struct node
{
	char *name;
	struct node *next;
	struct node *prev;
}linklist;

linklist *creatlist();
linklist *creatnode(char *name);
void insert(linklist *head,linklist *new);

#endif

