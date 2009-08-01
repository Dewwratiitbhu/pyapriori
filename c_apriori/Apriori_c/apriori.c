#include <stdio.h>
#include <string.h>

#include "apriori.h"
#include "linked_list.h"


extern void* _test_malloc(const size_t size, const char* file, const int line);
extern void _test_free(void* const ptr, const char* file, const int line);
#define malloc(size) _test_malloc(size, __FILE__, __LINE__)
#define free(ptr) _test_free(ptr, __FILE__, __LINE__)


struct node* parser(char* file_name,int w_size, int d_wind)
{
	struct node* list_of_parses = NULL;
	// Open the file and get the file descriptor
	FILE* fp = read_file(file_name);
	if(fp == NULL)
	{
		return NULL;
	}
	
	// get the token list
	struct node* token_list = get_token_list(fp);
	
	if(d_wind == FALSE)
	{
		//get the windows w/o dynamic windowing
		list_of_parses = get_windows(token_list, w_size);
		
	}
	
	else
	{
		/* get the windows w/dynamic windowing */ 
		list_of_parses = get_dynamic_windows(token_list);
	}
	
	
	//sort the list
	list_of_parses = mergesort(list_of_parses,&compare_lists);
	
	/* find average window size and max window size */ 

	/*cleanup */ 
	free_list(&token_list);
	fclose(fp);

	return list_of_parses;

}




struct node* get_windows(struct node* token_list, int w_size)
{
 	int k = 0;
 	int token = 0;
 	int iter = 0;
 	int * temp = NULL;
 	struct node *parse_list = NULL;
 	struct node *list_of_parses = NULL;
 	for(k; k < get_len_list(token_list)-w_size + 1; k++)
 	{ 
 		for(token = k; token < k + w_size; token++)
 		{
 			temp = (int *)malloc(sizeof(int));
 			*temp = *((int *)get_data(token_list,token));
 			if(!add(&parse_list,(void *)temp))
 			{
 				printf("Error while reading from file");
 				exit(0);
 			}
 			
 			if(iter < w_size -1)
 				iter++;
 			else
 			{
 				add(&list_of_parses,(void *)parse_list);
 				parse_list = NULL;
 				iter = 0;
 					
 			}
 			
 		} 			
 	}
 
 	return list_of_parses;
  	
}

struct node* get_dynamic_windows(struct node* token_list)
{
	int *temp = NULL;
	struct node *parse_list = NULL;
 	struct node *list_of_parses = NULL;
	struct node* start_t = token_list;
	struct node* token = NULL;
	while(start_t != NULL)
	{
		temp = (int *)malloc(sizeof(int));
		*temp = *((int *)start_t->data);
		if(!add(&parse_list,(void *)temp))
		{
			printf("Error while reading from file");
			exit(0);
		}
		token = start_t->next;
		while(token != NULL)
		{
			if(*((int *)token->data) == *((int *)start_t->data))
			{
				parse_list = mergesort(parse_list,&compare_ints);
				add(&list_of_parses,(void *)parse_list);
				parse_list = NULL;
				break;
			}
			else
			{
				temp = (int *)malloc(sizeof(int));
				*temp = *((int *)token->data);
				if(!add(&parse_list,(void *)temp))
				{
					printf("Error while reading from file");
					exit(0);
				}
			}
			token = token->next;
		}
		if(parse_list != NULL || *((int *)token->data) != *((int *)start_t->data))
		{
			if(parse_list != NULL)
			{
				parse_list = mergesort(parse_list,&compare_ints);
				add(&list_of_parses,(void *)parse_list);
			}
			parse_list = NULL;
		}	
		start_t = start_t->next;
		
			
	}
	return list_of_parses;
	
}

struct node* get_token_list(FILE* fp)
{
	char *s;
	struct node *head = NULL;
	int *temp = NULL;
		
	while(!feof(fp))
	{
		
		if(fscanf(fp,"%as",&s) > 0)
		{	
			temp = malloc(sizeof(int));
			*temp = atoi(s);
			if(!add(&head,(void *) temp))
			{
				printf("Error while reading from file");
				exit(1);
			}
			
		}
		else
			break;
			
	}	
	
	return head;
	
}



FILE* read_file(char* file_name)
{
	
	
	FILE *fp;

	if((fp=fopen(file_name,"r"))==NULL)
	{
		printf("Cannot open file.\n");
		return NULL;
	}

	return fp;
		
	
}



