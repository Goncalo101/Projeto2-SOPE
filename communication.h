#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "types.h"

void read_fifo_server(char* path, tlv_request_t* t);
void read_fifo_answer(char* path);
void write_fifo_server(char* path, tlv_request_t* to_write);
void write_fifo_answer(char* path, tlv_reply_t* to_write);

#endif /* _COMMUNICATION_H_ */