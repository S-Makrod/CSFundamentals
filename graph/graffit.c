/**
 * CSC A48 - Intro to Computer Science II, Summer 2020
 * 
 * Assignment 3 - Graffit
 * 
 * Graphs &
 * Recursion
 * Assignment
 * For
 * Freshmen
 * In
 * Toronto
 *
 * (I am so proud of that initialism.)
 * 
 * This is the program file where you will implement your solution for
 * assignment 3. Please make sure you read through this file carefully
 * and that you understand what is provided and what you need to complete.
 * 
 * You will also need to have read the handout carefully. 
 * 
 * Parts where you have to implement functionality are clearly labeled TODO
 * 
 * Be sure to test your work thoroughly, our testing will be extensive
 * and will check that your solution is *correct*, not only that it
 * provides functionality.
 * 
 * (c) 2020 William Song, Mustafa Quraish
 **/

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_STR_LEN 1024
#ifndef __testing
#define MAT_SIZE 3	// A small graph
#endif

typedef struct user_struct {
  char name[MAX_STR_LEN];
  struct friend_node_struct* friends;
  struct brand_node_struct* brands;
  bool visited;
} User;

typedef struct friend_node_struct {
  User* user;
  struct friend_node_struct* next;
} FriendNode;

typedef struct brand_node_struct {
  char brand_name[MAX_STR_LEN];
  struct brand_node_struct* next;
} BrandNode;

typedef struct stack_linked_list{
  User * user;
  int depth;
  struct stack_linked_list * next;
} stackLL;

typedef struct degree_stack{
  stackLL * head;
} stack;

/** Note: We are reusing the FriendNode here as a Linked List for allUsers.
  * This is usually bad coding practice but it will allow us to reuse the
  * helper functions.
**/
FriendNode* allUsers; 

int brand_adjacency_matrix[MAT_SIZE][MAT_SIZE];
char brand_names[MAT_SIZE][MAX_STR_LEN];

/**
 * Checks if a user is inside a FriendNode LL.
 **/
bool in_friend_list(FriendNode *head, User *node) {
  for (FriendNode *cur = head; cur != NULL; cur = cur->next) {
    if (strcmp(cur->user->name, node->name) == 0) {
      return true;
    }
  }
  return false;
}

/**
 * Checks if a brand is inside a BrandNode LL.
 **/
bool in_brand_list(BrandNode *head, char *name) {
  for (BrandNode *cur = head; cur != NULL; cur = cur->next) {
    if (strcmp(cur->brand_name, name) == 0) {
      return true;
    }
  }
  return false;
}

/**
 * Inserts a User into a FriendNode LL in sorted position. If the user 
 * already exists, nothing is done. Returns the new head of the LL.
 **/
FriendNode *insert_into_friend_list(FriendNode *head, User *node) {
  if (node == NULL) return head;

  if (in_friend_list(head, node)) {
    printf("User already in list\n");
    return head;
  }
  FriendNode *fn = calloc(1, sizeof(FriendNode));
  fn->user = node;
  fn->next = NULL;

  if (head == NULL)
    return fn;
    
  if (strcmp(head->user->name, node->name) > 0) {
    fn->next = head;
    return fn;
  } 

  FriendNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->user->name, node->name) < 0; cur = cur->next);

  fn->next = cur->next;
  cur->next = fn;
  return head;
}

/**
 * Inserts a brand into a BrandNode LL in sorted position. If the brand 
 * already exists, nothing is done. Returns the new head of the LL.
 **/
BrandNode *insert_into_brand_list(BrandNode *head, char *node) {
  if (node == NULL) return head;

  if (in_brand_list(head, node)) {
    printf("Brand already in list\n");
    return head;
  }
  BrandNode *fn = calloc(1, sizeof(BrandNode));
  strcpy(fn->brand_name, node);
  fn->next = NULL;

  if (head == NULL)
    return fn;
    
  if (strcmp(head->brand_name, node) > 0) {
    fn->next = head;
    return fn;
  } 

  BrandNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->brand_name, node) < 0; cur = cur->next);

  fn->next = cur->next;
  cur->next = fn;
  return head;
}

/**
 * Deletes a User from FriendNode LL. If the user doesn't exist, nothing is 
 * done. Returns the new head of the LL.
 **/
FriendNode *delete_from_friend_list(FriendNode *head, User *node) {
  if (node == NULL) return head;

  if (!in_friend_list(head, node)) {
    printf("User not in list\n");
    return head;
  }

  if (strcmp(head->user->name, node->name) == 0) {
    FriendNode *temp = head->next;
    free(head);
    return temp;
  }

  FriendNode *cur;
  for (cur = head; cur->next->user != node; cur = cur->next);

  FriendNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);
  return head;
}

