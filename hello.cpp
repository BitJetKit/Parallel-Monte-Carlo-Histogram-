/* Programming_Assignment2*/
// Monte Carlo method

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>

void Get_input(long long int tosses_p, int my_rank, int comm_sz, MPI_Comm comm);
int MPI_Recv(void *buf,
	     int count,
	     MPI_Datatype datatype,
	     int source,
	     int tag,
	     MPI_Comm comm,
	     MPI_Status *status);

int main(int argc, char* argv[]){

  int number_in_circle = 0;
  double x, y;
  double distance_squared;
  int reducedCount;// reducedTosses;
  time_t t;
  double pi_estimate;
  long long int toss; long long int number_of_tosses;
  int my_rank;
  int comm_sz;
  MPI_Comm comm;

  srand(time(&t));
  
  MPI_Init(NULL, NULL);
  comm = MPI_COMM_WORLD;

  MPI_Comm_size(comm, &comm_sz);
  MPI_Comm_rank(comm, &my_rank);
  Get_input(&number_of_tosses, my_rank, comm_sz, comm);
  
    if( my_rank != 0)
      {
	for(toss = 0; toss < number_of_tosses; toss++) {

	  x =(double)(rand() % 2) - 1;
	  y =(double)(rand() %2) - 1;
	  
	  distance_squared = x*x + y*y;
	  if (distance_squared <= 1)
	    {
	     number_in_circle++;
	    }
	  }
     }
		     
	MPI_Reduce(&number_in_circle, &reducedCount, 1 , MPI_INT, MPI_SUM, 0, comm);
	//	MPI_Reduce(&number_of_tosses, reducedTosses, number_of_tosses, MPI_INT, MPI_SUM, 0, comm);

	// reducedTosses -= number_of_tosses;

	pi_estimate = 4*reducedCount/((double) number_of_tosses);
	
	if (my_rank == 0)
	  {
	    printf("Pi: %f\n%lld\n%lld\n", pi_estimate, reducedCount, number_of_tosses);			   // Print the estimated value of pi
	  }
	
	MPI_Finalize();
	return 0;
}

void Get_input(long long int tosses_p, int my_rank,int comm_sz, MPI_Comm comm){
  if(my_rank == 0) {
    printf("Enter the number of tosses\n");
    scanf("%lld", &tosses_p);
    for (int i = 1; i < comm_sz; i++){
      MPI_Send(&tosses_p, 1, MPI_LONG_LONG_INT, 0, 0, comm);
    }
  } else {
      MPI_Send(&tosses_p, 1, MPI_LONG_LONG_INT, 0, 0, comm);
  }
  MPI_Bcast(&tosses_p, 1, MPI_LONG_LONG_INT, 0, comm);
} /* Get_input */


