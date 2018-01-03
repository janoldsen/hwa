#ifndef TIME_H
#define TIME_H


typedef unsigned long time;

#define SECONDS(s) (s)
#define MINUTES(m) (SECONDS(m * 60))
#define HOURS(h) (MINUTES(h * 60))
#define DAYS(d) (HOURS(24 * d))

#define TO_SECONDS(time) (time)
#define TO_MINUTES(time) (TO_SECONDS(time) / 60)
#define TO_HOURS(time) (TO_MINUTES(time) / 60)
#define TO_DAYS(time) (TO_HOURS(time) / 24)

#endif
