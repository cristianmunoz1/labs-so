#include <stdio.h>
#include "request.h"
#include "io_helper.h"
#include <pthread.h>
#include "buffer.h"

char default_root[] = ".";
//
// ./wserver [-d <basedir>] [-p <portnum>] 
//
void* consumer_function(void* arg){
    while(1){
        int conn_fd = buffer_remove(&request_buffer);
        request_handle(conn_fd);
        close_or_die(conn_fd);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int c;
    char *root_dir = default_root;
    int port = 10000;
    int threads = 1; // 1 thread by default
    int buffers = 1; //1 request at a time by default
    char* schedalg = "FIFO";
    
    while ((c = getopt(argc, argv, "d:p:t:b:s:")) != -1)
	switch (c) {
	case 'd':
	    root_dir = optarg;
	    break;
	case 'p':
	    port = atoi(optarg);
	    break;
    case 't':
        threads = atoi(optarg);
        break;
    case 'b':
      buffers = atoi(optarg);
      break;
    default:
	    fprintf(stderr, "usage: wserver [-d basedir] [-p port] [-t number of threads] [-b buffers] [-s FIFO or SFF]\n");
        exit(1);
	}
    
    // run out of this directory
    chdir_or_die(root_dir);

    // now, get to work
    int listen_fd = open_listen_fd_or_die(port);

    //initialize the buffer with #buffer spaces
    buffer_init(&request_buffer, buffers);

    // Create the threads pool
    pthread_t workers[threads];

    for(int i = 0; i < threads; i++){
        int* p_listen_fd = malloc(sizeof(int));
        *p_listen_fd = listen_fd;
        pthread_create(&workers[i], NULL, consumer_function, NULL);
    }
    
    while (1) {
	    struct sockaddr_in client_addr;
	    int client_len = sizeof(client_addr);
	    int conn_fd = accept_or_die(listen_fd, (sockaddr_t *) &client_addr, (socklen_t *) &client_len);

        buffer_insert(&request_buffer, conn_fd);
    }

    buffer_destroy(&request_buffer);

    return 0;
}


    


 
