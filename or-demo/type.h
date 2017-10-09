/**
 * types
 *
 */

#ifndef _OR_TYPE_H_
#define _OR_TYPE_H_


/**
 * the call back for interupt
 * data buffer and int
 */
typedef void (*callback_task_func_t)(void*,int);
struct callback_task {
  callback_task_func_t callback_task_func;
  void* callback_task_data;
  int   callback_task_data_size;
};

#endif // _OR_TYPE_H_
