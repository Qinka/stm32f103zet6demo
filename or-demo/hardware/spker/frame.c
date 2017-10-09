/**
 * the frame for speaker
 *
 * source
 */

#include "frame.h"

/**
 * fill a frame
 * @param spker_frame the frame item
 * @param data the buf of data
 * @param size size of buf
 */
void fill_spker_frame(struct spker_frame* frame,void *data,int size) {
  frame -> frame_size = size;
  frame -> frame_data = data;
}
