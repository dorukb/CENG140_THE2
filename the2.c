#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node { 
	int data;
	struct node *next;
};
struct message_node{
	int sender_id;
	int topic_id;
	int message_id;
	struct message_node *next;
};
struct topic_node{
	char *topic_name;
	int topic_id;
	struct node *follower_list; /* hold user ID's in data field */
	struct topic_node *next;
};
struct user_node{
	char *user_name;
	int user_id;
	struct node *followed_topics; /* hold topic ID's in data field */
	struct message_node *inbox;
	/* Add additional fields as needed */
	struct user_node *next_user;
	struct user_node **list;
};

void add_user		(struct user_node **,char*,int);
void make_userNode	(struct user_node* user,char *username,int id_count);
void subscribe_user (struct user_node **,struct topic_node **,char *,char *, int );
void make_topicNode (struct topic_node* topic,char *topicname,int topic_id,int user_id);
int  find_id		(struct user_node **user_list,char *username);
void users_topics	(struct user_node **user_list,int user_id,int t_id);
int  find_topic		(struct topic_node **topic_list,char* topicname);
void addMessage		(struct user_node **user_list,int ,int);
void printInbox		(struct user_node **user_list);
void unsub			(struct topic_node **topic_list, int topic_id, int user_id);
void del_sub		(struct user_node **user_list,int topics_id,int users_id);
void delete_user	(struct user_node **user_list, int user_id);
void delete_messages(struct user_node **user_list,int user_id);

int topic_id = 1;
int message_id = 1;

int main()
{
	char control;
	
	
	/* KEY VARIABLES */
	struct user_node **user_list = NULL;
	struct topic_node **topic_list = NULL;
	int id_count = 1;
	
	
	topic_list = (struct topic_node **)malloc(sizeof(struct topic_node*));
	user_list  = (struct user_node **) malloc(sizeof(struct user_node*));
	/* KEY VARIABLES */
	
	
	while((control = getchar()) != EOF)
	{
		char **input =(char **)malloc(sizeof(char*)*3);
		char c = control;
		int i = 0,n=0,size=1;
		int author_id;
		int users_id;
		int sent_topic;
		int unsub_topic;
		int id_to_del;
	
		input[0]=(char *)malloc(sizeof(char));
		

		/* GETTING THE INPUT AND ORGANIZING IT.*/
		do{
			if(c == ' ')
			{
				/* new part of the string*/
				input[n] = (char *)realloc(input[n],sizeof(char)*size);
				input[n][i] = '\0';
				
				n++;
				input[n]=(char *)malloc(sizeof(char));
				i = 0;
				size = 1;
			}
			
			 /* continue building the string*/
			else{
				input[n] = (char *)realloc(input[n],sizeof(char)*size);
				input[n][i] = c;
				size ++;
				i++;
			}		
		}
		while(((c = getchar()) != '\n') && (c!=EOF));
		
		input[n] = (char *)realloc(input[n],sizeof(char)*size);
		input[n][i] = '\0';
		
		/* GETTING THE INPUT AND ORGANIZING IT.*/
	
		
		/*  ADD OPERATION  */
		if(!strcmp(input[0],"ADD"))
		{
			add_user(user_list,input[1],id_count);
			id_count++;
		}
		
		/* SUBSCRIBE OPERATION */
		else if(!strcmp(input[0],"SUBSCRIBE"))
		{
			subscribe_user(user_list,topic_list,input[1],input[2],topic_id);	
		}
		
		
		/* SEND OPERATION -- SEND OPERATION -- SEND OPERATION */
		else if(!strcmp(input[0],"SEND"))
		{
			author_id = find_id(user_list,input[1]);
			sent_topic = find_topic(topic_list,input[2]);
			addMessage(user_list,author_id,sent_topic);
			
			message_id++;
			
		}
		
		/* -- UNSUBSCRIBE OPERATION -- */
		else if(!strcmp(input[0],"UNSUBSCRIBE"))
		{
			
			users_id = find_id(user_list,input[1]);
			unsub_topic = find_topic(topic_list,input[2]);
			
			unsub(topic_list,unsub_topic,users_id);
			del_sub(user_list,unsub_topic,users_id);
			
		}
		
		/* -- DELETEUSER OPERATION -- */
		else if(!strcmp(input[0],"DELETEUSER"))
		{
			
			id_to_del = find_id(user_list,input[1]);
			
			delete_user(user_list,id_to_del);
			
			delete_messages(user_list,id_to_del);
		
		}
		
	}
	
	printInbox(user_list);
	
	
	return 0;
}


