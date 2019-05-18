#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "types.h"

void create_name_fifo(char* final, pid_t pid);
int read_fifo_server(tlv_request_t* t);
void read_fifo_answer(char* name, tlv_reply_t* t);
int write_fifo_server(tlv_request_t* to_write, ret_code_t *a);
void write_fifo_answer(char* name, tlv_reply_t* to_write);

#endif /* _COMMUNICATION_H_ */