/**
 * Deletes a brand from BrandNode LL. If the user doesn't exist, nothing is 
 * done. Returns the new head of the LL.
 **/
BrandNode *delete_from_brand_list(BrandNode *head, char *node) {
  if (node == NULL) return head;

  if (!in_brand_list(head, node)) {
    printf("Brand not in list\n");
    return head;
  }

  if (strcmp(head->brand_name, node) == 0) {
    BrandNode *temp = head->next;
    free(head);
    return temp;
  }

  BrandNode *cur;
  for (cur = head; strcmp(cur->next->brand_name, node) != 0; cur = cur->next);

  BrandNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);
  return head;
}

/**
 * Prints out the user data.
 **/
void print_user_data(User *user) {
  printf("User name: %s\n", user->name);
  printf("Friends:\n");
  for (FriendNode *f = user->friends; f != NULL; f = f->next) {
    printf("   %s\n", f->user->name);
  }
  printf("Brands:\n");
  for (BrandNode *b = user->brands; b != NULL; b = b->next) {
    printf("   %s\n", b->brand_name);
  }
}

/**
 * Get the index into brand_names for the given brand name. If it doesn't
 * exist in the array, return -1
 **/
int get_brand_index(char *name) {
  for (int i = 0; i < MAT_SIZE; i++) {
    if (strcmp(brand_names[i], name) == 0) {
      return i;
    }
  }
  printf("brand '%s' not found\n", name);
  return -1; // Not found
}

/**
 * Print out brand name, index and similar brands.
 **/
void print_brand_data(char *brand_name) {
  int idx = get_brand_index(brand_name);
  if (idx < 0) {
    printf("Brand '%s' not in the list.\n", brand_name);
    return;
  }
  printf("Brand name: %s\n", brand_name);
  printf("Brand idx: %d\n", idx);
  printf("Similar brands:\n");
  for (int i = 0; i < MAT_SIZE; i++) {
    if (brand_adjacency_matrix[idx][i] == 1 && strcmp(brand_names[i], "") != 0) {
      printf("   %s\n", brand_names[i]);
    }
  }
}

/**
 * Read from a given file and populate a the brand list and brand matrix.
 **/
void populate_brand_matrix(char* file_name) {
  // Read the file
  char buff[MAX_STR_LEN];
  FILE* f = fopen(file_name, "r");
  fscanf(f, "%s", buff);
  char* line = buff;
  // Load up the brand_names matrix
  for (int i = 0; i < MAT_SIZE; i++) {
    if (i == MAT_SIZE - 1) {
      strcpy(brand_names[i], line);
      break;
    }
    int index = strchr(line, ',') - line;
    strncpy(brand_names[i], line, index);
    line = strchr(line, ',') + sizeof(char);
  }
  // Load up the brand_adjacency_matrix
  for (int x = 0; x < MAT_SIZE; x++) {
    fscanf(f, "%s", buff);
    for (int y = 0; y < MAT_SIZE; y++) {
      int value = (int) buff[y*2];
      if (value == 48) { value = 0; }
      else {value = 1;}
      brand_adjacency_matrix[x][y] = value;
    }
  }
}

// Users
/**TODO: Complete this function
 * Creates and returns a user. Returns NULL on failure.
 **/
User* create_user(char* name){
  User * new_user = (User *)calloc(1, sizeof(User));

  if(new_user != NULL){
    strcpy(new_user->name, name);
    new_user->brands = NULL;
    new_user->friends = NULL;
    new_user->visited = false;

    allUsers = insert_into_friend_list(allUsers, new_user);
    
    return new_user;
  }
  return NULL;
}

/**TODO: Complete this function
 * Deletes a given user. 
 * Returns 0 on success, -1 on failure.
 **/
int delete_user(User* user){
  if(user != NULL){
    FriendNode * pointer = user->friends, * temp = NULL;
    BrandNode * pointer2 = user->brands, * temp2 = NULL;

    while(pointer != NULL){
      pointer->user->friends = delete_from_friend_list(pointer->user->friends, user);
      temp = pointer;
      pointer = pointer->next;
      free(temp);
    }

    while(pointer2 != NULL){
      temp2 = pointer2;
      pointer2 = pointer2->next;
      free(temp2);
    }

    allUsers = delete_from_friend_list(allUsers, user);

    if(!in_friend_list(allUsers, user)){
      free(user);
      return 0;
    }
  }

  return -1;
}

/**TODO: Complete this function
 * Create a friendship between user and friend.
 * Returns 0 on success, -1 on failure.
 **/
