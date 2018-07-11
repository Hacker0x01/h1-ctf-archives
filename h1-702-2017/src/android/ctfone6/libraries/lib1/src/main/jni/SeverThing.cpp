//
// Created by Christopher Thompson on 6/28/17.
//

#include "SeverThing.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "md5.h"
#include <android/log.h>
#include <errno.h>

//#define  LOG_TAG    "ServerThing"
//#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
//#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGI(...)
#define LOGE(...)

int flag_char_hashes[35][16] = {
        {0xfc, 0x7b, 0x1a, 0x4d, 0x8b, 0xed, 0xd6, 0xbc, 0xdf, 0xe0, 0x39, 0x22, 0xc4, 0x51, 0x5e, 0xcc},
        {0xe2, 0x24, 0xde, 0x07, 0x56, 0x6c, 0x18, 0x42, 0xca, 0x70, 0xd7, 0xb8, 0xfc, 0x31, 0x33, 0x36},
        {0x2d, 0x8a, 0x44, 0x45, 0x86, 0x46, 0x88, 0x1e, 0x32, 0x5a, 0x0a, 0x20, 0x5f, 0x78, 0x10, 0x3d},
        {0x55, 0x0f, 0x14, 0xbd, 0x66, 0x59, 0xe3, 0xa3, 0x63, 0xf0, 0xcc, 0x6f, 0xc5, 0x70, 0x5d, 0x1f},
        {0x79, 0x89, 0xeb, 0xd9, 0x84, 0x4e, 0xd9, 0x4a, 0xda, 0x6c, 0xaf, 0x5d, 0xbb, 0xe0, 0x0e, 0x08},
        {0x56, 0x53, 0x7d, 0x76, 0x49, 0x47, 0xb9, 0xbf, 0x6f, 0x5b, 0x9a, 0xab, 0xed, 0xc6, 0x2e, 0x11},
        {0x11, 0x77, 0xbb, 0x74, 0xba, 0x3f, 0x14, 0xe4, 0x26, 0xf0, 0x53, 0xa4, 0x93, 0xd4, 0xa7, 0x06},
        {0x35, 0x05, 0x0e, 0x84, 0x60, 0x65, 0xbd, 0xd1, 0x79, 0x9b, 0x31, 0x47, 0x8b, 0x63, 0x4f, 0xa5},
        {0xf3, 0x4e, 0xdd, 0xf9, 0x83, 0x71, 0x0b, 0xbd, 0x31, 0x64, 0x6b, 0xc4, 0xca, 0xda, 0xbc, 0x58},
        {0xea, 0x60, 0xce, 0xd3, 0x2e, 0xd4, 0x40, 0x93, 0x71, 0x07, 0xec, 0xdb, 0x24, 0x95, 0x47, 0x1d},
        {0xac, 0x88, 0x6b, 0x27, 0x20, 0x87, 0xde, 0xdf, 0x7b, 0xbf, 0xba, 0x44, 0xd9, 0xa4, 0xeb, 0x97},
        {0x11, 0x77, 0xbb, 0x74, 0xba, 0x3f, 0x14, 0xe4, 0x26, 0xf0, 0x53, 0xa4, 0x93, 0xd4, 0xa7, 0x06},
        {0x35, 0x05, 0x0e, 0x84, 0x60, 0x65, 0xbd, 0xd1, 0x79, 0x9b, 0x31, 0x47, 0x8b, 0x63, 0x4f, 0xa5},
        {0x0f, 0x86, 0x4d, 0xd5, 0x52, 0x78, 0xb1, 0x58, 0xf9, 0xc7, 0x25, 0xa8, 0x4c, 0xe7, 0xf4, 0x81},
        {0xac, 0x88, 0x6b, 0x27, 0x20, 0x87, 0xde, 0xdf, 0x7b, 0xbf, 0xba, 0x44, 0xd9, 0xa4, 0xeb, 0x97},
        {0x33, 0xd1, 0x2a, 0x3e, 0x55, 0x7d, 0x7c, 0x3f, 0x24, 0x3d, 0x8e, 0x48, 0x57, 0xc3, 0xe1, 0xe7},
        {0xcb, 0x92, 0xcc, 0xce, 0xf0, 0x3d, 0x82, 0xa7, 0x52, 0xe6, 0x5a, 0xe6, 0x6c, 0xd5, 0x8d, 0x08},
        {0xac, 0x88, 0x6b, 0x27, 0x20, 0x87, 0xde, 0xdf, 0x7b, 0xbf, 0xba, 0x44, 0xd9, 0xa4, 0xeb, 0x97},
        {0x70, 0xe5, 0xfd, 0xcf, 0xea, 0x95, 0xf8, 0x63, 0x37, 0x09, 0xb7, 0xbc, 0xfc, 0x77, 0x4b, 0x54},
        {0x2d, 0x8a, 0x44, 0x45, 0x86, 0x46, 0x88, 0x1e, 0x32, 0x5a, 0x0a, 0x20, 0x5f, 0x78, 0x10, 0x3d},
        {0x21, 0xf5, 0x36, 0xa8, 0x1a, 0x40, 0x74, 0xa3, 0x4c, 0x3f, 0x40, 0x88, 0x23, 0xf7, 0x3c, 0x16},
        {0xac, 0x88, 0x6b, 0x27, 0x20, 0x87, 0xde, 0xdf, 0x7b, 0xbf, 0xba, 0x44, 0xd9, 0xa4, 0xeb, 0x97},
        {0xca, 0x74, 0xd5, 0xf2, 0xf6, 0xee, 0xe8, 0x44, 0xd6, 0xdc, 0xe6, 0x95, 0x94, 0xc0, 0xd0, 0x7e},
        {0xea, 0x60, 0xce, 0xd3, 0x2e, 0xd4, 0x40, 0x93, 0x71, 0x07, 0xec, 0xdb, 0x24, 0x95, 0x47, 0x1d},
        {0xac, 0x88, 0x6b, 0x27, 0x20, 0x87, 0xde, 0xdf, 0x7b, 0xbf, 0xba, 0x44, 0xd9, 0xa4, 0xeb, 0x97},
        {0xc7, 0xd4, 0xbb, 0x2a, 0xa9, 0x18, 0x0a, 0xd8, 0x48, 0x1e, 0xc2, 0xff, 0xec, 0x26, 0x18, 0x36},
        {0xf3, 0x4e, 0xdd, 0xf9, 0x83, 0x71, 0x0b, 0xbd, 0x31, 0x64, 0x6b, 0xc4, 0xca, 0xda, 0xbc, 0x58},
        {0xe7, 0xbc, 0x13, 0x61, 0xb9, 0xef, 0x0c, 0x17, 0xff, 0x02, 0x6f, 0x88, 0x13, 0x44, 0x60, 0xe1},
        {0x33, 0xd1, 0x2a, 0x3e, 0x55, 0x7d, 0x7c, 0x3f, 0x24, 0x3d, 0x8e, 0x48, 0x57, 0xc3, 0xe1, 0xe7},
        {0x35, 0x05, 0x0e, 0x84, 0x60, 0x65, 0xbd, 0xd1, 0x79, 0x9b, 0x31, 0x47, 0x8b, 0x63, 0x4f, 0xa5},
        {0xfe, 0xe5, 0x2e, 0xda, 0x9e, 0x51, 0x5f, 0x68, 0x8f, 0x94, 0x9f, 0x0b, 0xfa, 0xb0, 0xbe, 0x37},
        {0xcc, 0xc6, 0x56, 0x46, 0x3d, 0x6d, 0xda, 0x53, 0x2d, 0xf4, 0xd3, 0x7d, 0x38, 0x78, 0x6b, 0xf6},
        {0x1b, 0x6f, 0xfa, 0x71, 0x6e, 0xe4, 0x92, 0xe6, 0x9e, 0x7b, 0x51, 0x68, 0xe2, 0x24, 0x17, 0xa0},
        {0x11, 0x77, 0xbb, 0x74, 0xba, 0x3f, 0x14, 0xe4, 0x26, 0xf0, 0x53, 0xa4, 0x93, 0xd4, 0xa7, 0x06},
        {0xad, 0x46, 0x38, 0xd6, 0x1a, 0x68, 0xbc, 0x8e, 0x98, 0xae, 0x51, 0x36, 0x91, 0x78, 0x60, 0x3e}
};

