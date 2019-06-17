#include <iostream>
#include <limits>
#include <random>

#define alpha 0.3

struct node {
  int key;
  int jsize;
  int nsize;
  node* next;
  node* jump;
  node(int k) {
    key = k;
    jsize = 0;
    nsize = 0;
    next = NULL;
    jump = NULL;
  }
  int count() {
    return 1 + jsize + nsize;
  }
};

node* build_perfect_jumplist(node* root, int n) {
  while (n > 1) {
    int m = (n-1)/2;
    n = n - m - 1;
    root->nsize = m;
    root->jsize = n;
    node* y = root->next;
    if (m == 0) root->jump = y;
    else root->jump = build_perfect_jumplist(y, m);
    root = root->jump;
  }

  root->jump = root;
  root->nsize = 0;
  root->jsize = 0;
  return root->next;
}

void insert(node* header, int k) {
  node* x = header;
  node* y = new node(k);
  y->jump = y;
  bool found = false;
  while(x->jump != x && !found) {
    float c = (x->nsize + 1) / (x->count() + 1);
    if (c < alpha || (1 - alpha) < c)
      build_perfect_jumplist(x, x->count());
    
    if (x->jump->key <= k) {
      x->jsize = x->jsize + 1;
      x = x->jump;
    } else if (x->next->key <= k) {
      x->nsize = x->nsize + 1;
      x = x->next;
    } else found = true;
  }

  y->next = x->next;
  x->next = y;
  if (x->jump == x) {
    x->jump = y;
    x->jsize = 1;
  } else {
    if (x->jump != x->next) {
      y->nsize = x->nsize;
      while (y->nsize > 1) {
        y->jump = y->next->jump;
        y->jsize = y->next->jsize;
        y->nsize = y->next->nsize + 1;
        y = y->next;
      }
      if (y->nsize == 0) {
        y->jump = y;
        y->jsize = 0;
      } else {
        y->jump = y->next;
        y->jsize = 1;
        y->nsize = 0;
      }
    }
    if (x != header) x->nsize = x->nsize + 1; 
  }
}

node* find_predecessor(node* header, int k) {
  node* x = header;
  bool found = false;

  while (x->jump != x && !found) {
    if (x->jump->key <= k) {
      x = x->jump;
    } else if (x->next->key <= k) {
      x = x->next;
    } else found = true;
  }
  return x;
}

bool contains(node* header, int k) {
  node* pre = find_predecessor(header, k);
  return pre->key == k;
}

int main(int argc, char* argv[]) {

  node* header = new node(std::numeric_limits<int>::min());
  header->next = header;

  header->next = build_perfect_jumplist(header, 0);
  for (int i = 0; i < 20; i++) {
    int k = rand() % 10;
    std::cout << "insert " << k << std::endl;
    insert(header, k);
  }

  for (int i = 0; i < 30; i++) {
    int k = rand() % 10;
    std::cout << "key " << k << ": " << contains(header, k) << std::endl;
  }

  return 0;
}