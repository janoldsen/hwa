#include "hwa.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_TASKS 4096
#define MAX_RELEVANT_TASKS 128
#define MAX_TASKS_PER_DAY 32


struct task
{
  char *name;
  unsigned min_days;
  unsigned max_days;

  time duration;
  unsigned room;
  
  unsigned last_update;
};

struct relevant_task
{
  struct task *task;
  struct task *prev_task;
  time time;
  unsigned add_cost;
  unsigned cost;
};


#define ALPHA 1.0
#define BETA 0.5

unsigned cost(time time, unsigned add_cost, time budget)
{
  return TO_SECONDS(abs(budget-time)) * ALPHA + add_cost * BETA; 
}



struct task tasks[MAX_TASKS];
unsigned num_tasks;

unsigned curr_day;

struct relevant_task relevant_tasks[MAX_RELEVANT_TASKS];
unsigned num_relevant_tasks;
struct task *task_list[MAX_TASKS_PER_DAY];
unsigned task_list_length;


time days_budget[7];

int init_hwa()
{
 
  num_tasks = 0;

  curr_day = 0;
}

int register_task( char *name, unsigned min_days, unsigned max_days, unsigned offset_days )
{
  if (num_tasks == MAX_TASKS)
    return MAX_TASKS_EXCEEDED;



  struct task *task = tasks + num_tasks++;

  for( unsigned i = 0; i < num_tasks - 1; ++i )
    {
      if ( strcmp( tasks[i].name, name) == 0 )
	{
	  /* task already exists */
	  task = tasks + i;
	  num_tasks--;
	  break;
	}
    }
  
  task->name = name;
  task->min_days = min_days;
  task->max_days = max_days;
  task->last_update = offset_days; /* TODO probably wrong*/
   
  return SUCCESS;
}

int remove_task( char *name )
{
  for( unsigned i = 0; i < num_tasks; ++i )
    {
      if ( strcmp( tasks[i].name, name) == 0 )
	{
	  tasks[i] = tasks[--num_tasks];	 
	  return SUCCESS;
	}
    }

  return TASK_NOT_FOUND;
}


struct search_front
{
  struct relevant_task *front[MAX_RELEVANT_TASKS];
  unsigned size;
};


void push_front(struct relevant_task *task, struct search_front *search_front)
{

  unsigned pos = search_front->size;
  /* TODO binary search */
  for (; pos + 1 > 1; --pos)
    {
      if (search_front->front[pos-1]->cost > task->cost)
	break;
    }

  memmove(search_front + pos+1, search_front + pos, (search_front->size - pos) / sizeof(*search_front->front));

  search_front->front[pos] = task;
  search_front->size++;
}

 


struct relevant_task *pop_front(struct search_front *search_front)
{
  return search_front->front[--search_front->size];
}

static int fill_relevant_tasks( struct relevant_task *search_front)
{
  num_relevant_tasks = 0;
  task_list_length = 0;
  
  signed highest_priority = 356;
  struct relevant_task *most_relevant_task = NULL;

  for(unsigned i = 0; i < num_tasks; ++i)
    {
      if ( curr_day - tasks[i].last_update > tasks[i].min_days)
	{
	  if (num_relevant_tasks >= MAX_RELEVANT_TASKS)
	    break;
	  
	  relevant_tasks[num_relevant_tasks++] = (struct relevant_task){
	    .task = tasks + i,
	    .prev_task = NULL,
            .time = 0,
            .add_cost = UINT_MAX,
            .cost = UINT_MAX,
	  };

	  signed priority = tasks[i].max_days - (curr_day - tasks[i].last_update);

	  if ( priority < highest_priority )
	    {
 	      most_relevant_task = relevant_tasks + num_relevant_tasks-1;
	      priority = highest_priority;
	    }

	  if (priority <= 0)
	    {
	      task_list[task_list_length++] = tasks + i; 
	    }
	}
    }
    
  if (most_relevant_task != NULL)
    {
      search_front[0] = most_relevant_task;  
    }
  
  return most_relevant_task != NULL;
}



static void find_task_path(struct search_front *search_front)
{
  time budget = days_budget[curr_day % 7];

  /* subtract the MUST DO tasks from the currents days budget */
  for (unsigned i = 0; i < task_list_length; ++i)
    {
      if(budget < task_list[i]->duration)
	budget = 0;
      else
	budget -= task_list[i]->duration;
    }
    
  
  if (budget > 0)
    {
      struct relevant_task *first_rel = search_front->front[0]; 
      
      first_rel->time = first_rel->task->duration;
      first_rel->add_cost = 0;
      first_rel->cost = cost(first_rel->time, first_rel->cost, budget);
      first_rel->
      
      unsigned min_cost = UINT_MAX;
      struct relevant_task* min_rel;
      
      while(front_size > 0)
	{
	  struct relevant_task *rel = pop(search_front);

	  for (unsigned i = 0; i < 
	  

	}
    }
}


int new_day()
{
  curr_day++;

  struct search_front *search_front = (struct search_front){.size = 0};
  
  if(fill_relevant_tasks(&search_front))
    {
      find_task_path(&search_front);
    }
}