void delete_user(struct user_node **user_list, int user_id)
{
	struct user_node *head = *user_list;
	struct user_node *curr = head;
	struct user_node *prev = NULL;
	struct user_node *tmp = NULL;
	
	
	while(curr) /*traverse the list!!*/
	{
		if(curr->user_id == user_id)/*right user to delete.*/
		{
			tmp = curr -> next_user;
			if(prev)/*he is not the first user in the list. nice case. */
			{
				free(curr);
				prev->next_user = tmp;
				return;
			}
			else /*he IS the first user.. must change the head.*/
			{
					free(curr);
					*user_list = tmp;
					return;
			}
		}
		prev = curr;
		curr = curr -> next_user;
	}
	
		
}

void delete_messages(struct user_node **user_list,int user_id)
{
	struct user_node *head = *user_list;
	struct user_node *curr_user = head;
	
	struct message_node *curr_msg = NULL;
	struct message_node *prev_msg = NULL;
	struct message_node *tmp = NULL;
	
	while(curr_user)
	{
		tmp = NULL;
		prev_msg = NULL;
		curr_msg = curr_user -> inbox;
		while(curr_msg)
		{
			/*if this msg is from the deleted user. then we should delete it.*/
			if(curr_msg -> sender_id == user_id)
			{
				tmp = curr_msg -> next;
				if(prev_msg)/*not the first msg in the inbox.*/
				{
						free(curr_msg);
						prev_msg -> next = tmp;
						curr_msg = prev_msg -> next;
						continue;
				}
				else
				{
					free(curr_msg);
				curr_user -> inbox = (struct message_node *)malloc(sizeof(struct message_node));
					curr_user -> inbox = tmp;
					curr_msg = curr_user -> inbox;
					continue;
				}
				
			}
			prev_msg = curr_msg;
			curr_msg = curr_msg -> next;
			
		}
		/*job done. continue with the next user. RESET prev & tmp variables.*/
		curr_user = curr_user -> next_user;		
	}
	
}


void unsub(struct topic_node **topic_list, int topic_id, int user_id)
{
	struct topic_node* topic_ptr = *topic_list;
	struct node *follower_list  = NULL;
	struct node *prev_follower  = NULL;
	struct node *tmp  = NULL;
	
	while(topic_ptr)
	{
		if(topic_ptr -> topic_id == topic_id)
		{
			follower_list = topic_ptr -> follower_list;
			/*only one subscriber!!. head will change:// */
			if(follower_list->next == NULL)
			{
				if(follower_list -> data == user_id)
				{
					topic_ptr -> follower_list ->data = 0;
					topic_ptr -> follower_list ->next = NULL;
					return;
				}
			}
			
			
			while(follower_list)/*more than 1 sub.*/
			{
				
				if(follower_list -> data == user_id)
				{
					tmp = follower_list->next;
					free(follower_list);
					
					if(prev_follower == NULL)
					{	
						topic_ptr -> follower_list = tmp;
						return;
					}
					
					prev_follower ->next = tmp;
					return;
				}
				prev_follower = follower_list;
				follower_list = follower_list ->next;
			}
		}
		topic_ptr = topic_ptr->next;
	}
	
}


void del_sub(struct user_node **user_list,int topics_id,int users_id)
{
	struct user_node *curr_user = *user_list;
	struct message_node *msg = NULL;
	struct message_node *prev_msg = NULL;
	struct message_node *tmp = NULL;
	
	struct node *followed=NULL;
	struct node *prev_followed=NULL;
	struct node *fol_tmp=NULL;
	
	while(curr_user)/*traverse the user list for the greater good!*/
	{
		if(curr_user ->user_id == users_id)
		{
				followed = curr_user -> followed_topics;/*traverse the users followed topics lst*/
				while(followed)
				{
					if(followed->data == topics_id)/*gotta delete this topic id..*/
					{
						fol_tmp = followed->next;
						if(prev_followed)
						{
							free(followed);
							prev_followed->next = fol_tmp;
						}
						else
						{
							free(followed);
							curr_user -> followed_topics = fol_tmp;
						}
					}
					prev_followed = followed;
					followed = followed -> next;	
				}
		}
		curr_user = curr_user->next_user;
	}
	
	
	curr_user = *user_list;
	while(curr_user)
	{
		if(curr_user -> user_id == users_id)
		{	
			msg = curr_user ->inbox;
			prev_msg = NULL;
			
			while(msg)/*traverse the inbox.*/
			{
				
				if(msg->topic_id == topics_id) /*CONTROVERSIAL PART!! ARE WE DELETING ALLL THE MESSAGES?_?_? INCLUDING THE ONES USER HIMSELF SENT?_?*/
				/*found a candidate.*/
				{
					tmp = msg->next;
					free(msg);
					
					if(prev_msg == NULL)/*then its the first message in the inbox..*/
					{
						curr_user->inbox = tmp;
						msg = curr_user -> inbox;
						continue;
					}
					else
					{
						prev_msg ->next= tmp;
						msg = prev_msg -> next;
						continue;
					}
				
				}
				
				prev_msg = msg;
				msg = msg->next;
			}
			
			
		}
		curr_user = curr_user ->next_user;
	}
	
}



