typedef enum{
    RUNNING = 0;
    READY = 1;
    BLOCKED = 2;
    TERMINATED = 3;
    INNACTIVE = 4;
}task_state_t;

typedef enum{
    HIGHEST;
    HIGH;
    MED_HIGH;
    MED;
    MED_LOW;
    LOW;
    LOWEST;
    IDLE;
}priority_t;

typedef struct{
    uint32_t task_id;
    void *stack_pointer;
    task_state_t state;
    uint8_t priority;
    tcb_t *list_pointer;
//    uint32_t delay_time; // If you want to implement task delays maybe
    // EVENT FLAGS... used to unblock task from blocked state
    void *stack_base_address;
    void *stack_upper_address;
    uint32_t stack_size;
}tcb_t;

typedef struct{
    tcb_t* head;
    tcb_t* tail;
    uint32_t size;
}tcb_list_t;

typedef struct{
    tcb_t* running_task;
    tcb_list_t ready_lists[8];
    tcb_list_t blocked_list; // should be priority queue based on first task to be unblocked
    tcb_list_t terminated_list;
    tcb_list_t inactive_list;
}scheduler_t;