#include "stdctl.h"
#ifdef _WIN32
#include <conio.h>
#define clear() system("cls")
#else
#include <termios.h>
#define clear() printf("\033[H\033[2J\033[3J")
int getch(void) {
  int c;
  struct termios old, curr;
  tcgetattr(0, &old);
  memcpy(&curr, &old, sizeof(struct termios));
  curr.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(0, TCSANOW, &curr);
  c = getchar();
  tcsetattr(0, TCSANOW, &old);
  return c;
}
int getche(void) {
  int c;
  struct termios old, curr;
  tcgetattr(0, &old);
  memcpy(&curr, &old, sizeof(struct termios));
  curr.c_lflag &= ~ICANON;
  tcsetattr(0, TCSANOW, &curr);
  c = getchar();
  tcsetattr(0, TCSANOW, &old);
  return c;
}
#endif

static Table *tab;
static char statbar[233];
static int over = 0;

static void show_main_hint(void)
{
  clear();
  printf("欢迎来到学生信息管理系统\n");
  printf("请按键以选择功能:\n");
  printf("\n\t");
  printf("0: 录入新学生\t");
  printf("1: 按学号查找\t");
  printf("2: 按姓名查找\n\t");
  printf("3: 打印全部学生\t");
  printf("4: 进行数据排序\t");
  printf("5: 显示成绩统计\n\t");
  printf("q: 保存并退出\n\t");
  printf("\n");
  print_table(tab);
  printf("%s\n", statbar);
}

void print_tab_tail(void)
{
  printf("+------+------+--------+------+\n");
}

void print_tab_head(void)
{
  print_tab_tail();
  printf("| 学号 | 性别 |  姓名  | 成绩 |\n");
  print_tab_tail();
}

void print_student(Student *stu)
{
  char xb_t[][6] = {" 男 ", " 女 "};
  printf("| %4d | %s | %-8s | %4d |\n", stu->xh, xb_t[stu->xb], stu->xm, stu->cj);
}

void print_table(Table *tab)
{
  int i;
  print_tab_head();
  for (i = 0; i < tab->len; i++) {
    print_student(&tab->s[i]);
  }
  print_tab_tail();
}

int run_init_student(Student *stu)
{
  printf("请输入学号：");
  scanf("%d", &stu->xh);
  if (stu->xh <= 0) {
invalid:
    strcpy(statbar, "参数错误！");
    getchar(); // 防止多余的'\n'漏到getch里
    return 0;
  }
  printf("请输入性别（男=0, 女=1）：");
  scanf("%d", &stu->xb);
  if (stu->xb != 0 && stu->xb != 1) {
    sprintf(statbar, "!!%d!!", stu->xb);
    goto invalid;
  }
  printf("请输入姓名：");
  scanf("%s", &stu->xm);
  printf("请输入成绩：");
  scanf("%d", &stu->cj);
  if (stu->cj < 0 || stu->cj > 100) {
    goto invalid;
  }
  getchar(); // 防止多余的'\n'漏到getch里
  return 1;
}

static void run_new_student(void)
{
  Student stu;
  printf("\r=== 录入新学生 === \n");
  if (!run_init_student(&stu))
    return;
  print_tab_head();
  print_student(&stu);
  print_tab_tail();
  printf("确认录入？ (y/n)");
  if (getch() == 'n') {
    strcpy(statbar, "录入未确认");
    return;
  }
  table_add_student(tab, &stu);
  strcpy(statbar, "学生信息录入成功");
}

static void run_modify_student(int i)
{
  Student stu;
  printf("\r=== 修改个人信息 === \n");
  if (!run_init_student(&stu))
    return;
  print_tab_head();
  print_student(&stu);
  print_tab_tail();
  printf("确认修改？ (y/n)");
  if (getch() == 'n') {
    strcpy(statbar, "修改未确认");
    return;
  }
  table_modify_student(tab, i, &stu);
  strcpy(statbar, "个人信息修改成功");
}

static void run_search_result(int i)
{
  int c;
  if (i == -1) {
    strcpy(statbar, "查无此人！");
    return;

  } else {
    printf("查到如下学生：\n");
    print_tab_head();
    print_student(&tab->s[i]);
    print_tab_tail();
  }
  printf("0: 确定\t1: 修改\t2: 删除\n");
  printf("\n要做什么？");
  c = getch();
  switch (c) {
  case '1':
    run_modify_student(i);
    break;
  case '2':
    printf("\r确认删除: %s？(y/n)", tab->s[i].xm);
    if (getch() != 'y') {
      strcpy(statbar, "删除未确认");
      break;
    }
    strcpy(statbar, "删除成功");
    break;
  default:
    strcpy(statbar, "参数错误");
  case '0':
    break;
  }
}

static void run_search_by_xh(void)
{
  int i, xh;
  printf("请输入要查找的学号：");
  scanf("%d", &xh);
  getchar(); // 防止多余的'\n'漏到getch里
  i = search_by_xh(tab, xh);
  run_search_result(i);
}

static void run_search_by_xm(void)
{
  int i;
  char xm[12];
  printf("请输入要查找的姓名：");
  scanf("%s", &xm);
  getchar(); // 防止多余的'\n'漏到getch里
  i = search_by_xm(tab, xm);
  run_search_result(i);
}

static void run_main_command(int c)
{
  switch (c) {
  case '0':
    run_new_student();
    break;
  case '1':
    run_search_by_xh();
    break;
  case '2':
    run_search_by_xm();
    break;
  case 'q':
    over = 1;
  }
}

int main(void)
{
  int c;
#ifndef _WIN32
  setbuf(stdout, NULL);
  setbuf(stdin, NULL);
#endif

  tab = load_table("table.txt");
  if (tab == NULL)
    return -1;

  while (!over) {
    show_main_hint();
    c = getch();
    statbar[0] = 0;
    run_main_command(c);
  }
}