void addMessage(struct user_node **user_list,int author_id,int topics_id)
{
	struct user_node *ptr = *user_list;
	struct user_node *ptr2 = *user_list;
	struct node *followed = NULL;
	struct message_node *inboxptr = NULL;
	struct message_node *prev = NULL;
	
	
	/*add to authors inbox*/
	
	while(ptr2)
	{
		if(ptr2->user_id == author_id)
		{
			inboxptr = ptr2->inbox;
			if(inboxptr == NULL)
			{
				ptr2->inbox = (struct message_node*)malloc(sizeof(struct message_node));
				inboxptr = ptr2->inbox;
				
				inboxptr -> sender_id = author_id;
				inboxptr -> topic_id = topics_id;
				inboxptr -> message_id = message_id;
				inboxptr -> next = NULL;
			}
			else
			{
				while(inboxptr)
				{
					prev = inboxptr;
					inboxptr = inboxptr -> next;
				}
				prev->next = (struct message_node*)malloc(sizeof(struct message_node));
				prev->next->sender_id = author_id;
				prev->next->topic_id = topics_id;
				prev->next->message_id = message_id;
				prev->next->next = NULL;
			}
			
		}
		ptr2 = ptr2->next_user;
	}
	inboxptr = NULL;
	prev = NULL;
	
	/*find the users who follow this topic.*/
	while(ptr)
	{
		followed = ptr->followed_topics;
		while(followed)
		{

			if(followed->data == topics_id)
			{
				
				if(ptr -> inbox == NULL)
				{
					ptr -> inbox = (struct message_node*)malloc(sizeof(struct message_node));
					ptr->inbox->sender_id = author_id;
					ptr->inbox->topic_id = topics_id;
					ptr->inbox->message_id = message_id;
					ptr->inbox->next = NULL;
				}
				else
				{
					if(ptr->user_id != author_id)
					{
						inboxptr = ptr->inbox;
						
						while(inboxptr && (message_id > (inboxptr->message_id)) )
						{
							prev = inboxptr;
							inboxptr = inboxptr->next;
						}
						prev->next = (struct message_node*)malloc(sizeof(struct message_node));
						prev->next->sender_id = author_id;
						prev->next->topic_id = topics_id;
						prev->next->message_id = message_id;
						prev->next->next = NULL;
					}
					
				}
				
			}
			followed = followed -> next;
		}
		ptr = ptr->next_user;
	}
}



void printInbox(struct user_node **user_list)
{
	struct user_node *ptr = *user_list;
	struct message_node *msg = NULL;
	
	/*traverse the userlist and print their inboxes.*/
	while(ptr)
	{
		printf("%d:",ptr->user_id);
		msg = ptr->inbox;
		while(msg)
		{
			printf(" %d",msg->message_id);
			msg = msg->next;
		}
		printf("\n");
		ptr = ptr->next_user;
	}
	
}



