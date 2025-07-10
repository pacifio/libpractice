#include <stdio.h>
#include <stdlib.h>

struct node {
  int data;
  struct node *next;
};

struct LinkedList {
  struct node *head;
};

void add(struct LinkedList *ll, int data) {
  struct node *newNode = malloc(sizeof(struct node));
  newNode->data = data;
  newNode->next = NULL;

  if (ll->head == NULL) {
    ll->head = newNode;
  } else {
    struct node *curr = ll->head;
    while (curr->next != NULL) {
      curr = curr->next;
    }

    curr->next = newNode;
  }
}

struct node *get(struct LinkedList *ll, int index) {
  int counter = 0;
  struct node *curr = ll->head;
  while (curr->next != NULL) {
    if (index == counter) {
      return curr;
    }

    curr = curr->next;
    counter++;
  }

  return NULL;
}

void update(struct LinkedList *ll, int index, int data) {
  int i = 0;
  struct node *curr = ll->head;

  while (curr->next != NULL) {
    if (i == index) {
      curr->data = data;
      break;
    }

    curr = curr->next;
    i++;
  }
}

void delete(struct LinkedList *ll, int index) {
  int i = 0;
  struct node *curr = ll->head;
  while (curr->next != NULL) {
    if (i == index - 1) {
      curr->next = curr->next->next;
      break;
    }

    curr = curr->next;
    i++;
  }
}

int len(struct LinkedList *ll) {
  int i = 0;
  struct node *curr = ll->head;
  while (curr->next != NULL) {
    curr = curr->next;
    i++;
  }

  return i + 1;
}

void display(struct LinkedList *ll) {
  int i = 0;
  struct node *curr = ll->head;
  int total = len(ll);

  if (ll->head != NULL) {
    printf("%d", ll->head->data);
    if (ll->head->next != NULL) printf(" -> ");
  }

  while (curr->next != NULL) {
    curr = curr->next;
    i++;

    printf("%d", curr->data);
    if (i < total - 1) printf(" -> ");
  }

  printf("\n");
}

int main(int argc, char *argv[]) {
  struct LinkedList *ll = malloc(sizeof(struct LinkedList));
  if (ll == NULL) {
    printf("error at allocating ll\n");
    return 0;
  }

  add(ll, 10);
  add(ll, 20);
  add(ll, 30);

  display(ll);

  struct node *item = get(ll, 1);
  printf("%d\n", item->data);

  update(ll, 1, 40);
  display(ll);

  delete(ll, 1);
  display(ll);

  free(ll);

  return 0;
}