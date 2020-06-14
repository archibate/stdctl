#define _DEBUG
//#define _DEBUG_SORT
#define PGSIZE 10
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
  printf("3: 查看全部学生\t");
  printf("4: 进行数据排序\t");
  printf("5: 显示统计信息\n\t");
  printf("6: 成绩范围查找\t");
  printf("7: 姓名模糊查找\t");
  printf("q: 保存并退出\n\t");
  printf("\n");
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
  printf("| %4d | %s | %-8s | %4d |\n",
      stu->xh, xb_t[stu->xb], stu->xm, stu->cj);
}

void print_table(Table *tab)
{
  int i, c, r, pages;
  if (tab->len == 0) {
    strcpy(statbar, "没有结果！");
    return;
  }
  clear();
  print_tab_head();
  pages = (tab->len + PGSIZE - 1) / PGSIZE;
  for (i = 0; i < tab->len; i++) {
    print_student(&tab->s[i]);
    if (i % PGSIZE == PGSIZE - 1 || i == tab->len - 1) {
      print_tab_tail();
      printf("第%d/%d页：上/下翻页(k/j)，"
          "输入数字快速跳转，退出(q)..", i / PGSIZE + 1, pages);
      c = getch();
      switch (c) {
      case 'k':
        // 多减1可以抵消for里的i++
        i -= i % PGSIZE;
        i -= PGSIZE + 1;
        if (i < -1)
          i = -1;
      case 'j':
        // 直接让for帮我i++就行了
        break;
      case 'q':
        return;
      default:
        if (isdigit(c)) {
          printf("\n快速跳转至哪一页？%c", c);
          r = c - '0';
          while (isdigit(c = getch())) {
            printf("%c", c);
            r *= 10;
            r += c - '0';
          }
          r = (r - 1) * PGSIZE;
          if (r < 0 || r >= tab->len) {
            printf("\t页码错误！请输入1~%d！", pages);
            getch();
          } else {
            i = r - 1; // 减1抵消for里的i++
          }
        }
      }
      clear();
      print_tab_head();
    }
  }
  print_tab_tail();
}

static int run_init_student(Student *stu)
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
  printf("\r=== 录入新学生 ===\n");
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
    table_delete_student(tab, i);
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

static void print_statics(Table *tab)
{
  Student *s;
  int i, tmp;
  struct {
    float cj, hg;
    int n;
  } sum[3], *p;
  memset(sum, 0, sizeof(sum));
  for (i = 0; i < tab->len; i++) {
    s = &tab->s[i];
    p = &sum[!!s->xb];
    p->cj += s->cj;
    p->hg += s->cj >= 60;
    p->n++;
  }
  sum[2].cj = sum[0].cj + sum[1].cj;
  sum[2].hg = sum[0].hg + sum[1].hg;
  sum[2].n = sum[0].n + sum[1].n;
  for (i = 0; i < 3; i++) {
    sum[i].cj /= sum[i].n;
    sum[i].hg *= 100.0 / sum[i].n;
  }
  printf("+----------+--------+--------+--------+\n");
  printf("|   项目   |  男生  |  女生  |  总计  |\n");
  printf("+----------+--------+--------+--------+\n");
  printf("| 平均成绩 | %6.2f | %6.2f | %6.2f |\n",
      sum[0].cj, sum[1].cj, sum[2].cj);
  printf("| 合格率 % | %6.2f | %6.2f | %6.2f |\n",
      sum[0].hg, sum[1].hg, sum[2].hg);
  printf("+----------+--------+--------+--------+\n");
  printf("按任意键继续...");
  getch();
}

static void run_search_by_cj_range(void)
{
  Table *res;
  int cmax, cmin;
  printf("输入成绩下限：");
  scanf("%d", &cmin);
  printf("输入成绩上限：");
  scanf("%d", &cmax);
  getchar(); // 防止多余enter传给getch
  clear();
  res = search_by_cj_range(tab, cmin, cmax);
  print_table(res);
  free_table(res);
}

static void run_search_partial_xm(void)
{
  Table *res;
  char xm[10];
  printf("输入部分姓名：");
  scanf("%s", xm);
  getchar(); // 防止多余enter传给getch
  clear();
  res = search_partial_xm(tab, xm);
  print_table(res);
  free_table(res);
}

static void run_sort_table(void)
{
  StuKey k;
#ifdef _DEBUG_SORT
  memset(&k, 0, sizeof(k));
  k.xb = -100;
  k.cj = 1;
#else
  printf("输入学号权重：");
  scanf("%d", &k.xh);
  printf("输入性别权重：");
  scanf("%d", &k.xb);
  printf("输入成绩权重：");
  scanf("%d", &k.cj);
  printf("输入姓名权重：");
  scanf("%d", &k.xm);
  getchar(); // 防止多余enter传给getch
#endif
  sort_table(tab, &k);
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
  case '3':
    print_table(tab);
    break;
  case '4':
    run_sort_table();
    break;
  case '5':
    print_statics(tab);
    break;
  case '6':
    run_search_by_cj_range();
    break;
  case '7':
    run_search_partial_xm();
    break;
  case 'q':
    over = 1;
  }
}

int main(void)
{
  int c;
  char fname[233];

#ifndef _WIN32
  setbuf(stdout, NULL);
  setbuf(stdin, NULL);
#endif

#ifdef _DEBUG
  strcpy(fname, "table.txt");
#else
  printf("=== 载入数据 ===\n");
  printf("请输入数据文件名：");
  scanf("%s", fname);
  getchar(); // 防止多余的'\n'漏到getch里
  printf("载入中...\n");
#endif
  tab = load_table(fname);
  if (tab == NULL) {
    printf("载入失败，请检查文件是否存在\n");
    return -1;
  }
  strcpy(statbar, "载入成功");

  while (!over) {
    show_main_hint();
    c = getch();
    statbar[0] = 0;
    run_main_command(c);
  }
}