#define MAX_CLIENTS	100

static unsigned int cli_count = 0;
static int uid = 10;

/* Client structure */
typedef struct {
    struct sockaddr_in addr;	/* Client remote address */
    int connfd;			/* Connection file descriptor */
    int uid;			/* Client unique identifier */
    char name[32];			/* Client name */
} client_t;

client_t *clients[MAX_CLIENTS];

/* Add client to queue */
void queue_add(client_t *cl){
    int i;
    for(i=0;i<MAX_CLIENTS;i++){
        if(!clients[i]){
            clients[i] = cl;
            return;
        }
    }
}

/* Delete client from queue */
void queue_delete(int uid){
    int i;
    for(i=0;i<MAX_CLIENTS;i++){
        if(clients[i]){
            if(clients[i]->uid == uid){
                clients[i] = NULL;
                return;
            }
        }
    }
}

/* Send message to all clients but the sender */
void send_message(char *s, int uid){
    int i;
    for(i=0;i<MAX_CLIENTS;i++){
        if(clients[i]){
            if(clients[i]->uid != uid){
                write(clients[i]->connfd, s, strlen(s));
            }
        }
    }
}

/* Send message to all clients */
void send_message_all(char *s){
    int i;
    for(i=0;i<MAX_CLIENTS;i++){
        if(clients[i]){
            write(clients[i]->connfd, s, strlen(s));
        }
    }
}

