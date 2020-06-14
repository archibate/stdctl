#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct _Student {
  int xh;
  int xb;
  int cj;
  char xm[9];
} Student;

typedef struct _Table {
  int len;
  Student *s;
} Table;

////////// io.c
Table *load_table(const char *filename);
void free_table(Table *tab);
Table *alloc_table(void);

////////// alg.c
typedef void (*cb_t)(Student *stu);
int search_by_xh(Table *tab, int xh);
int search_by_xm(Table *tab, const char *xm);
Table *search_by_cj_range(Table *tab, int cmin, int cmax);
Table *search_partial_xm(Table *tab, const char *xm);
void table_add_student(Table *tab, Student *stu);
void table_modify_student(Table *tab, int i, Student *stu);
void table_delete_student(Table *tab, int i);

////////// main.c
void print_tab_tail(void);
void print_tab_head(void);
void print_student(Student *stu);
void print_table(Table *tab);
