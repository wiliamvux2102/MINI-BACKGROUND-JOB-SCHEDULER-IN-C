#include "queue.h"
#include <stdlib.h>

static int sjf_before(job_t *a, job_t *b) {
    if (!a || !b) return 0;
    if (a->estimated_runtime != b->estimated_runtime) {
        return a->estimated_runtime < b->estimated_runtime;
    }
    return a->arrival_time < b->arrival_time;
}

void queue_init(queue_t * q) {
    if (!q) return;
    q->head = q->tail = NULL;
    q->size = 0;
}

void queue_destroy(queue_t * q) {
    if (!q) return;
    job_node_t * curr = q->head;
    while (curr) {
        job_node_t * next = curr->next_job;
        free (curr);
        curr = next;
    }
    q->head = q->tail = NULL;
    q->size = 0;
}

int enqueue(queue_t *q, job_t * j) {
    if (!q || !j) return 0;
    job_node_t * node = (job_node_t *)malloc(sizeof(job_node_t));
    node->job = j; node->next_job = NULL;
    if (q->tail == NULL) {
        q->head = q->tail = node;
    } else {
        q->tail->next_job = node;
        q->tail = node;
    } q->size+= 1; return 1;
}

job_t * dequeue (queue_t * q) {
    if (!q || !q->head) return NULL;
    job_node_t * node = q->head; job_t * j= node->job;
    q->head = node->next_job;
    if (q->head == NULL) q->tail = NULL;
    free(node);
    q->size--;
    return j;
}

int sorted_insert_sjf(queue_t *q, job_t *j) {
    if (!q || !j) return 0;

    job_node_t *node = (job_node_t *)malloc(sizeof(job_node_t));
    if (!node) return 0;

    node->job = j;
    node->next_job = NULL;

    if (q->head == NULL || sjf_before(j, q->head->job)) {
        node->next_job = q->head;
        q->head = node;
        if (q->tail == NULL) q->tail = node;
        q->size++;
        return 1;
    }

    job_node_t *cur = q->head;
    while (cur->next_job && !sjf_before(j, cur->next_job->job)) {
        cur = cur->next_job;
    }

    node->next_job = cur->next_job;
    cur->next_job = node;

    if (node->next_job == NULL) q->tail = node;

    q->size++;
    return 1;
}

static int priority_before(job_t *a, job_t *b) {
    if (!a || !b) return 0;
    if (a->priority != b->priority) {
        return a->priority < b->priority;
    }
    return a->arrival_time < b->arrival_time;
}

int sorted_insert_priority(queue_t *q, job_t *j) {
    if (!q || !j) return 0;

    job_node_t *node = (job_node_t *)malloc(sizeof(job_node_t));
    if (!node) return 0;

    node->job = j;
    node->next_job = NULL;

    if (q->head == NULL || priority_before(j, q->head->job)) {
        node->next_job = q->head;
        q->head = node;
        if (q->tail == NULL) q->tail = node;
        q->size++;
        return 1;
    }

    job_node_t *cur = q->head;
    while (cur->next_job && !priority_before(j, cur->next_job->job)) {
        cur = cur->next_job;
    }

    node->next_job = cur->next_job;
    cur->next_job = node;

    if (node->next_job == NULL) q->tail = node;

    q->size++;
    return 1;
}