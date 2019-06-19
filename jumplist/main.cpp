#include <iostream>
#include <limits>
#include <random>
#include <ctime>
#include <map>
#include <assert.h>

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

node* build_perfect_jumplist(node* x, int n) {
  node* y;
  while (n > 1) {
    int m = floor(double(n-1)/2.0);
    n = n - m - 1;
    x->nsize = m;
    x->jsize = n;
    y = x->next;
    if (m == 0) x->jump = y;
    else x->jump = build_perfect_jumplist(y, m);
    x = x->jump;
  }
  x->jump = x;
  x->nsize = 0;
  x->jsize = 0;
  return x->next;
}

node* find_predecessor(node* header, int k) {
  node* x = header;
  bool found = false;

  while (x->jump != x && !found) {
    
    if (x->jump->key < k) {
      x = x->jump;
    } else if (x->next->key < k) {
      x = x->next;
    } else found = true;
  }
  return x;
}

bool contains(node* header, int k) {
  node* pre = find_predecessor(header, k);
  if (pre->next != NULL)
    return pre->key == k || pre->next->key == k;
  return pre->key == k;
}

void insert(node* header, int k) {
  if (contains(header, k)) return;
  node* x = header;
  node* y = new node(k);
  y->jump = y;
  bool found = false;
  
  while(x->jump != x && !found) {
    // float c = (x->nsize+1) / (x->count()+1);
    // if (c < alpha || (1 - alpha) < c)
    //   build_perfect_jumplist(x, x->count());
    
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
    if (x->jump != y->next) {
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


bool delete_j(node* header, int k) {
  if (!contains(header, k)) return false;
  node* x = header;
  node* s; 
  while (x->key != k) {
    s = x;
    // float c = (x->nsize + 1) / (x->count()+1);
    // if (c < alpha || (1 - alpha) < c)
    //   build_perfect_jumplist(x, x->count());
    if (x->jump->key <= k) {
      x->jsize = x->jsize - 1;
      x = x->jump;
    } else if (x->next->key <= k) {
      x->nsize = x->nsize - 1;
      x = x->next;
    }
  }
  if (s->jump == x) {
    if (x->jump == x) {
      if (s->next == x) s->jump = s; 
      else s->jump = s->next;
      s->jsize = s->nsize;
      s->nsize = 0; 
    } else {
      s->jump = x->next;
    }
    if (s->next != x) {
      s = s->next;
      while (s->jump != s) s = s->jump;
    }
  }
  s->next = x->next;
  if (x->nsize != 0) {
    node* y = x->next;
    node* t1 = x->jump;
    int jc1 = x->jsize;
    while (y->nsize > 0) {
      node* t2 = y->jump;
      int jc2 = y->jsize;
      y->jump = t1;
      y->nsize = y->nsize + jc2;
      y->jsize = jc1;
      t1 = t2;
      jc1 = jc2;
      y = y->next;
    }
    y->jump = t1;
    y->nsize = y->jsize;
    y->jsize = jc1;
  }
  free(x);
  return true;
}

int main(int argc, char* argv[]) {
  srand(2612);
  int N = strtol(argv[1], NULL, 10);
  node* header = new node(std::numeric_limits<int>::min());
  header->jump = header;
  // header->next = header;
  clock_t j_begin, j_end, m_begin, m_end;
  double j_total_ins, m_total_ins, j_total_cont, m_total_cont, j_total_del, m_total_del;
  j_total_ins = j_total_cont = j_total_del = 0;
  m_total_ins = m_total_cont = m_total_del = 0;
  // build_perfect_jumplist(header, 0);

  std::map<int, int> m;
  std::vector<int> ins(1000000, 0);

  for (int i = 0; i < N; i++) {
    int k = rand() % 1000000;
    // std::cout << "insert " << k << std::endl;
    ins[k] = 1;
    j_begin = clock();
    insert(header, k);
    j_end = clock();
    j_total_ins += (double(j_end - j_begin)) / CLOCKS_PER_SEC; 

    m_begin = clock();
    m[k] = 1;
    m_end = clock();
    m_total_ins += (double(m_end - m_begin)) / CLOCKS_PER_SEC;

  }

  bool j_c, m_c;
  for (int i = 0; i < N; i++) {
    int k = rand() % 1000000;
    // std::cout << "key " << k << std::endl;

    j_begin = clock();
    j_c = contains(header, k);
    j_end = clock();
    j_total_cont += (double(j_end - j_begin)) / CLOCKS_PER_SEC;

    m_begin = clock();
    m_c = (m.find(k) != m.end());
    m_end = clock();
    m_total_cont += (double(m_end - m_begin)) / CLOCKS_PER_SEC;

    //assert(j_c == m_c);
    if (j_c != m_c) {
      std::cout << "FUCK" << std::endl;
      std::cout << ins[k] << std::endl;
    } 
  }

  for (int i = 0; i < N; i++) {
    int k = rand() % 1000000;
    // std::cout << "key deleted " << k << std::endl;
    j_begin = clock();
    j_c = delete_j(header, k);
    j_end = clock();
    j_total_del += (double(j_end - j_begin)) / CLOCKS_PER_SEC;

    m_begin = clock();
    std::multimap<int,int>::iterator it;
    it = m.find(k);
    m_c = it != m.end();
    if (m_c) {
      m.erase(it);
      m_c = true;
    }
    m_end = clock();
    m_total_del += (double(m_end - m_begin)) / CLOCKS_PER_SEC;

    // assert(j_c == m_c);
    if (j_c != m_c) {
      std::cout << "FUCK" << std::endl;
      std::cout << j_c << " " << m_c << std::endl;
      std::cout << ins[k] << std::endl;
      std::cout << contains(header, k) << std:: endl;
    }
    ins[k] = 2;
  } 

  std::cout << "Total jumplist insertion time for N = " << N << ": " << j_total_ins << std::endl;
  std::cout << "Total map insertion time for N = " << N << ": " << m_total_ins << std::endl;

  std::cout << "Total jumplist contains time for N = " << N << ": " << j_total_cont << std::endl;
  std::cout << "Total map contains time for N = " << N << ": " << m_total_cont << std::endl;

  std::cout << "Total jumplist deletion time for N = " << N << ": " << j_total_del << std::endl;
  std::cout << "Total map deletion time for N = " << N << ": " << m_total_del << std::endl;


  return 0;
}