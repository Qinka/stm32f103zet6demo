#include <aiko/job/job_s.h>
#include <hardware/beep.h>

void sys_initialization_f(struct job_t*);
void job_initialization_f(struct job_t*);

struct job_t sys_initialization = {
  job_method: sys_initialization_f,
  job_data_p: 0,
  job_data_s: 0,
  next_job_p: 0
};
struct job_t job_initialization = {
  job_method: job_initialization_f,
  job_data_p: 0,
  job_data_s: 0,
  next_job_p: 0
};

void job_initialization_f(struct job_t* j) {
  beep_off();
  return;
}

void sys_initialization_f(struct job_t* j) {
  beep_init();
  beep_on();
  return;
}