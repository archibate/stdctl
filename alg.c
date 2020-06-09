#include "stdctl.h"

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

void table_modify_student(Table *tab, int i, Student *stu)
{
  memcpy(&tab->s[tab->len], stu, sizeof(Student));
}

void table_add_student(Table *tab, Student *stu)
{
  tab->s = realloc(tab->s, ++tab->len);
  table_modify_student(tab, tab->len - 1, stu);
}
