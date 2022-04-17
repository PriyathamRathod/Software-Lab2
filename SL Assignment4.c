//BT20CSE100 Priyatham Rathod - SL Assignment 4.

#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>

typedef struct node
{
    int data;
    bool mark;
    int refCount;
    struct node *next1;
    struct node *next2;
    struct node *next3;    
}Node;

 Node *ptrArr[8];

void displayNode(int i)
{
	printf("value=%d\t reference_count=%d freed_size=%d\n",ptrArr[i]->data,ptrArr[i]->refCount,sizeof(Node));
}

void setEdge(int so,int destination1,int destination2,int destination3)
{
	if(destination1!=-1)
	{
		ptrArr[so]->next1=ptrArr[destination1];
		ptrArr[destination1]->refCount+=1;
	}
	if(destination2!=-1)
	{
		ptrArr[so]->next2=ptrArr[destination2];
		ptrArr[destination2]->refCount+=1;
	}
	if(destination3!=-1)
	{
		ptrArr[so]->next3=ptrArr[destination3];
		ptrArr[destination3]->refCount+=1;
	}
	
}

void displayAllNodes(Node* root)
{
	if(root!=NULL)
	{
		printf("value=%d:refCount=%d\n",root->data,root->refCount);
	}
	if(root==NULL)
	{
		return;
	}
	displayAllNodes(root->next1);
	displayAllNodes(root->next2);
	displayAllNodes(root->next3);
}

void adjacencyList()
{
	int i=0;
	for(i=0;i<8;i++)
	{
		if(ptrArr[i]!=NULL)
		{
			printf("Value=%d: ",ptrArr[i]->data);
			if(ptrArr[i]->next1!=NULL)
			{
				printf("%d ->",ptrArr[i]->next1->data);
			}
			if(ptrArr[i]->next2!=NULL)
			{
				printf("%d ->",ptrArr[i]->next2->data);
			}
			if(ptrArr[i]->next3!=NULL)
			{
				printf("%d ->",ptrArr[i]->next3->data);
			}
			printf("NULL\n");
		}
	}
}

int rootIsPresent(Node* root1,Node* temp)
{
	if(root1==NULL)
	{
		return 0;
	}
	if(root1->data==temp->data)
	{
		return 1;
	}
	
	if(rootIsPresent(root1->next1,temp))
	{
		return 1;
	}
	
	if(rootIsPresent(root1->next2,temp))
	{
		return 1;
	}
	if(rootIsPresent(root1->next3,temp))
	{
		return 1;
	}
 return 0;
}


void garbageCollectionrefCounting(Node* root)
{
	int i=0;
	while(i<8)
	{
		if(rootIsPresent(root,ptrArr[i])==0 )
		{		
			if(ptrArr[i]->next1!=NULL)
			{
				ptrArr[i]->next1->refCount-=1;
			}
			if(ptrArr[i]->next2!=NULL)
			{
				ptrArr[i]->next2->refCount-=1;
			}
			if(ptrArr[i]->next3!=NULL)
			{
				ptrArr[i]->next3->refCount-=1;
			}
			printf("Garbage:");
			displayNode(i);
			free(ptrArr[i]);
			ptrArr[i]=NULL;
		}
		i++;		
	}
	 
}

void adjacencyMatrix()
{
	int adm[8][8];
	int i,j,k;
	
	for(i=0;i<8;i++)		
	{
		for(j=0;j<8;j++)
		{
			adm[i][j]=0;
		}	
	}
	
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			
		if(ptrArr[j]!=NULL&&ptrArr[i]!=NULL)
		{
			
			if(ptrArr[i]->next1!=NULL)
			{
				if(ptrArr[i]->next1->data==ptrArr[j]->data&&i!=j)
				{
					adm[i][j]=1;
				}
			}
			if(ptrArr[i]->next2!=NULL)
			{
				if(ptrArr[i]->next2->data==ptrArr[j]->data&&i!=j)
				{
					adm[i][j]=1;
				}
			}
			if(ptrArr[i]->next3!=NULL)
			{
				if(ptrArr[i]->next3->data==ptrArr[j]->data&&i!=j)
				{
					adm[i][j]=1;
				}
			}
		}
		
		}
	}
	
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			printf("%d ",adm[i][j]);		
		}
		printf("\n");
	}
}

