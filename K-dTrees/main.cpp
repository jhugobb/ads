#include <stdio.h>
#include <stdlib.h>
#include <array>
#include <list>

#define K 5

struct node {
  std::array<int,K> key;
  int discriminant;
  node *left;
  node *right;

};


void insert(std::array<int,K> elements, node* root) {
  if (root == NULL) {
    root = (node*) malloc(sizeof(struct node));
    root->key = elements;
    root->left = NULL;
    root->right = NULL;
    root->discriminant = rand() % (K-1);
  } else if (root->key[root->discriminant] < elements[root->discriminant]) {
    insert(elements, root->right);
  } else insert(elements, root->left);
}

// not specified-> NULL
//TODO: need to check that tree is not null
void partial_match(std::array<int,K> partial, node* root, std::list<std::array<int,K>> result){
  bool has_specified = false;
  bool is_equal = true;
  // Check if the element matches
  for(int i=0; i<K && is_equal; i++){
    int p = partial[i];
    if (p != NULL) {
      has_specified = true;
      if (p != root->key[i]) {
        is_equal = false;
      }
    }
  }
  // The element matches so we add it to the result
  if (is_equal && has_specified) {
    result.push_back(root->key);
  }
  if (root->key[root->discriminant] < partial[root->discriminant]) {
    partial_match(partial, root->right, result);
  } else partial_match(partial, root->left, result);
}
// x_d [l_d, u_p] => if x_d < l_d go to right
//                => if x_d > u_d go to left
//                => if x_d in range, follow both
// if range is not specified 
void orthogonal_range();


int main() {
    return 0;
}