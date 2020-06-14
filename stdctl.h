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

////////// alg.c
int search_by_xh(Table *tab, int xh);
int search_by_xm(Table *tab, const char *xm);
void table_add_student(Table *tab, Student *stu);
void table_modify_student(Table *tab, int i, Student *stu);

////////// main.c
void print_tab_tail(void);
void print_tab_head(void);
void print_table(Table *tab);
void print_student(Student *stu);
int run_init_student(Student *stu);
