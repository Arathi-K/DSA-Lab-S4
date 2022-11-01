#include<stdio.h>
#include<stdlib.h>
struct node {
    int key;
    int degree;
    struct node* parent;
    struct node* child;
    struct node* sibling;
};
struct BH
{
   struct node* head; 
};
struct node* CREATE_NODE(int k)
{
    struct node* new=(struct node*)malloc(sizeof(struct node));
    new->key=k;
    new->degree=0;
    new->parent=NULL;
    new->child=NULL;
    new->sibling=NULL;
    return new;
}
struct BH* Make_heap(struct BH* H)
{
	H=(struct BH*)malloc(sizeof(struct BH));
	H->head=NULL;
	return H;
}
void BINOMIAL_LINK(struct node* y,struct node* z)
{
	y->parent=z;
	y->sibling=z->child;
	z->child=y;
	z->degree++;
}
int degree(struct node* x)
{
    if(x==NULL)
        return -1;
    else
        return x->degree;
}
void LevelOrder(struct node * root)
{
    printf("%d ",root->key);
    struct node* start=root->child;
    while(start)
    {
        struct node* present=start;
        while(present)
        {
            printf("%d ",present->key);
            present=present->sibling;
        }
        start=start->child;
    }
}
struct BH* Binomial_heap_merge(struct BH* H1,struct BH* H2)
{
    struct BH* H;
	H=Make_heap(H);
	// printf("degree of H1=%d,Degree of H2=%d\n",degree(H1->head),degree(H2->head));
	if(degree(H1->head)<=degree(H2->head))
	{
	    if(H1->head!=NULL)
	    { 
	       H->head=H1->head;
	       H1->head=H1->head->sibling;
	    }
	    else
	    {
	        H->head=H2->head;
	        H2->head=H2->head->sibling;
	    }
	}
	else 
	{
	    if(H2->head!=NULL)
	    {
	        H->head=H2->head;
	        H2->head=H2->head->sibling;
	    }
	    else
	    {
	        H->head=H1->head;
	       H1->head=H1->head->sibling;
	    }
	    
	}
	struct node*present=H->head;
	while(degree(H1->head)!=-1 || degree(H2->head)!=-1)
	{
	    // printf("degree of H1=%d,Degree of H2=%d\n",degree(H1->head),degree(H2->head));
	    if(degree(H1->head)<=degree(H2->head))
    	{
	         if(H1->head!=NULL)
	        { 
	              present->sibling=H1->head;
	              H1->head=H1->head->sibling;
	        }
	        else
	        {
	              present->sibling=H2->head;
	              H2->head=H2->head->sibling;
	        }
	    }
    	else 
	    {
	        if(H2->head!=NULL)
	        {
	            present->sibling=H2->head;
	            H2->head=H2->head->sibling;
	        }
	        else
	        {
	            present->sibling=H1->head;
	            H1->head=H1->head->sibling;
	        }
	    
	    }
	      present=present->sibling;
	}
    return H;
}

struct BH* Binomial_heap_union(struct BH* H1,struct BH* H2)
{
    // printf("H1->head is: %d \n",H1->head->key);
    // printf("H2->head is: %d \n",H2->head->key);
	struct BH* H;
	H=Make_heap(H);
	H=Binomial_heap_merge(H1,H2);
	// printf("H after merging:\n");
	//             struct node* present=H->head;
	// 	        while(present)
	// 	         {
	// 	             LevelOrder(present);
	// 	             present=present->sibling;
	// 	         }
	// 	         printf("\n");


	if (H->head==NULL)
		return H;
	struct node* previous=NULL;
	struct node* x=H->head;
	struct node* next=H->head->sibling;
	while(next!=NULL)
	{
		// printf("x->degree=%d,next->degree=%d,degree(next->sibling)=%d\n",x->degree,next->degree,degree(next->sibling));
		if((degree(x)!=degree(next))||(next->sibling!=NULL && x->degree==next->sibling->degree)) //cases 1 and 2
		{
			previous=x;
			x=x->sibling;
			next=x->sibling;
		}
		else//case 3 and 4
		{
		 if(x->key<=next->key)
		 {
			x->sibling=next->sibling;
			BINOMIAL_LINK(next,x);
		 }
		 else
		 {
		 	if(previous==NULL)
		 		H->head=next;
		 	else
		 		previous->sibling=next;
		 	BINOMIAL_LINK(x,next);
		 	x=next;
		 }
		 next=x->sibling;
		}
	}
	return H;
}

struct node* MINIMUM(struct BH* H)
{
	struct node* min_node;
	struct node* x=H->head;
	min_node=x;
	x=x->sibling;
	while(x!=NULL)
	{
		if(x->key<min_node->key)
			min_node=x;
		x=x->sibling;
	}
	return min_node;
}
struct BH* Decrease_key(struct BH* H,struct node* x,int k)
{
	
		x->key=k;
		struct node* y=x;
		struct node* z=x->parent;
		while(z!=NULL && y->key<z->key)
		{
			int temp=y->key;
			y->key=z->key;
			z->key=temp;
			y=z;
			z=y->parent;
		}
	
	return H;
}
struct BH* Extract_min(struct BH* H)
{
	if(H->head==NULL)
		return H;
	// printf("H is not NULL\n");
	struct node* min_node=H->head;
	struct node* x=H->head->sibling;
	