int add_friend(User* user, User* friend){
  if(user != NULL && friend != NULL){
    user->friends = insert_into_friend_list(user->friends, friend);
    friend->friends = insert_into_friend_list(friend->friends, user);

    if(in_friend_list(user->friends, friend) && in_friend_list(friend->friends, user)) return 0;
  }
  return -1;
}

/**TODO: Complete this function
 * Removes a friendship between user and friend.
 * Returns 0 on success, -1 on faliure.
 **/
int remove_friend(User* user, User* friend){
  if(user != NULL && friend != NULL){
    if(!in_friend_list(user->friends, friend) && !in_friend_list(friend->friends, user)) return -1;

    user->friends = delete_from_friend_list(user->friends, friend);
    friend->friends = delete_from_friend_list(friend->friends, user);

    if(!in_friend_list(user->friends, friend) && !in_friend_list(friend->friends, user)) return 0;
  }
  return -1;
}

bool valid_brand(char * brand_name){
  for(int i = 0; i < MAT_SIZE; i++){
    if(strcmp(brand_name, brand_names[i]) == 0) return true;
  }
  return false;
}

/**TODO: Complete this function
 * Creates a follow relationship, the user follows the brand.
 * Returns 0 on success, -1 on faliure.
 **/
int follow_brand(User* user, char* brand_name){
  if(brand_name == NULL || user == NULL) return -1;
  if(!valid_brand(brand_name)) return -1;

  user->brands = insert_into_brand_list(user->brands, brand_name);

  if(in_brand_list(user->brands, brand_name)) return 0;
  
  return -1;
}

/**TODO: Complete this function
 * Removes a follow relationship, the user unfollows the brand.
 * Returns 0 on success, -1 on faliure.
 **/
int unfollow_brand(User* user, char* brand_name){
  if(brand_name == NULL || user == NULL) return -1;
  if(!valid_brand(brand_name)) return -1;

  user->brands = delete_from_brand_list(user->brands, brand_name);

  if(!in_brand_list(user->brands, brand_name)) return 0;

  return -1;
}

/**TODO: Complete this function
 * Return the number of mutual friends between two users.
 **/
int get_mutual_friends(User* a, User* b){
  if(a == NULL || b == NULL) return -1;
  
  int counter = 0;
  FriendNode * a_friends = a->friends;

  while(a_friends != NULL){
    if(in_friend_list(b->friends, a_friends->user)){
      counter++;
    }
    a_friends = a_friends->next;
  }

  return counter;
}

stackLL * add_to_head(stack * follows, User * user, int depth){
  stackLL * new_node = (stackLL *)calloc(1, sizeof(stackLL));
  if(new_node != NULL){
    new_node->user = user;
    new_node->depth = depth;
    new_node->next = follows->head;

    return new_node;
  }
  return follows->head;
}

stackLL * remove_from_head(stack * follows){
  if(follows->head == NULL) return NULL;
  stackLL * new_head = follows->head->next;
  free(follows->head);
  return new_head;
}

int check_for_friend(stack * follows, User * user){
  User * to_check = follows->head->user;

  if(to_check == NULL) return -1;

  int depth = follows->head->depth;
  to_check->visited = true;
  follows->head = remove_from_head(follows);

  if(in_friend_list(to_check->friends, user)) return depth+1;
  
  FriendNode * pointer = to_check->friends;

  while(pointer != NULL){
    if(!(pointer->user->visited) || pointer->user->friends == NULL) follows->head = add_to_head(follows, pointer->user, depth+1);       
    pointer = pointer->next;
  }

  return -1;
}

void delete_stack(stack * follows){
  stackLL * pointer = follows->head;
  stackLL * temp = NULL;

  while(pointer != NULL){
    temp = pointer;
    pointer = pointer->next;
    free(temp);
  }
}

void reset_to_false(){
  FriendNode * pointer = allUsers;

  while(pointer != NULL){
    pointer->user->visited = false;
    pointer = pointer->next;
  }
}

/**TODO: Complete this function
 * A degree of connection is the number of steps it takes to get from
 * one user to another.
 * 
 * For example, if X & Y are friends, then we expect to recieve 1 when calling
 * this on (X,Y). Continuing on, if Y & Z are friends, then we expect to
 * recieve 2 when calling this on (X,Z).
 * 
 * Returns a non-negative integer representing the degrees of connection
 * between two users, -1 on failure.
 **/

int get_degrees_of_connection(User* a, User* b){
  int degree = -1, temp = -1;

  if(a->friends == NULL || b->friends == NULL) return -1;
  if(a == b) return -1;
  if(in_friend_list(a->friends, b)) return 1;

  stack follows;
  follows.head = add_to_head(&follows, a, 0);
  follows.head->next = NULL;

  while(follows.head != NULL){
    temp = check_for_friend(&follows, b);
    if(temp < degree && temp != -1) degree = temp;
    else if(degree == -1) degree = temp;
  }

  delete_stack(&follows);
  reset_to_false();
  return degree;
}

