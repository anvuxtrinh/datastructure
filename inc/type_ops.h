typedef void* (*data_clone_cb_t)(const void *elem);
typedef void  (*data_free_cb_t)(void *elem);

typedef struct data_ops {
    data_clone_cb_t clone;
    data_free_cb_t free;
} data_ops_t;