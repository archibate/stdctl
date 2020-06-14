#include "stdctl.h"

void (*search_callback)(Student *stu);

int search_by_xh(Table *tab, int xh)
{
  int i;
  for (i = 0; i < tab->len; i++) {
    if (tab->s[i].xh == xh)
      return i;
  }
  return -1;
}

int search_by_xm(Table *tab, const char *xm)
{
  int i;
  for (i = 0; i < tab->len; i++) {
    if (!strcmp(tab->s[i].xm, xm))
      return i;
  }
  return -1;
}

int match_xm(Student *stu, const char *xm)
{
  int i, xml = strlen(xm), sxml = strlen(stu->xm);
  for (i = 0; i < sxml; i++) {
    if (!strncmp(stu->xm + i, xm, xml)) {
      return 1;
    }
  }
  return 0;
}

Table *search_partial_xm(Table *tab, const char *xm)
{
  int i;
  Table *res;
  res = alloc_table();
  for (i = 0; i < tab->len; i++) {
    if (match_xm(&tab->s[i], xm))
      table_add_student(res, &tab->s[i]);
  }
  return res;
}

Table *search_by_cj_range(Table *tab, int cmin, int cmax)
{
  int i;
  Table *res;
  res = alloc_table();
  for (i = 0; i < tab->len; i++) {
    if (tab->s[i].cj >= cmin && tab->s[i].cj <= cmax)
      table_add_student(res, &tab->s[i]);
  }
  return res;
}

void table_modify_student(Table *tab, int i, Student *stu)
{
  memcpy(&tab->s[i], stu, sizeof(Student));
}

void table_delete_student(Table *tab, int i)
{
  memmove(&tab->s[i], &tab->s[i + 1],
      (tab->len - i) * sizeof(Student));
}

void table_add_student(Table *tab, Student *stu)
{
  tab->s = realloc(tab->s, ++tab->len * sizeof(Student));
  table_modify_student(tab, tab->len - 1, stu);
}

static int dotsub(Student *a, Student *b, StuKey *k)
{
  return (a->xh - b->xh) * k->xh + (a->xb - b->xb) * k->xb
    + (a->cj - b->cj) * k->cj + strcmp(a->xm, b->xm) * k->xm;
}

void sort_table(Table *tab, StuKey *k)
{
  int i, j, a[tab->len], tmp;
  Student ns[tab->len];
  // 避免一次次memcpy大结构Student的常数开销
  for (i = 0; i < tab->len; i++) {
    a[i] = i;
  }
  memcpy(ns, tab->s, sizeof(Student) * tab->len);
  for (i = 0; i < tab->len - 1; i++) {
    for (j = 0; j < tab->len - 1 - i; j++) {
      if (dotsub(&tab->s[a[j]], &tab->s[a[j + 1]], k) < 0) {
        tmp = a[j];
        a[j] = a[j + 1];
        a[j + 1] = tmp;
      }
    }
  }
  for (int i = 0; i < tab->len; i++) {
    memcpy(&tab->s[i], &ns[a[i]], sizeof(Student));
  }
}