// Brands
/**TODO: Complete this function
 * Marks two brands as similar.
 **/
void connect_similar_brands(char* brandNameA, char* brandNameB){
  if(brandNameA == NULL || brandNameB == NULL) return;

  int index_A = get_brand_index(brandNameA);
  int index_B = get_brand_index(brandNameB);

  if(index_A == -1 || index_B == -1) return;

  brand_adjacency_matrix[index_B][index_A] = 1;
  brand_adjacency_matrix[index_A][index_B] = 1;
}

/**TODO: Complete this function
 * Marks two brands as not similar.
 **/
void remove_similar_brands(char* brandNameA, char* brandNameB){
  if(brandNameA == NULL || brandNameB == NULL) return;

  int index_A = get_brand_index(brandNameA);
  int index_B = get_brand_index(brandNameB);

  if(index_A == -1 || index_B == -1) return;

  brand_adjacency_matrix[index_B][index_A] = 0;
  brand_adjacency_matrix[index_A][index_B] = 0;
}

int get_same_brands(User * user, User * check){
  int counter = 0;
  BrandNode * pointer = check->brands;
  
  while(pointer != NULL){
    if(in_brand_list(user->brands, pointer->brand_name)) counter++;
    pointer = pointer->next;
  }

  return counter;
}

// Harder ones
/**TODO: Complete this function
 * Returns a suggested friend for the given user, returns NULL on failure.
 * See the handout for how we define a suggested friend.
 **/
User* get_suggested_friend(User* user){
  if(user == NULL) return NULL;

  FriendNode * pointer = allUsers;
  User * suggested_friend = NULL;
  int sim = 0, temp = 0;

  while(pointer != NULL){
    if(!in_friend_list(user->friends, pointer->user) && strcmp(pointer->user->name, user->name) != 0){
      temp = get_same_brands(user, pointer->user);
      if(sim < temp){
        sim = temp;
        suggested_friend = pointer->user;
      } else if(sim == temp){
        if(suggested_friend == NULL) suggested_friend = pointer->user;
        if(strcmp(suggested_friend->name, pointer->user->name) < 0 ) suggested_friend = pointer->user;
      }
    }
    pointer = pointer->next;
  }

  return suggested_friend;
}

/**TODO: Complete this function
 * Friends n suggested friends for the given user.
 * See the handout for how we define a suggested friend.
 * Returns how many friends were successfully followed.
 **/
int add_suggested_friends(User* user, int n){
  int counter = 0;
  User * new_friend = NULL;
  for(int i = 0; i < n; i++){
    new_friend = get_suggested_friend(user);

    if(new_friend != NULL){
      counter++;
      user->friends = insert_into_friend_list(user->friends, new_friend);
      new_friend->friends = insert_into_friend_list(new_friend->friends, user);
    }
  }
  return counter;
}

int get_similar_brand_count(User * user, char * brand_name){
  if(user == NULL || brand_name == NULL) return 0;
  if(!valid_brand(brand_name)) return 0;

  int counter = 0, index = get_brand_index(brand_name), brand_index;
  BrandNode * pointer = user->brands;

  if(user->brands == NULL) return 0;
  
  while(pointer != NULL){
    brand_index = get_brand_index(pointer->brand_name);
    if(brand_adjacency_matrix[brand_index][index] == 1) counter++;
    pointer = pointer->next;
  }
  
  return counter;
}

char * get_suggested_brands(User* user){
  char * suggested_brand = NULL;
  int sim = 0, temp = 0;

  for(int i = 0; i < MAT_SIZE; i++){
    if(!in_brand_list(user->brands, brand_names[i])){
      temp = get_similar_brand_count(user, brand_names[i]);
      if(sim < temp){
        sim = temp;
        suggested_brand = brand_names[i];
      } else if(sim == temp){
        if(suggested_brand == NULL) suggested_brand = brand_names[i];
        if(strcmp(suggested_brand, brand_names[i]) < 0 ) suggested_brand = brand_names[i];
      }
    }
  }

  return suggested_brand;
}

/**TODO: Complete this function
 * Follows n suggested brands for the given user.
 * See the handout for how we define a suggested brand.     
 * Returns how many brands were successfully followed. 	  	
 **/
int follow_suggested_brands(User* user, int n){
  int counter = 0;
  char * name = NULL;

  if(n > MAT_SIZE) n = MAT_SIZE;

  for(int i = 0; i < n; i++){
    name = get_suggested_brands(user);

    if(name != NULL){
      counter++;
      user->brands = insert_into_brand_list(user->brands, name);
    }
  }

  return counter;
}
