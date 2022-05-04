#include"knihovna.h"


int *shared = NULL; //Number of printed action
sem_t *begin = NULL; //start in the same time
sem_t *cert = NULL; //For leting continue certificated imigrants
sem_t *leave = NULL; //For leting go imigratns who wait till judge leaves (want away) 
sem_t *start = NULL; //For leting go imigrants who wait till judge leaves (want inside)
FILE *out;
int *judgeinside = NULL; //1 - if judge is inside 0 - if judge is away
int *confirm = NULL; //number of imigrants who waits for confirmation
int *NB = NULL; //imigrants in building
int *NE = NULL; //imigrants in building, who have not been decided
int *NC = NULL; //imigrants in building, which already check
int *next = NULL;
int *count = NULL;
int *first = NULL;

#define MMAP(pointer) {(pointer) = mmap(NULL,sizeof(*(pointer)),PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);}
#define UNMAP(pointer) {munmap((pointer),sizeof((pointer)));}

////////////FUNCTION DECLARATIONS////////////
//random time to sleep (miliseconds)
void waiting(int time);

//Try to make proj2 output file
int output();

//Before end of program clean the mess
void clean();

//Arguments to int
int argtoint(char arg[]);

//Check range of args
int checkargs(int P, int I, int J, int T, int S);

//First action
void bornimigrant(int number, int time);


//Judge process
void judge(int people ,int starttime, int confirmtime);

//generator
void generator(int people, int timefornew, int timeconf);

////////////////////MAIN/////////////////////
int main(int argc, char *argv[])
{
	if(output() == -1){
		clean();
		fprintf(stderr,"memory fail");
		return -1;
	}
	if(argc != 6){
		fprintf(stderr,"wrong number of arguments\n");
		return -1;
	}
	int PI=argtoint(argv[1]);//Počet přistěhovalců
	int IG=argtoint(argv[2]);//Doba do které přijde nový imigrant
	int JG=argtoint(argv[3]);//Doba do které se soudce vrátí zpět do budovy
	int IT=argtoint(argv[4]);//Simuluje vyzvedávání certifikátu
	int JT=argtoint(argv[5]);//Trvání rozhodnutí soudcem

	if(checkargs(PI, IG, JG, IT, JT) < 0){
		fprintf(stderr, "One of your inputs is out of the range or it's wrong type \n"); 
		return -1;
	}
	pid_t first = fork();
	if (first == 0){
		judge(PI,JG,JT);
	}
        else{
		int erro;
		pid_t gen = fork();
     		if(gen == 0) generator(PI,IG,IT);
		waitpid(gen, &erro, 0);
	}
	int err;
	waitpid(first, &err, 0);
	clean();
	exit(0);
	return 0;
}
/////////////////////FUNCTION FULL//////////////////////////
void waiting(int time)
{	
	if(time > 0){
		usleep(time*1000);
	}
}

