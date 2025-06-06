#include <stdio.h>
#include "request.h"
#include "io_helper.h"

char default_root[] = ".";

//
// ./wserver [-d <basedir>] [-p <portnum>] 
// 
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
    case 's':
      if(strcmp("FIFO", optarg)==0){
        schedalg = "FIFO"; 
    } else if (strcmp("SFF", optarg)==0){
        schedalg = "SFF";
    }else{
        fprintf(stderr, "usage: wserver [-d basedir] [-p port] [-t number of threads] [-b buffers] [-s FIFO or SFF]\n");
	    exit(1);
    }
      break;
	default:
	    fprintf(stderr, "usage: wserver [-d basedir] [-p port] [-t number of threads] [-b buffers] [-s FIFO or SFF]\n");
        exit(1);
	}

    // run out of this directory
    chdir_or_die(root_dir);

    // now, get to work
    int listen_fd = open_listen_fd_or_die(port);
    while (1) {
	struct sockaddr_in client_addr;
	int client_len = sizeof(client_addr);
	int conn_fd = accept_or_die(listen_fd, (sockaddr_t *) &client_addr, (socklen_t *) &client_len);
	request_handle(conn_fd);
	close_or_die(conn_fd);
    }
    return 0;
}


    


 
