#include "stdctl.h"

static void load_student(FILE *fp, Student *stu)
{
  fscanf(fp, "%d %d %d %s", &stu->xh, &stu->xb, &stu->cj, stu->xm);
}

Table *load_table(const char *filename)
{
  Table *tab;
  FILE *fp;
  int i;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    perror(filename);
    return NULL;
  }
  tab = malloc(sizeof(Table));
  fscanf(fp, "%d", &tab->len);
  tab->s = malloc(sizeof(Student) * tab->len);
  for (i = 0; i < tab->len; i++) {
    load_student(fp, &tab->s[i]);
  }
  fclose(fp);
  return tab;
}

Table *alloc_table(void)
{
  Table *tab;
  tab = malloc(sizeof(Table));
  tab->len = 0;
  tab->s = calloc(sizeof(Student), 1);
  return tab;
}

void free_table(Table *tab)
{
  free(tab->s);
  free(tab);
}
