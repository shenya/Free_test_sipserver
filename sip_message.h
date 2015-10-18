#ifndef __SIP_MESSAGE_H__
#define __SIP_MESSAGE_H__

#define HEADER_NAME_LEN 32
#define HEADER_VALUE_LEN 256
#define HEADER_MAX_NUM 64
#define BODY_LEN 1024
#define PADDING_LEN 3
#define MESSAGE_MAX_COUNT 36

typedef struct sip_header {
    char name[HEADER_NAME_LEN];
    int used_flag;
    char header_value[HEADER_VALUE_LEN];
}sip_header;

typedef struct tx_message {
    sip_header header[HEADER_MAX_NUM];
    char padding[PADDING_LEN]; // \r\n
    char body[BODY_LEN];
    int cur_index; // current index of header array.
    int used_flag;
}tx_message_t;

typedef struct rx_message {
    sip_header header[HEADER_MAX_NUM];
    char padding[PADDING_LEN]; // \r\n
    char body[BODY_LEN];
    int cur_index; // current index of header array.
    int used_flag;
}rx_message_t;

typedef struct tx_manager {
    tx_message_t tx_msg[MESSAGE_MAX_COUNT];
    int cur_index;
}tx_manager_t;

typedef struct rx_manager {
    rx_message_t rx_msg[MESSAGE_MAX_COUNT];
    int cur_index;
}rx_manager_t;

extern tx_manager_t *tx_manager;
extern rx_manager_t *rx_manager;

extern int message_manager_init(tx_manager_t **tx_manager, rx_manager_t **rx_manager);

//extern int tx_message_get_from_file(char *file_name, char *buf, int len);

extern int rx_message_parse(rx_message_t *rx_msg, char *buf, int len);

extern int tx_message_parse(tx_message_t *tx_msg, char *buf, int len);

extern rx_message_t *get_rx_message(void);

extern tx_message_t *get_tx_message(void);

extern int rx_manager_display(int detail_flag);
#endif
