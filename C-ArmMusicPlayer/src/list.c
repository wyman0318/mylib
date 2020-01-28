#include "common.h"

typedef struct node 
{
	char *name;
	struct node *next;
	struct node *prev;
}linklist;

linklist *creatlist()
{
	linklist *head = (linklist *)malloc(sizeof(linklist));
	if(head!=NULL)
	{
		head->next = head;
		head->prev = head;
	}
	return head;
}

linklist *creatnode(char *name)
{
	linklist *new = (linklist *)malloc(sizeof(linklist));
	if(new!=NULL)
	{
		new->name = name;
		new->next = new;
		new->prev = new;
	}
	return new;
}

void insert(linklist *head,linklist *new)
{
	linklist *p = head;
	while(p->next != head)
	{
		p=p->next;
	}
	new->next = p->next;
	new->next->prev=new;
	p->next = new;
	new->prev = p;
}
