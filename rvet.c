/**
 * Código base (incompleto) para implementação de relógios vetoriais.
 * Meta: implementar a interação entre três processos ilustrada na figura
 * da URL: 
 * 
 * https://people.cs.rutgers.edu/~pxk/417/notes/images/clocks-vector.png
 * 
 * Compilação: mpicc -o parte1 parte1.c
 * Execução:   mpiexec -n 3 ./parte1
 */
 
#include <stdio.h>
#include <string.h>  
#include <mpi.h>   
#include <stdlib.h>


typedef struct Clock { 
   int p[3];
} Clock;


void printClock(Clock *clock, int processo) {
   printf("Process: %d, Clock: (%d, %d, %d)\n", processo, clock->p[0], clock->p[1], clock->p[2]);
}

void Event(int pid, Clock *clock){
   clock->p[pid]++;   
}


void Send(int origem, int destino, Clock *clock){
   // TO DO
   clock->p[origem]++;  //atualiza o clock
   int * mensagem;
   mensagem = calloc (3, sizeof(int));
   
   for (int i = 0; i < 3; i++) {
         mensagem[i] = clock->p[i];
   }
   MPI_Send(mensagem, 3, MPI_INT, destino, origem, MPI_COMM_WORLD);
   
   free(mensagem);
}




void Receive(int origem, int destino, Clock *clock){
   // TO DO
   clock->p[destino]++;  //atualiza o clock
   int * mensagem;
   mensagem = calloc (3, sizeof(int));
   
   MPI_Recv(mensagem, 3,  MPI_INT, origem, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   for (int i = 0; i < 3; i++) { 
      if ( mensagem[i] > clock->p[i]) {
         clock->p[i] = mensagem[i];
      }
   }
   
   free(mensagem);
}



// Representa o processo de rank 0
void process0(){
   Clock clock = {{0,0,0}};
   
   Event(0, &clock);
   printClock(&clock, 0);
   

   Send(0, 1, &clock);
   printClock(&clock, 0);

   Receive(1, 0, &clock);
   printClock(&clock, 0);
   

   Send(0, 2, &clock);
   printClock(&clock, 0);
   

   Receive(2, 0,  &clock);
   printClock(&clock, 0);
   

   Send(0, 1, &clock);
   printClock(&clock, 0);
   
   Event(0, &clock);
   printClock(&clock, 0);
   
}

// Representa o processo de rank 1
void process1(){
   Clock clock = {{0,0,0}};


   Send(1, 0, &clock);
   printClock(&clock, 1);


   Receive(0, 1, &clock);
   printClock(&clock, 1);
   

   Receive(0, 1, &clock);
   printClock(&clock, 1);
   
   

}

// Representa o processo de rank 2
void process2(){
   Clock clock = {{0,0,0}};

   Event(2, &clock);
   printClock(&clock, 2);
   

   Send(2, 0, &clock);
   printClock(&clock, 2);
   

   Receive(0, 2, &clock);
   printClock(&clock, 2);   
}

int main(void) {
   int my_rank;               

   MPI_Init(NULL, NULL); 
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   if (my_rank == 0) { 
      process0();
   } else if (my_rank == 1) {  
      process1();
   } else if (my_rank == 2) {  
      process2();
   }

   /* Finaliza MPI */
   MPI_Finalize(); 

   return 0;
}  /* main */
