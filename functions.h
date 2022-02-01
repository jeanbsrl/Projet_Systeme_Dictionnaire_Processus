/**
 * authors:
 * FRANCES Tom
 * BOU SERHAL Jean
 * 
 */


#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void close_unused_pipes(int** pipes, int node, int N);
void write_pipe(int pipe[], Request* request);
void read_pipe(int pipe[], Request* request);
void interprete_response(Request* request);
void node(Request* request, int reading_pipe[], int writing_pipe[], int controller_pipe[], int N, int node_indice);
void controller(Request* request, int** pipes, int N);
void wait_childs_ready(Request* request, int** pipes, int N);

#endif