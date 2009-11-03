
#ifndef TIME_UTILS_HPP

static unsigned long long getTime()
{
  timespec tp;
  clock_gettime(CLOCK_REALTIME, &tp);
  return 1e9 * tp.tv_sec + tp.tv_nsec;
}

#define TIME_UTILS_HPP

#endif
