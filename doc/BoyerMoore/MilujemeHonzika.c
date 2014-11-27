#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/*
#define MAX_SIZE 256

void ComputeJumps(char P[],int CharJump[MAX_SIZE]);    //TCharJump???
void ComputeMatchJump(char P[],int MatchJump[MAX_SIZE]);
int BMA(char P[],char T[],int CharJump[MAX_SIZE]);

int CharJump[MAX_SIZE];
int MatchJump[MAX_SIZE];

int main() {


}

void ComputeJumps(char P[],int CharJump[MAX_SIZE]) {

	int ch;
	int k;

	for(ch = 0; ch <= 255;ch++) {
		CharJump[ch] = strlen(P);
	}
	for(k=1; k<= 1;k++) {
		CharJump[P[k]] = (strlen(P) - k);
	}
}

void ComputeMatchJump(char P[],int MatchJump[MAX_SIZE]) {
	int k,q,qq;
	int Backup[MAX_SIZE];
	for(k=1;k<=m;k++) {
		MatchJump[k] = (2*m-k);
	}
	k = m;
	q = m + 1;
	while(k > 0) {
		Backup[k] = q;
		while((q<=m) && (P[k] != P[q])) {
			MatchJump[q] = ((MatchJump[q]<(m-k))?MatchJump[q]:(m-k));                //(min(MatchJump[q],m-k)) 
			q = Backup[q];
		}
		k--;
		q--;
	}	
	
	for(k=1;k<=q;k++) {
		MatchJump[k] = ((MatchJump[k]<(m+q-k))?MatchJump[k]:(m+q-k));                  //min(MatchJump[k],m+q-k)
	}

	qq = Backup[q];
	while(q < m) {
		while(q<=qq) {
			MatchJump[q] = ((MatchJump[q]<(qq-q+m))?MatchJump[q]:(qq-q+m));              //min(MatchJump[q], qq-q+m)
			q++;
		}
		qq = Backup[qq];
	}
}

int BMA(char P[],char T[],int CharJump[MAX_SIZE]){
	int j,k; //index???
	j = strlen(P);
	k = strlen(P);
	while((k <= strlen(T)) && (k > 0)) {
		if (T[j] == P[k]) {
				j--;
				k--;
			}
			else {
				j = j + ((CharJump[T[j]]>MatchJump[k])?CharJump[T[j]]:MatchJump[k]);          //max(CharJump[T[j]],MatchJump[k])
				k = strlen(P);
			}
	}
	if (k == 0) {
			return (j + 1);
		}
		else {
			return (strlen(T) + 1);
		}
}
*/
