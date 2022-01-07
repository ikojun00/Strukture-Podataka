#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Tree;
typedef struct Tree * Position;
typedef struct Tree {
    int el;
    Position L;
    Position R;
}BinaryTree;

Position Insert(int x, Position s);
int PrintInOrder(Position current);
int PrintPreOrder(Position current);
int PrintPostOrder(Position current);
Position DeleteElement(int x, Position s);
Position FindMin(Position s);
Position FindElement(int x, Position s);
int Menu(Position root);


int main()
{
    Position root = NULL;
    Menu(root);
    return 0;
}

Position Insert(int x, Position s)
{
    if(NULL == s)
    {
        s = (Position)malloc(sizeof(BinaryTree));
        s->el = x;
        s->L = NULL;
        s->R = NULL; 
    }
    else if(x < s->el)
    {
        s->L = Insert(x, s->L);
    }
    else if(x > s->el)
    {
        s->R = Insert(x, s->R);
    }

    return s;
}


int PrintInOrder(Position current)
{
	if(NULL == current) 
		return 0; 

	PrintInOrder(current->L);

	printf("%d ", current->el);

	PrintInOrder(current->R);

	return 0;
}


int PrintPreOrder(Position current)
{
	if(NULL == current) 
		return 0; 
    
	printf("%d ", current->el);

	PrintInOrder(current->L);
	PrintInOrder(current->R);

	return 0;
}


int PrintPostOrder(Position current)
{
	if(NULL == current) 
		return 0; 
    
    PrintInOrder(current->L);
	PrintInOrder(current->R);

	printf("%d ", current->el);

	return 0;
}

Position DeleteElement(int x, Position s)
{
    Position temp = NULL;
    if(NULL == s)
    {
        printf("No such element in the binary tree.");
    }
    else if(x < s->el)
    {
        s->L = DeleteElement(x, s->L);
    }
    else if(x > s->el)
    {
        s->R = DeleteElement(x, s->R);
    }
    else if(s->L != NULL && s->R != NULL)//ima dvoje djece
    {
        temp= FindMin(s->R);
        s->el = temp->el;
        s->R = DeleteElement(s->el, s->R);
    }
    else //ima jedno djete
    {
        temp= s;
        if(NULL == s->L)
            s= s->R;
        else
            s = s->L;
        free(temp);
    }
    return s;
}

Position FindMin(Position s)
{
    if(NULL == s)
        return NULL;
    else if(NULL == s->L)
        return s;
    else
        return FindMin(s->L);
}


Position FindElement(int x, Position s){
    if (s == NULL)
        return NULL;
    else if(x < s->el)
        return FindElement(x, s->L);
    else if(x > s->el)
        return FindElement(x, s->R);
    else
        return s; 
}


int Menu(Position root)
{
    int n, i, choice;
    Position pos = NULL;

    printf("\n1 - Insert a number\n"
    "2 - Delete a number from the tree\n"
    "3 - Find a number\n"
    "4 - Print inorder\n"
    "5 - Print preorder\n"
    "6 - Print postorder\n"
    "0 - Exit\n"
    );
    printf("Your choice: ");
    scanf("%d", &choice);
    printf("\n");

    switch (choice)
    {
    case 1:
        printf("Number to insert: ");
		scanf("%d", &n);
        root = Insert(n, root);
        Menu(root);
        break;
    case 2:
        printf("Number to delete :");
        scanf("%d", &n);
        root = DeleteElement(n, root);
        Menu(root);
        break;
    case 3:
        printf("Number to find:");
        scanf("%d", &n);
        if(FindElement(n, root) !=  NULL)
            printf("Element %d was found!\n", n );
        else
            printf("Element %d not found!\n", n);
        Menu(root);
        break;
    case 4:  
        printf("In order: ");
        PrintInOrder(root);
        Menu(root);
        break;
    case 5:  
        printf("Pre order: ");
        PrintPreOrder(root);
        Menu(root);
        break;
    case 6:  
        printf("Post order: ");
        PrintPostOrder(root);
        Menu(root);
        break;
    case 0:
        return 0;
    default:
        Menu(root);
        break;
    }
    return 1;
}