	if(x==NULL)//i.e.,only one root is present
	{
	    // printf("only one root is present\n");
		struct node* present=min_node->child;
		if(present==NULL)
		{
		    // printf("no children,so became empty now\n");
			H->head=NULL;
			free(min_node);
			return H;
		}
		else    //children present
		{
			struct BH* H2;
			H2=Make_heap(H2);
			struct node* previous=min_node->child;
                                			// printf("previous=%d\n",previous->key);
                                			// printf("present=%d\n",present->key);
                                	    	// printf("min_node=%d\n",min_node->key);
                                		    // printf("min_node->child=%d\n",min_node->child->key);
			while(min_node->child!=NULL)
			{
			    // printf("min_node->child is not null,it is %d \n",min_node->child->key);
				while(present->sibling!=NULL)
				{
				    // printf("present->sibling!=NULL,it is %d \n",present->sibling->key);
				    previous=present;
				    present=present->sibling;
				}
				if(present==min_node->child)
				{
				    if(H2->head==NULL)
					     H2->head=present;
					 else
				     {
					    present->sibling=H2->head;
					    H2->head=present;
				     }
				    //  present->parent=NULL;
					 min_node->child=NULL;
				// 	 free(min_node);
					 
				}
				else
				{
			    	previous->sibling=NULL;
			    	//present->parent=NULL;
				    if(H2->head==NULL)
				    	H2->head=present;
    				else
    				{
    					present->sibling=H2->head;
    					H2->head=present;
    				}
		    	}
			}
		return H2;
		}
	}
	else
	{
	    // printf("not only one root in the  toot lst\n");
		struct node* mins_prev;
		struct node* xs_prev=H->head;
		while(x!=NULL)
		{
			if(min_node->key>x->key)
			{
				mins_prev=xs_prev;
				min_node=x;
				xs_prev=x;
				x=x->sibling;
			}
		}
		// printf("min_node is %d\n",min_node->key);
		if(min_node==H->head)
			H->head=min_node->sibling;
		else
			mins_prev->sibling=min_node->sibling;
			
	// 		printf("Level order of H:");
	// 		struct node* present=H->head;
	// 		        while(present)
	// 		         {
	// 		             LevelOrder(present);
	// 		             present=present->sibling;
	// 		         }
	// 		         printf("\n");
		
			struct BH* H2;
			H2=Make_heap(H2);
			struct node* present=min_node->child;
			if(present==NULL)
			{
				free(min_node);
				return H;
			}
			struct node* previous=min_node->child;
			while(min_node->child!=NULL)
			{
				while(present->sibling!=NULL)
				{
				previous=present;
				present=present->sibling;
				}
                if(present==min_node->child)
				{
				    if(H2->head==NULL)
					     H2->head=present;
					 else
				     {
					    present->sibling=H2->head;
					    H2->head=present;
				     }
				    //  present->parent=NULL;
					 min_node->child=NULL;
				// 	 free(min_node);
					 
				}
				else
				{
			    	previous->sibling=NULL;
			    //	present->parent=NULL;
				    if(H2->head==NULL)
				    	H2->head=present;
				    else
			    	{
					    present->sibling=H2->head;
					    H2->head=present;
				    }
			    } 
			}
		H=Binomial_heap_union(H,H2);
	}
	return H;
		
}
struct BH* Delete(struct BH* H,struct node* x)
{
	H=Decrease_key(H,x,-1000001);
	H=Extract_min(H);
	return H;
}
struct node* Search(struct BH* H,int k)
{
	struct node* present=H->head;
	struct node* x;
	while(present!=NULL)
	{
		if(k>=present->key)
		{
			if(k==present->key)
				return present;
			while(present->child!=NULL)
			{
				present=present->child;
				while(present!=NULL)
				{
				if(k==present->key)
					return present;
				else
					present=present->sibling;
				}
			}
		}
		present=present->sibling;
	}
	return NULL;
}

struct BH* INSERT(struct BH* H,struct node* new)
{
	struct BH* H2;
	H2=Make_heap(H2);
	H2->head=new;
	// printf("H was:\n");
	// struct node* present=H->head;
	// 	        while(present)
	// 	         {
	// 	             LevelOrder(present);
	// 	             present=present->sibling;
	// 	         }
	// 	         printf("\n");
	if(H->head)
	    H2=Binomial_heap_union(H,H2);
	//   printf("H->head in the function=%d\n",H2->head->key);
	return H2;
}
void main()
{
	char ch;
	int k,y,z;
	struct node* present;
	struct BH* H;
	H=Make_heap(H);
	while(1)
	{
		scanf(" %c",&ch);
		switch(ch)
		{
    	case 'i':scanf("%d",&k);
    	     present=CREATE_NODE(k);
    	    //  printf("inserted %d?\n",present->key);
			 H=INSERT(H,present);
			//   printf("H->head in main=%d\n",H->head->key);
			 break;
		case 'm':printf("%d\n",MINIMUM(H)->key);
			  break;
		case 'x':printf("%d\n",MINIMUM(H)->key);
		H=Extract_min(H);
			 break;
		case 'p':present=H->head;
		        while(present)
		         {
		             LevelOrder(present);
		             present=present->sibling;
		         }
		         printf("\n");
		        break;
		case 'r':scanf("%d",&y);
			 scanf("%d",&z);
			 if(y<z)
			    printf("-1\n");
			 else
			 {
			 present=Search(H,y);
			 printf("%d\n",y-z);
			 H=Decrease_key(H,present,y-z);
			 }
			 break;
		case 'd':scanf("%d",&k);
			 printf("%d",k);
			 present=Search(H,k);
		         H=Delete(H,present);
			 break;
		case 'e':exit(0);
		}
	}
}