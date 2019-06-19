#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <array>
#include <string>
#include <list>
#include <vector>
#include <tuple>
#include <map>
#include <ctime>

// Dimension of the tree 
#define K 3

using namespace std;

template <typename Iter>
bool next(Iter begin, Iter end)
{
    while (begin != end)       // we're not done yet
    {
        --end;
        if ((*end & 1) == 0)   // even number is treated as zero
        {
            ++*end;            // increase to one
            return true;       // still more numbers to come
        }
        else                   // odd number is treated as one
        {
            --*end;            // decrease to zero and loop
        }
    }
    return false;              // that was the last number
}

/** 
 * Implementation of a k-quad tree for positive integers
 */
struct nodequad {
  array<int,K> key;
  map<string, nodequad*> subtrees;
};

/**
 * Inserts an element into a k-d tree 
 */
nodequad* insert(array<int,K> elements, nodequad* root) {
  if (root == NULL) {
    root = (nodequad*) malloc(sizeof(struct nodequad));
    root->key = elements;
    string s = "";
    root->subtrees = map<string, nodequad*>();
    for (int i=0;i<K;i++)
      s.append("0");
    for (int i=0; i<K; i++){
      do {
        (root->subtrees)[s]; //declares the index as existing
      } while (next(s.begin(), s.end()));
    }
      
  } else {
    string s = "";
    for (int i = 0; i<K; i++) {
      if (elements[i] < (root->key)[i]) {
        s.append("0");
      } else {
        s.append("1");
      }
    }
    (root->subtrees)[s] = insert(elements, (root->subtrees)[s]);
  }
  return root;
}

/**
 * Searches a k-d tree for elements matching a partial element
 * Indexes not specified -> -1
 */
list<array<int,K>> partial_match(array<int,K> partial, nodequad* root, list<array<int,K>> result){
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
  string s = "";
  for (int i = 0; i<K; i++) {
    s.append("0");
  }
  vector<string> possible;
  possible.push_back(s);

  for (int i=0; i<K; i++) {
    vector<string> newones = vector<string>();
    for (int j=0; j<possible.size(); j++) {
      
      if (partial[i] == -1) {
        string newbits = possible.at(j);
        newbits[i] = '1';
        possible.at(j)[i] = '0';
        newones.push_back(newbits);
      } else if (partial[i] < (root->key)[i]) {
        possible.at(j)[i] = '0';
      } else {
        possible.at(j)[i] = '1';
      }
    }
    for (int y=0; y<newones.size(); y++) {
      string str = newones.at(y);
      possible.push_back(str);
    }
  }
  for (int i=0; i<possible.size(); i++) {
    string curr = possible.at(i);
    result = partial_match(partial, (root->subtrees)[curr], result);
  }
  return result;
}

int main(int argc, char* argv[]) {
  srand (100);
  int N = strtol(argv[1], NULL, 10);
  clock_t begin, end;
  double total;
  nodequad* root = NULL; 
  array<int,K> el;
  // Initialize random element arrays
  begin = clock();
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < K; j++) {
      el[j] = rand() % 100;
    }
    root = insert(el, root);
  }
  end = clock();

  total = double(end - begin) / CLOCKS_PER_SEC;

  cout << "total time of insertion: " << total << endl;
  cout << "value divided by N: " << total / double(N) << endl;
  
  array<int, K> a, b, c, d, e;
  for (int i = 0; i < K; i++) {
    a[i] = rand() % 10; 
    b[i] = rand() % 10; 
    c[i] = rand() % 10; 
    d[i] = rand() % 10; 
    e[i] = rand() % 10; 
  }

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
  array<int,K> partial = {2, -1, 7};

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
  return 0;
}