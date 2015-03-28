static int testint(const void *a, const void *b)
{
   int x = *(const int*)a, y = *(const int*)b;
   if(x < y)
      return -1;
   if(x > y)
      return 1;
   return 0;
}

static void sortints(int *data, int count)
{
   qsort(data, count, sizeof *data, testint);
}

static int getavg(int *data, int minind, int maxind, int count)
{
   int i, total = 0;
   for(i = minind; i<maxind; i++)
      total += data[i];
   
   return (int)((double)total / (maxind - minind) + .5);
}
