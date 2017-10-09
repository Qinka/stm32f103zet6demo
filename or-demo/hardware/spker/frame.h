/**
 * The frame for speaker
 *
 * head file
 */

#ifndef _SPKER_FRAME_H_
#define _SPKER_FRAME_H_

struct spker_frame {
  int frame_size;
  char* frame_data;
};

void fill_spker_frame(struct spker_frame* frame,void *data,int size);

#endif // _SPKER_FRAME_H_
