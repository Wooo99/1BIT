/* 
Jmeno:	Michal Řezník
Login:	xrezni28
Název: Projekt 1 - Práce s textem
*/


#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

#define	LINE 101

	//kontrola množství argumentů
int check_arg(int argc){
	if(argc > 2) return -1;
	else return 0; 
}

	//kontrola poslopnosti číslic
int check_number_array(int len, char arr[]) {
	for(int i = 0; i <=(len -1); i++){
		if((isdigit(arr[i])) == 0) return -1;
		else continue;
	}
	return 0;
}

int main(int argc, char *argv[]){
	char lichy [LINE];
        char radek [LINE];    
	char clich [LINE];
	int n = 0;
	//nezadaný argument
	if(argc < 2){
		while(fgets(radek,sizeof(radek),stdin) != NULL){
                	n++;
                        if(n % 2 != 0){
				
                        	strcpy(lichy, radek);
                                lichy[strcspn(lichy, "\n")] = 0;
				printf("%c\n", lichy[1]);
                                continue;
                         }
                         if(n % 2 == 0){
                                radek[strcspn(radek, "\n")] = 0;
                                fprintf(stdout ,"%s, %s\n", lichy, radek);
                         }
                }
        return 0;
       }
	char *dif;  //pro tel.číslo
	char *diff; //pro porovnání mobilní klávesnice
	if(check_arg(argc) == -1){
		fprintf(stderr,"Many argument\n");
		return -1;
	}
	if(check_number_array(strlen(argv[1]), argv[1]) != 0){
		fprintf(stderr,"wrong argument");
		return -1;
	}	
	//vyhledávání
	int d = 1;
	while(fgets(radek, sizeof(radek), stdin) != 0){
		n++; 
		if(n % 2 != 0){
			strcpy(lichy, radek);
			lichy[strcspn(lichy, "\n")] = 0; //odečíst poslední znaky
			continue;		
		}
		if(n % 2 == 0){		
			radek[strcspn(radek, "\r\n")] = 0;
			for(int z = 0; z <= strlen(lichy); z++){
				if((lichy[z] >= 'a' && lichy[z] <= 'c')  || (lichy[z] >= 'A' && lichy[z] <= 'C')) clich[z] = '2';
				else if((lichy[z] >= 'd' && lichy[z] <= 'f')  || (lichy[z] >= 'D' && lichy[z] <= 'F')) clich[z] = '3';
				else if((lichy[z] >= 'g' && lichy[z] <= 'i')  || (lichy[z] >= 'G' && lichy[z] <= 'I')) clich[z] = '4';
 				else if((lichy[z] >= 'j' && lichy[z] <= 'l')  || (lichy[z] >= 'J' && lichy[z] <= 'L')) clich[z] = '5';
 				else if((lichy[z] >= 'm' && lichy[z] <= 'o')  || (lichy[z] >= 'M' && lichy[z] <= 'O')) clich[z] = '6';
 				else if((lichy[z] >= 'p' && lichy[z] <= 's')  || (lichy[z] >= 'P' && lichy[z] <= 'S')) clich[z] = '7';
 				else if((lichy[z] >= 't' && lichy[z] <= 'v')  || (lichy[z] >= 'T' && lichy[z] <= 'V')) clich[z] = '8';
 				else if((lichy[z] >= 'w' && lichy[z] <= 'z')  || (lichy[z] >= 'W' && lichy[z] <= 'Z')) clich[z] = '9';
 				else if(lichy[z] == '+') clich[z] = 0;
				else clich[z] = lichy[z];
			}
			dif = strstr(radek, argv[1]); //hledání v čísle
			diff = strstr(clich, argv[1]);// hledání v klávesnici(strstr vrací !NULL hodnotu při nálezu)
			if ((dif != NULL) || (diff != NULL)){
 				d++;
				fprintf(stdout ,"%s, %s\n", lichy, radek);
			}
		}
	}
	//nenalezeno
	if(d == 1) fprintf(stdout, "Not found\n");
	return 0;
}
