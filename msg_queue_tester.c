/*
 * Zachary S. Porter
 * CSCI 4100 Assignment 3
 * This program tests a message queue by creating
 * multiple sending and receiving threads.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "msg_queue.h"

 /* Structure to hold thread arguments */
struct thread_args {
	int t_id;
	int num_msgs;
	struct msg_queue *mq;

};

/* Function prototypes */
void thread_args_init(struct thread_args *args, int t_id, int num_msgs,
	struct msg_queue *mq);
void * send_msgs(void *args);
void * receive_msgs(void *args);

/* Main function */
int main(int argc, char *argv[]) {

	pthread_t *sender;
	pthread_t *receiver;

	struct msg_queue *new_mq;
	mq_init(new_mq);

	struct thread_args *new_args;
	thread_args_init(new_args, 2, argc, new_mq);

	int i;
	for (i = 0; i < argc; i++) {
		pthread_create(sender, NULL, send_msgs, &new_args);
		pthread_create(receiver, NULL, receive_msgs, &new_args);
		pthread_join(*sender, NULL);
		pthread_join(*receiver, NULL);
	}

	return 0;
}

/* Initialize thread_args structure */
void thread_args_init(struct thread_args *args, int t_id, int num_msgs,
	struct msg_queue *mq) {

	// Initialize Variables
	struct thread_args *ptr = args;
	ptr->t_id = t_id;
	ptr->num_msgs = num_msgs;
	ptr->mq = mq;

}

/* Function for sending thread to execute */
void * send_msgs(void *args) {
	struct thread_args *real_args = (struct thread_args *) args;

	int i;
	for (i = 0; i < real_args->num_msgs; i++) {
		struct mq_msg *msg_sent;
		msg_sent->t_id = i;
		msg_sent->m_id = i;
		mq_send(real_args->mq, msg_sent);
	}

	return NULL;
}

/* Function for receiving thread to execute */
void * receive_msgs(void *args) {
	struct thread_args *real_args = (struct thread_args *) args;

	int i;
	for (i = 0; i < real_args->num_msgs; i++) {
		struct mq_msg *msg_receive;
		msg_receive->t_id = i;
		msg_receive->m_id = i;
		mq_receive(real_args->mq, msg_receive);
		printf("[Sender: ", real_args->mq->messages->t_id);
		printf(", receiver: %d", msg_receive->t_id, ", msg_id: ", msg_receive->m_id, "]");
	}

	return NULL;
}
