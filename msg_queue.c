/* Zachary S. Porter
 * CSCI 4100 Assignment 3
 * Source file for message queue implementation
 */
#include "msg_queue.h"

 /* Initialize the message queue */
void mq_init(struct msg_queue * mq) {
	mq->in = 0;
	mq->out = 0;

	pthread_mutex_init(&mq->lock, NULL);
	pthread_cond_init(&mq->msg_added, NULL);
	pthread_cond_init(&mq->msg_removed, NULL);
}

/* Send a message to a message queue */
void mq_send(struct msg_queue * mq, struct mq_msg * message) {
	pthread_mutex_lock(&mq->lock);

	while ((mq->in - mq->out) == QUEUE_SIZE) {
		pthread_cond_wait(&mq->msg_removed, &mq->lock);
	}

	mq->messages[QUEUE_SIZE % mq->in] = *message;
	mq->in++;

	pthread_cond_signal(&mq->msg_added);
	pthread_mutex_unlock(&mq->lock);
}

/* Receive a message from a message queue */
void mq_receive(struct msg_queue * mq, struct mq_msg * message) {
	pthread_mutex_lock(&mq->lock);

	while ((mq->in - mq->out) == 0) {
		pthread_cond_wait(&mq->msg_added, &mq->lock);
	}

	mq->messages[mq->out % QUEUE_SIZE] = *message;
	mq->out++;

	pthread_cond_signal(&mq->msg_removed);
	pthread_mutex_unlock(&mq->lock);
}
