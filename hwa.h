#ifndef HWA_H
#define HWA_H

#include "time.h"



enum return_values
  {
    SUCCESS,
    MORE_TASKS,
    MAX_TASKS_EXCEEDED,
    TASK_NOT_FOUND,
  };


int init_hwa();

int register_task( char *name, unsigned min_days, unsigned max_days, unsigned offset_days );

int remove_task( char *name );

int new_day();

int get_task_list( char *tasks[], unsigned max_tasks );

int resolve_task( char *task);

#endif
