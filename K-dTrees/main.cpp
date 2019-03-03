#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <array>
#include <list>
#include <tuple>

// Dimension of the tree 
#define K 5

using namespace std;

/** 
 * Implementation of a k-d tree for positive integers
 */
struct node {
  array<int,K> key;
  int discriminant;
  node *left;
  node *right;
};

/**
 * Inserts an element into a k-d tree 
 */
node* insert(array<int,K> elements, node* root) {
  if (root == NULL) {
    root = (node*) malloc(sizeof(struct node));
    root->key = elements;
    root->left = NULL;
    root->right = NULL;
    root->discriminant = rand() % (K-1);
  } else if (root->key[root->discriminant] < elements[root->discriminant]) {
    root->right = insert(elements, root->right);
  } else root->left = insert(elements, root->left);
  return root;
}

/**
 * Searches a k-d tree for elements matching a partial element
 * Indexes not specified -> -1
 */
list<array<int,K>> partial_match(array<int,K> partial, node* root, list<array<int,K>> result){
  if (root == NULL) return result;
  bool has_specified = false;
  bool is_equal = true;
  // Check if the element matches
  for(int i=0; i<K && is_equal; i++){
    int p = partial[i];
    if (p != -1) {
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
  if (partial[root->discriminant] == -1) {
    result = partial_match(partial, root->right, result);
    result = partial_match(partial, root->left, result);
  }
  else if (root->key[root->discriminant] < partial[root->discriminant]) {
    result = partial_match(partial, root->right, result);
  } else result = partial_match(partial, root->left, result);
  return result;
}

/** 
  * Searches a k-d tree for elements inside the given range
  * Indexes not specified -> -1
  */  
list<array<int,K>> orthogonal_range(array<tuple<double, double>,K> ranges, node *root, list<array<int,K>> result){
  if (root == NULL) return result;
  
  int lower_disc = get<0>(ranges[root->discriminant]);
  int upper_disc = get<1>(ranges[root->discriminant]);

  bool has_specified = false;
  bool is_inside = true;
  // Check if the element is inside the range
  for(int i=0; i<K && is_inside; i++){
    int lower = get<0>(ranges[i]);
    int upper = get<1>(ranges[i]);
    if (lower != -1) {
      has_specified = true;
      if (root->key[i] < lower || root->key[i] > upper) {
        is_inside = false;
      }
    }
  }
  // The element matches so we add it to the result
  if (is_inside && has_specified) {
    result.push_back(root->key);
  }
  if (lower_disc == -1 || upper_disc == -1 || ((root->key[root->discriminant] >= lower_disc) && root->key[root->discriminant] <= upper_disc)) {
    result = orthogonal_range(ranges, root->right, result);
    result = orthogonal_range(ranges, root->left, result);
  }
  else if (root->key[root->discriminant] < lower_disc) {
    result = orthogonal_range(ranges, root->right, result);
  } else if (root->key[root->discriminant] > upper_disc) {
    result = orthogonal_range(ranges, root->left, result);
  }
  return result;
}


int main() {
  srand (101);
  array<int,K> a,b,c,d,e;
  // Initialize random element arrays
  for (int i = 0; i < K; i++) {
    a[i] = rand() % 10; 
    b[i] = rand() % 10; 
    c[i] = rand() % 10; 
    d[i] = rand() % 10; 
    e[i] = rand() % 10; 
  }

  // Create k-d tree
  node *root = NULL;

  // Insert elements into tree
  root = insert(a, root);
  root = insert(b, root);
  root = insert(c, root);
  root = insert(d, root);
  root = insert(e, root);

  // Checking the values of the elements
  cout << "A => " << a[0] << " " << a[1] << " " << a[2] << " " << a[3] << " " << a[4] << endl;  
  cout << "B => " << b[0] << " " << b[1] << " " << b[2] << " " << b[3] << " " << b[4] << endl;  
  cout << "C => " << c[0] << " " << c[1] << " " << c[2] << " " << c[3] << " " << c[4] << endl;  
  cout << "D => " << d[0] << " " << d[1] << " " << d[2] << " " << d[3] << " " << d[4] << endl;  
  cout << "E => " << e[0] << " " << e[1] << " " << e[2] << " " << e[3] << " " << e[4] << endl;  

  // Declaring the partial match query element
  array<int,K> partial = {-1, 0, -1, -1, -1};

  list<array<int,K>> result;

  // Fetching the elements matching our query
  result = partial_match(partial, root, result);

  // Checking results
  cout << result.empty() << endl;
  while (!result.empty())
  {
    array<int,K> curr = result.front();
    cout << "Partial match result => " << curr[0] << " " << curr[1] << " " << curr[2] << " " << curr[3] << " " << curr[4] << endl;  
    result.pop_front();
  }

  list<array<int,K>> result2;

  // Define orthogonal ranges for our query
  tuple<double,double> at (0, 9);
  tuple<double,double> bt (-1, -1);
  tuple<double,double> ct (-1, -1);
  tuple<double,double> dt (-1, -1);
  tuple<double,double> et (0.7, 4.9);

  array<tuple<double, double>, K> ranges = {at, bt, ct, dt, et};

  // Fetching the elements matching our query
  result2 = orthogonal_range(ranges, root, result2);

  // Checking results
  cout << result2.empty() << endl;
  while (!result2.empty())
  {
    array<int,K> curr = result2.front();
    cout << "Partial match result => " << curr[0] << " " << curr[1] << " " << curr[2] << " " << curr[3] << " " << curr[4] << endl;  
    result2.pop_front();
  }

  return 0;
}