int output()
{
	out=fopen("proj2.out", "w");
	setbuf(out,NULL);
	MMAP(first); MMAP(next); MMAP(shared); MMAP(judgeinside); MMAP(NB); MMAP(NE); MMAP(NC); MMAP(confirm); MMAP(count);
	if((cert = sem_open("/xrezni28.semafor1", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) return -1;
	if((leave = sem_open("/xrezni28.semafor2", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) return -1;
	if((start = sem_open("/xrezni28.semafor3", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) return -1;
	if((begin = sem_open("/xrezni28.semafor4", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) return -1;
        return 0;
}
void clean()
{
	sem_close(start);
	sem_close(cert);
	sem_close(leave);
	sem_close(begin);
	UNMAP(first); UNMAP(next); UNMAP(shared); UNMAP(NB); UNMAP(NE); UNMAP(NC); UNMAP(confirm); UNMAP(judgeinside); UNMAP(count);
	sem_unlink("/xrezni28.semafor1");
	sem_unlink("/xrezni28.semafor2");
	sem_unlink("/xrezni28.semafor3");
	sem_unlink("/xrezni28.semafor4");
	if(out != NULL) fclose(out);
}
int argtoint (char arg[])
{
	char *end;
	int cislo = strtol(arg,&end,10);
	if(*end != '\0'){
		return -1;
	}
	else return cislo;
}
int checkargs(int P, int I, int J, int T, int S)
{
	if(P >= 1){
		if(I >= 0 && I <= 2000){
			if(J >= 0 && J <= 2000){
				if(T >= 0 && T <= 2000){
					if(S >= 0 && S <= 2000){
						return 1;
					}
				}
	 		}
		}
	}
	return -1;
}

void bornimigrant(int number, int time)
{
	sem_post(begin);
	fprintf(out,"%d: %dIMM:starts\n", *shared = *shared + 1,  number);
	//waits until judge leaves if he is inside
	if(*judgeinside == 1){
		*first = *first + 1;
		sem_wait(start);
	}
	sem_wait(start);
	sem_post(start);
	
	fprintf(out,"%d: %dIMM:enter 	  :%d	  :%d	  :%d\n", *shared = *shared + 1,  number,*NE = *NE + 1, *NC,  *NB = *NB + 1);
	fprintf(out,"%d: %dIMM:check 	  :%d	  :%d	  :%d\n", *shared = *shared + 1,  number, *NE,*NC = *NC + 1, *NB);
	//waits until judge give result
	*confirm = *confirm + 1;
	if(*next == 0){
		*next = *next + 1;
		sem_wait(cert);
	}
	sem_wait(cert);

	fprintf(out, "%d: %dIMM:wants certificate           :%d     :%d     :%d\n", *shared = *shared + 1,  number, *NE, *NC, *NB);
	waiting(rand()%(time+1));
	fprintf(out,"%d: %dIMM:got certificate           :%d     :%d     :%d\n", *shared = *shared + 1,  number,*NE, *NC, *NB);
	//waits until judge leaves
	if(*judgeinside == 1){
		sem_wait(leave);
	}
	sem_wait(leave);
	//odešel
	fprintf(out,"%d: %dIMM:leaves           :%d     :%d     :%d\n", *shared = *shared + 1,  number,*NE, *NC, *NB = *NB -1);
	exit(0);
}
void judge(int people, int starttime, int confirmtime)
{
	sem_wait(begin);
	sem_wait(begin);
	while(*count < people){
		waiting(rand()%(starttime+1));
		*judgeinside = 1;
		fprintf(out, "%d: JUDGE:Wants to enter  :%d	  :%d	  :%d\n", *shared = *shared + 1,*NE, *NC, *NB);
		fprintf(out, "%d: JUDGE:enters  	  :%d	  :%d	  :%d\n", *shared = *shared + 1,*NE, *NC, *NB);
		//waits for check
		if(*NE != *NC){	
			fprintf(out, "%d : JUDGE:waits for imm  :%d     :%d     :%d\n", *shared = *shared + 1,*NE, *NC, *NB);
			while(*NE != *NC){
				usleep(10);
			}
		}
		fprintf(out, "%d: JUDGE:starts confirmation          :%d     :%d     :%d\n", *shared = *shared + 1,*NE, *NC, *NB);
		waiting(rand()%(confirmtime+1)); //wait JT
		fprintf(out,"%d: JUDGE:ends confirmation          :%d     :%d     :%d\n", *shared = *shared + 1,*NE = 0, *NC = 0, *NB);
		//let continue imm with confirmation
		*count = *count + *confirm;
		for(int g = 0; g < *confirm; g++){
			sem_post(cert);
		}
		*confirm = 0;
		fprintf(out, "%d\n", *count);
		   //for another judges round
		fprintf(out, "%d: JUDGE:leaves 	  :%d	  :%d	  :%d\n", *shared = *shared + 1, *NE, *NC, *NB);
		*judgeinside = 0;
		for(int s =0; s < *first; s++) sem_post(start);//let go inside imm waiting outside
		for(int p = 0; p <= *count+1; p++) sem_post(leave); //let go out imm waiting inside
	}
		fprintf(out, "%d: JUDGE:finishes\n", *shared = *shared + 1);
		exit(0);
}
void generator(int people,int timefornew, int timeconf)
{
	for(int i=1;i<=people;i++)
	{
 		pid_t GUY= fork();
             	if(GUY == 0){
			bornimigrant(i, timeconf);
		}
		else{
			waiting(rand()%(timefornew+1));
			if(i == people){
				 for(int o= 1; o<=people; o++) wait(NULL);
			}
		}
	}
	exit(0);
}