void subscribe_user(struct user_node **user_list,struct topic_node **topic_list,char *username,char *topic_name,int t_id)
{
	struct topic_node *head = NULL;
	struct topic_node *ptr  = NULL;
	struct topic_node *prev  = NULL;
	
	struct node *prev_fol  = NULL;
	struct node *followers=NULL;
	int user_id;
	int fol_top;
	
	/* find the user id of given username.*/
	user_id = find_id(user_list,username);
	
	/* topic list is empty.*/
	if(*topic_list == NULL)
	{
		
		*topic_list = (struct topic_node*)malloc(sizeof(struct topic_node));
		head = *topic_list;
		ptr = head;
		
		make_topicNode(head,topic_name,t_id,user_id);
		ptr -> follower_list -> data = user_id;
		
		/* add this topic's id to users' followed topics list*/
		fol_top = find_topic(topic_list,topic_name);
		users_topics(user_list,user_id,fol_top);
		
		topic_id++;
	} 
	else
	{
		head = *topic_list;
		ptr = head;
		
		do
		{
			/* if topic already existing */
			if(!strcmp(ptr->topic_name,topic_name))
			{
				followers = ptr->follower_list;
				
				if(followers == NULL)
				{
					ptr->follower_list =(struct node*)malloc(sizeof(struct node));
					followers = ptr->follower_list;
				}
				while(followers)
				{
					prev_fol = followers;
					followers = followers -> next;
				}
				/* update the follower list.*/
				prev_fol ->next = (struct node*)malloc(sizeof(struct node));
				prev_fol ->next ->data = user_id;
				prev_fol ->next ->next = NULL;
				
				/* add this topic's id to users' followed topics list*/
				fol_top = find_topic(topic_list,topic_name);
				users_topics(user_list,user_id,fol_top);
				
				return;
			}
			prev = ptr;
			ptr = ptr -> next;
		}
		while(ptr);
		
		/*add new topic.*/
		prev->next  = (struct topic_node*)malloc(sizeof(struct topic_node));
		make_topicNode(prev->next,topic_name,t_id,user_id);
		prev->next -> follower_list -> data = user_id;
		
		/* add this topic's id to users' followed topics list*/
		fol_top = find_topic(topic_list,topic_name);
		users_topics(user_list,user_id,fol_top);
		topic_id++;
	}

}


void make_topicNode(struct topic_node* topic,char *topicname,int topic_id,int user_id)
{
	topic -> topic_id = topic_id;
	topic -> topic_name = (char *) malloc(sizeof(char)*21);
	topic -> topic_name = strcpy(topic->topic_name,topicname);
	topic -> next = NULL;
	topic -> follower_list = (struct node *)malloc(sizeof(struct node));
	topic -> follower_list -> data = 0;
	topic -> follower_list -> next = NULL;
	
}

int find_id(struct user_node **user_list,char *username)
{
	int result;
	struct user_node *ptr = NULL;
	ptr = *user_list;
	
	while(ptr)
	{
		if(!strcmp(ptr->user_name,username))
		{
		
			result = ptr->user_id;
			return result;
		}
		ptr = ptr->next_user;
	}
	return 1;
}

int find_topic(struct topic_node **topic_list,char* topicname)
{
	int result;
	struct topic_node *ptr = NULL;
	ptr = *topic_list;
	
	while(ptr)
	{
		if(!strcmp(ptr->topic_name,topicname))
		{
			result = ptr->topic_id;
			return result;
		}
		ptr = ptr->next;
	}
	return -1;
}

void users_topics(struct user_node **user_list,int user_id,int fol_top)
{
	struct user_node *ptr = *user_list;
	struct node *fol_list = NULL;
	
	while(ptr)
	{
		if(ptr->user_id == user_id)
		{
			fol_list = ptr-> followed_topics;
			
			if(fol_list == NULL)
			{
				ptr->followed_topics =(struct node *)malloc(sizeof(struct node));
				fol_list = ptr->followed_topics;
				fol_list -> data = fol_top;
				fol_list -> next  = NULL;
				return;
			}
			
			while(fol_list)
			{
				if(fol_list->next == NULL)
				{
					fol_list -> next = (struct node *)malloc(sizeof(struct node));
					fol_list -> next -> data = fol_top;
					fol_list -> next -> next = NULL;
					
					return;
				}
				fol_list = fol_list -> next;
			}
		}
		ptr = ptr->next_user;
	}
}

void add_user(struct user_node ** list,char *username,int id_count)
{
	struct user_node *head = NULL;
	struct user_node *ptr = NULL;
	
	/* user list is empty*/
	if(*list == NULL)
	{
		*list = (struct user_node *)malloc(sizeof(struct user_node));
		head = *list;
		make_userNode(head,username,id_count);
		
	}	
	else /*some users already exist*/
	{
		head = *list;
		ptr = head;
		while(ptr->next_user) /*traverse the list till the end*/
		{
			ptr = ptr -> next_user;
		}
		/* create the new user.*/
		ptr->next_user = (struct user_node *)malloc(sizeof(struct user_node));
		make_userNode(ptr->next_user,username,id_count);
	}
}

void make_userNode(struct user_node* user,char *username,int id_count)
/* initializes the new user with default values.*/
{
	user -> user_id = id_count;
	user -> user_name = (char *) malloc(sizeof(char)*21);
	user -> user_name = strcpy(user->user_name,username);
	user -> next_user = NULL;
	user -> followed_topics = NULL;
	user -> inbox = NULL;
}

