#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define COL_WARN "#b58900"
#define COL_CRIT "#dc322f"
#define COL_SAFE "#93a1a1"

int get(unsigned long *c_idle, unsigned long *tot_cpu_since_boot)
{
  FILE *stat = fopen("/proc/stat", "r");
  if (!stat) {
    perror("fopen");
    return EXIT_FAILURE;
  }

  char buf[128];
  if (!fgets(buf, 128, stat)) {
    perror("fgets");
    return EXIT_FAILURE;
  }

  // NOTE: guest and guest_nice are included in user and nice
  unsigned long c_user, c_nice, c_system, c_iowait, c_irq, c_softirq, c_steal;
  char c_cpu[8];
  sscanf(buf, "%s %lu %lu %lu %lu %lu %lu %lu %lu",
         c_cpu, &c_user, &c_nice, &c_system, c_idle, &c_iowait, &c_irq, &c_softirq, &c_steal);

  *tot_cpu_since_boot = c_user + c_nice + c_system + *c_idle + c_iowait + c_irq + c_softirq + c_steal;
  return EXIT_SUCCESS;
}

int main()
{
  unsigned long idle1, idle2, total1, total2;
  get(&idle1, &total1);
  usleep(500*1000);
  get(&idle2, &total2);

  unsigned long d_idle = idle2 - idle1;
  unsigned long d_total = total2 - total1;
  double tot_cpu_percent = 100 * (d_total - d_idle) / (double)d_total;

  char *color;
  if (tot_cpu_percent > 350)
    color = COL_CRIT;
  else if (tot_cpu_percent > 99)
    color = COL_WARN;
  else
    color = COL_SAFE;

  /* i3blocks pango format */
  // XXX number of logical cores
  printf("<span foreground=\"%s\"> %.0f%%</span>\n", color, tot_cpu_percent*8);
  return 0;
}