/* Send message to sender */
void send_message_self(const char *s, int connfd){
    write(connfd, s, strlen(s));
}

/* Send message to client */
void send_message_client(char *s, int uid, int connfd){
    int i;
    MD5_CTX ctx;
    unsigned char result[16];

    s = strchr(s, ' ') + 1;
    unsigned int len_thing = strlen("gettin it done");
    if (uid == 1337
        && strncmp(s, "gettin it done", len_thing) == 0) {
        for (int i = 0; i < strlen(s); i++) {
            MD5_Init(&ctx);
            MD5_Update(&ctx, s + 1 + len_thing + i, 1);
            MD5_Final(result, &ctx);

            bool match = true;
            for (int j = 0; j < 16; j++) {
                if (flag_char_hashes[i][j] != result[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                send_message_self("Nice one!", connfd);
            }
        }
    }
    for(i=0;i<MAX_CLIENTS;i++){
        if(clients[i]){
            if(clients[i]->uid == uid){
                write(clients[i]->connfd, s, strlen(s));
            }
        }
    }
}

/* Send list of active clients */
void send_active_clients(int connfd){
    int i;
    char s[64];
    for(i=0;i<MAX_CLIENTS;i++){
        if(clients[i]){
            sprintf(s, "<<CLIENT %d | %s\r\n", clients[i]->uid, clients[i]->name);
            send_message_self(s, connfd);
        }
    }
}

/* Strip CRLF */
void strip_newline(char *s){
    while(*s != '\0'){
        if(*s == '\r' || *s == '\n'){
            *s = '\0';
        }
        s++;
    }
}

/* Print ip address */
void print_client_addr(struct sockaddr_in addr){
    LOGI("%d.%d.%d.%d",
           addr.sin_addr.s_addr & 0xFF,
           (addr.sin_addr.s_addr & 0xFF00)>>8,
           (addr.sin_addr.s_addr & 0xFF0000)>>16,
           (addr.sin_addr.s_addr & 0xFF000000)>>24);
}

/* Handle all communication with the client */
void *handle_client(void *arg){
    char buff_out[1024];
    char buff_in[1024];
    int rlen;

    cli_count++;
    client_t *cli = (client_t *)arg;

    LOGI("<<ACCEPT ");
    print_client_addr(cli->addr);
    LOGI(" REFERENCED BY %d\n", cli->uid);

    sprintf(buff_out, "<<JOIN, HELLO %s\r\n", cli->name);
    send_message_all(buff_out);

    /* Receive input from client */
    while((rlen = read(cli->connfd, buff_in, sizeof(buff_in)-1)) > 0){
        buff_in[rlen] = '\0';
        buff_out[0] = '\0';
        strip_newline(buff_in);

        /* Ignore empty buffer */
        if(!strlen(buff_in)){
            continue;
        }

        /* Special options */
        if(buff_in[0] == '\\'){
            char *command, *param;
            command = strtok(buff_in," ");
            if(!strcmp(command, "\\QUIT")){
                break;
            }else if(!strcmp(command, "\\PING")){
                send_message_self("<<PONG\r\n", cli->connfd);
            }else if(!strcmp(command, "\\NAME")){
                param = strtok(NULL, " ");
                if(param){
                    char *old_name = strdup(cli->name);
                    strcpy(cli->name, param);
                    sprintf(buff_out, "<<RENAME, %s TO %s\r\n", old_name, cli->name);
                    free(old_name);
                    send_message_all(buff_out);
                }else{
                    send_message_self("<<NAME CANNOT BE NULL\r\n", cli->connfd);
                }
            }else if(!strcmp(command, "\\PRIVATE")){
                param = strtok(NULL, " ");
                if(param){
                    int uid = atoi(param);
                    param = strtok(NULL, " ");
                    if(param){
                        sprintf(buff_out, "[PM][%s]", cli->name);
                        while(param != NULL){
                            strcat(buff_out, " ");
                            strcat(buff_out, param);
                            param = strtok(NULL, " ");
                        }
                        strcat(buff_out, "\r\n");
                        send_message_client(buff_out, uid, cli->connfd);
                    }else{
                        send_message_self("<<MESSAGE CANNOT BE NULL\r\n", cli->connfd);
                    }
                }else{
                    send_message_self("<<REFERENCE CANNOT BE NULL\r\n", cli->connfd);
                }
            }else if(!strcmp(command, "\\ACTIVE")){
                sprintf(buff_out, "<<CLIENTS %d\r\n", cli_count);
                send_message_self(buff_out, cli->connfd);
                send_active_clients(cli->connfd);
            }else if(!strcmp(command, "\\HELP")){
                strcat(buff_out, "\\QUIT     Quit chatroom\r\n");
                strcat(buff_out, "\\PING     Server test\r\n");
                strcat(buff_out, "\\NAME     <name> Change nickname\r\n");
                strcat(buff_out, "\\PRIVATE  <reference> <message> Send private message\r\n");
                strcat(buff_out, "\\ACTIVE   Show active clients\r\n");
                strcat(buff_out, "\\HELP     Show help\r\n");
                send_message_self(buff_out, cli->connfd);
            }else{
                send_message_self("<<UNKOWN COMMAND\r\n", cli->connfd);
            }
        }else{
            /* Send message */
            sprintf(buff_out, "[%s] %s\r\n", cli->name, buff_in);
            send_message(buff_out, cli->uid);
        }
    }

    /* Close connection */
    close(cli->connfd);
    sprintf(buff_out, "<<LEAVE, BYE %s\r\n", cli->name);
    send_message_all(buff_out);

    /* Delete client from queue and yeild thread */
    queue_delete(cli->uid);
    LOGI("<<LEAVE ");
    print_client_addr(cli->addr);
    LOGI(" REFERENCED BY %d\n", cli->uid);
    free(cli);
    cli_count--;
    pthread_detach(pthread_self());

    return NULL;
}

int start_server() {
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    pthread_t tid;

#ifdef PRINT_FLAG
    MD5_CTX ctx;
    unsigned char result[16];
    char shit[33] = {'\0'};

    const char *flag = "cApwN{d3us_d3x_my_4pk_is_augm3nted}";

    for (int i = 0; i < strlen(flag); i++) {
        MD5_Init(&ctx);
        MD5_Update(&ctx, flag + i, 1);
        MD5_Final(result, &ctx);

        for (int j = 0; j < 16; j++) {
            snprintf(shit + (j * 2), 3, "%02x", result[j]);
        }
        shit[32] = '\0';
        LOGI("%s", shit);
    }
#endif

    /* Socket settings */
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(1337);

    /* Bind */
    if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        LOGE("Socket binding failed: %s", strerror(errno));
        return 1;
    }

    /* Listen */
    if(listen(listenfd, 10) < 0){
        LOGE("Socket listening failed: %s", strerror(errno));
        return 1;
    }

    LOGI("<[SERVER STARTED]>\n");

    /* Accept clients */
    while(1){
        socklen_t clilen = sizeof(cli_addr);
        connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &clilen);

        /* Check if max clients is reached */
        if((cli_count+1) == MAX_CLIENTS){
            LOGI("<<MAX CLIENTS REACHED\n");
            LOGI("<<REJECT ");
            print_client_addr(cli_addr);
            LOGI("\n");
            close(connfd);
            continue;
        }

        /* Client settings */
        client_t *cli = (client_t *)malloc(sizeof(client_t));
        cli->addr = cli_addr;
        cli->connfd = connfd;
        cli->uid = uid++;
        sprintf(cli->name, "%d", cli->uid);

        /* Add client to the queue and fork thread */
        queue_add(cli);
        pthread_create(&tid, NULL, &handle_client, (void*)cli);

        /* Reduce CPU usage */
        sleep(1);
    }
    return 0;
}

int main() {
    start_server();
    return  0;
}
