
/* JMENO :<	 Michal
   PŘIJMENÍ: 	 Řezník
   LOGIN:	 xrezni28
   PROJEKT:      č.2
   NÁZEV:	 Iterační výpočty	
*/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<ctype.h>
#include<string.h>

#define I_o 1e-12
#define U_t 25.8563e-3

//půlení intervalu
double diode(double U_o, double r, double eps);

//chceck number of arguments
int check_args(int pocet);

//transfer string to double
double arg_to_double(char arr[]);

//check validity of inputs
int check_validity(double a, double b, double c);

//putting into the kirchhows law
double kirchhoffs_law(double U_o, double R, double U_p);

//single I_p
double single_ip(double U_p);

int main(int argc, char *argv[]){
	if(check_args(argc) < 1){
		fprintf(stderr,"wrong number of inputs arguments\n");
		return -1;
	}
	//put args to double
	double U_o = arg_to_double(argv[1]);
	double r = arg_to_double(argv[2]);
	double eps = arg_to_double(argv[3]);
	if(check_validity(U_o, r, eps) < 1){
		fprintf(stderr,"error: invalid arguments\n");
		return -1;
	}
	double U_p = diode(U_o, r, eps);		//calling law
	double I_p = single_ip(U_p);			//calling Ip part from law
	fprintf(stdout,"Up=%g V\nIp=%g A\n", U_p, I_p);  
	return 0;
}

//bisection of interval
double diode(double U_o, double r, double eps){
	double middle;
	double min = 0;
	double max = U_o;
	double last = 0;
	while((max - min) > eps){
		middle = (max + min)/2;
		if(last == middle) break;
		else if(kirchhoffs_law(U_o, r, middle) < 0){
			min = middle;
		}
		else{
			max = middle;
		}
	last = middle;
	}
	return middle;	
}

//chceck number of arguments
int check_args(int pocet){
	if(pocet != 4){
		return -1;
	}
	else return 1;
}

//transfer string to double
double arg_to_double(char arr[]){
	char *chyba = NULL;
	double dbl = strtod(arr, &chyba);
        if(*chyba != '\0') {
                 return -1;
        }
	return dbl;
}

//check validity of inputs
int check_validity(double a, double b, double c){
	if((a < 0 ) || (b < 0) || (c < 0 || c > 1)){
	 	return -1;
	}	
        else return 1;
}
	
//putting into the kirchhows law
double kirchhoffs_law(double U_o, double R, double U_p){
	return I_o * (exp(U_p / U_t) - 1) - ((U_o - U_p)/R);
}

//single I_p
double single_ip(double U_p){
	return I_o * (exp(U_p/ U_t) - 1);
}







