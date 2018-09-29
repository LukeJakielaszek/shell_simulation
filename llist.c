#include<stdio.h>
#include<stdlib.h>

typedef struct node{
  struct node * next;
  struct node * prev;
  char * command;
}node;

typedef struct linkedList{
  struct node * head;
  int size;
  struct linkedList *next;
  struct linkedList *prev;
}linkedList;

typedef struct llist2{
  struct linkedList * head;
  int size;
}llist2;

// two dimensional list functions
llist2 * makeList2();
int isEmpty2(llist2 * list);
void printList2(llist2 * list2);


// one dimensional list functions 
linkedList * makeList1();
void append1(linkedList * list, char * command);
node * createNode(char * command);
void printList1(linkedList * list);
char * get1(linkedList * list, int index);
char * remove1(linkedList * list, int index);
int isEmpty1(linkedList * list);

int main(){
  linkedList * singleList = makeList1();
  append1(singleList, "hello");

  append1(singleList, "world");

  append1(singleList, "hiya");

  printList1(singleList);

  remove1(singleList, 2);
  
  printf("\n");
  printList1(singleList);


  llist2 * list2 = makeList2();

  printf("isEmpty : %d\n", isEmpty2(list2));

  printList2(list2);
}

void printList2(llist2 * list2){
  if(isEmpty2(list2)){
    printf("2D llist is Empty\n");
  }else{
    linkedList * temp = list2->head;
    printList1(temp);

    int i = 0;
    for(i = 0; i < list2->size; i++){
      temp=temp->next;
      printList1(temp);
    }
  }
}

int isEmpty2(llist2 * list){
  return list->head == NULL;
}

// makes a two dimensional linked list
llist2 * makeList2(){
  llist2 * list;

  // allocate memory for linked list
  list = (llist2*)malloc(sizeof(llist2));

  if(list == NULL){
    printf("ERROR: Linked list 2 failed to malloc.\n");
    exit(-1);
  }

  list->head = NULL;
  list->size = 0;

  return list;
}

int isEmpty1(linkedList * list){
  return list->head == NULL;
}

char * remove1(linkedList * list, int index){
  if(index < 0 || index >= list->size){
    printf("ERROR: Index out of single dimensional list range.\n");
    exit(-1);
  }

  node * retval = list->head;

  // remove node
  if(list->size == 1){
    // remove node from list of size one and set everything to 0/NULL
    list->head = NULL;
  }else if(index == 0){
    // if removing the first thing
    list->head = list->head->next;
  }else{
    // remove node from list of size > 1
    int i = 0;

    // find the desired node
    for(i = 0; i < index; i++){
      retval = retval->next;
    }

    // detach node
    retval->prev->next = retval->next;

    // detaching node before final node
    if(index < list->size-1){
      retval->next->prev = retval->prev;
    }
  }

  
  // decrement size
  list->size--;

  char * retchar = retval->command;

  free(retval);
  
  return retchar;
}

// returns the value at the specified index for a 1 dimensional list
char * get1(linkedList * list, int index){
  if(index < 0 || index >= list->size){
    printf("ERROR: Index out of single dimensional list range.\n");
    exit(-1);
  }

  node * retval = list->head;

  int i = 0;
  for(i = 0; i < index; i++){
    retval = retval->next;
  }

  return retval->command;
}

// prints a single dimensional linked list
void printList1(linkedList * list){
  int i = 0;
  node * temp = list->head;

  for(i = 0; i < list->size; i++){
    printf("%s\n", temp->command);
    temp = temp->next;
  }
}

// makes a single dimensional linked list
linkedList * makeList1(){
  linkedList * list;

  // allocate memory for linked list
  list = (linkedList*)malloc(sizeof(linkedList));

  if(list == NULL){
    printf("ERROR: Linked list 1 failed to malloc.\n");
    exit(-1);
  }

  list->head = NULL;
  list->next = NULL;
  list->prev = NULL;
  list->size = 0;
}

// appends to a single dimensional linked list
void append1(linkedList * list, char * command){
  node * temp;

  // creates temporary node
  temp = createNode(command);
  
  // attaches node to list
  if(list->head == NULL){
    // if list is empty sets head to temp
    list->head = temp;
    list->size++;
  }else{
    // if not empty, sets temp.next to head and then head to next
    temp->next = list->head;
    list->head->prev = temp;
    list->head = temp;
    list->size++;
  }
}

// creates a node with next set to null and command set to a string
node * createNode(char * command){
  node * temp;

  // malloc a new node
  temp = (node*)malloc(sizeof(node));

  // check if failed
  if(temp == NULL){
    printf("ERROR: Node failed to malloc.\n");
    exit(-1);
  }

  // initialize node values
  temp->next = NULL;
  temp->prev = NULL;
  temp->command = command;

  // return node
  return temp;
}
