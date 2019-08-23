#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/fb.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <dirent.h>

typedef struct node 
{
	char *name;
	char *mem;
	int time;
	char* (*display)(char *s,int x,int y);
	struct node *next;
	struct node *prev;
}linklist;

linklist *creatlist()
{
	linklist *head=(linklist *)malloc(sizeof(linklist));
	if(head!=NULL)
	{
		head->next=head;
		head->prev=head;
	}
	return head;
}

linklist *creatnode(char *name)
{
	linklist *new=(linklist *)malloc(sizeof(linklist));
	if(new!=NULL)
	{
		new->name=name;
		new->mem=NULL;
		new->time=-1;
		new->display=NULL;
		new->next=new;
		new->prev=new;
	}
	return new;
}

void insert(linklist *head,linklist *new)
{
	linklist *p=head;
	while(p->next!=head)
	{
		p=p->next;
	}
	new->next=p->next;
	new->next->prev=new;
	p->next=new;
	new->prev=p;
}