void markTheNodes(Node*root,int i,int j)
{
    Node *current, *previous;

    current = root;
        
    while (current != NULL) 
    {
  
        if (current->next1== NULL) 
        {
            current->mark=true;
            current = current->next2;
        }   
        else 
        {
            previous = current->next1;
            while ((previous->next2 != NULL) && (previous->next2 != current))
            {
            	previous = previous->next2;
			}
                
            if (previous->next2 == NULL) 
            {
                previous->next2 = current;
                current = current->next1;
            }
            else 
            {
                previous->next2 = NULL;
                current->mark=true;
                current = current->next2;
            } 
        }
    }   
    
    current = root;
        
    while (current != NULL) 
    {
  
        if (current->next1== NULL) 
        {
            current->mark=true;
            current = current->next3;
        }   
        else 
        {
            previous = current->next1;
            while ((previous->next3 != NULL) && (previous->next3 != current))
            {
            	previous = previous->next3;
			}
                
            if (previous->next3 == NULL) 
            {
                previous->next3 = current;
                current = current->next1;
            }
            else 
            {
                previous->next3 = NULL;
                current->mark=true;
                current = current->next3;
            } 
        }
    }  
    
}

void markMethod(Node* root)
{
	
	if(root!=NULL)
	{
		root->mark=true;
	}
	if(root==NULL)
	{
		return;
	}
	markMethod(root->next1);
	markMethod(root->next2);
	markMethod(root->next3);
}

void sweepMethod()
{
	int i;
	for(i=0;i<8;i++)
	{
		if(ptrArr[i]->mark==false)
		{
			if(ptrArr[i]->next1!=NULL)
			{
				ptrArr[i]->next1->refCount-=1;
			}
			if(ptrArr[i]->next2!=NULL)
			{
				ptrArr[i]->next2->refCount-=1;
			}
			if(ptrArr[i]->next3!=NULL)
			{
				ptrArr[i]->next3->refCount-=1;
			}
			printf("Garbage :");
			displayNode(i);
			free(ptrArr[i]);
			ptrArr[i]=NULL;
		}
	}
}

int main()
{		
	int val[]={1,2,3,4,5,7,8};
	
	
	int i;
	
	for( i=0;i<8;i++)
	{
		Node* newNode =(Node*)malloc(sizeof(Node));
		newNode->data=val[i];
		newNode->next1=NULL;
		newNode->next2=NULL;
		newNode->next3=NULL;
		newNode->refCount=0;
		newNode->mark=false;		
		ptrArr[i]=newNode;
	}
	
	Node*root1=ptrArr[3];
	ptrArr[3]->refCount+=1;
	Node*root2=ptrArr[0];
	ptrArr[0]->refCount+=1;
	
	setEdge(0,1,6,7);
	setEdge(2,5,7,-1);
	setEdge(3,0,-1,-1);
	setEdge(4,0,5,-1);
	setEdge(5,6,-1,-1);
	
	printf("Root of value 3:");
	displayNode(2);
	
	printf("\nAll the nodes through Root-1:\n");
	displayAllNodes(root1);
	
		
	printf("\nAll the nodes through Root-2:\n");
	displayAllNodes(root2);
	
	printf("\n\nDisplaying Adjacency list of noeds with their values and vertex:\n");
	adjacencyList();
	
	printf("\n\nDisplaying Adjacency matrix of the nodes:\n");
	adjacencyMatrix();
	

	
	printf("\nCalling mark sweep garbage collector!\n");
	                                                                //markTheNodes(root1,0,1);
	markMethod(root1);
	sweepMethod();

	
	printf("\n\nAdjacency list after removal of garbage:\n");
	adjacencyList();
	
	printf("\n\nAdjacency matrix after removal of garbage:\n");
	adjacencyMatrix();

	return 0;	
}
