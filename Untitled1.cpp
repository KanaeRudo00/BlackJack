#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include <unistd.h>
#include <conio.h>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
void setupDeck(int number);
void setupAccount();
void getCardAll(int sizehand, int number);
void showCard(int hand[2][13], int order);
void showHand(int hand[2][13], int sizeh);
void getCard( int hand[2][13], int *sizeh);
void blackjack(int number, int bot);
void playBlackjack(int hand[2][13], int *sizeh);
void checkWinBj(int number, int bot, int dealer);
int  sumCard(int hand[2][13], int sizeh);
void swap(int*a, int*b);
void swapHand(int order1, int order2);
void botBj(int botHand[2][13], int *sizeBoth);
void betsystem();
int getBot();
void menu();
void updatePName();
void setupGame();

int deck[4][13];
int hand[8][2][13];
int sizeh[8];
int account[8];
int botHand[8][2][13];
int sizeBoth[8];
int botManage[8]= {0,0,0,0,0,0,0,0};
char pName[8][15];
int number=0;
int count = 0;
int bot = 0;
int oldcount;
int setup=0;
int update=0;

int main() {
	srand(time(NULL));	
	setupAccount();
	setupDeck(number);
	while(1==1){

		menu();
		if(count!=oldcount){
			while(1==1){
				char c;
				fflush(stdin);
				printf("\nDo you want to continue? (Y/N)\n");
				if((c=toupper(getchar()))=='N') return 0;
				else if(c=='Y') break;
				printf("\nInvalid value!");
				fflush(stdin);
			}
		}
	}
}

void setupAccount(){
	for(int i = 0; i<=7;i++) account[i]=2000;
}

void setupDeck(int number){
	for(int i = 0; i<=3;i++) for(int j = 0; j<=12;j++) deck[i][j]=1;
	for(int i = 0; i<=7;i++) sizeh[i]=0;
	for(int i = 0; i<=7;i++) sizeBoth[i]=0;
}

void getCardAll(int sizehand, int number) {
	for(int i = 0; i<= sizehand-1;i++) for(int j = 0; j<=number-1; j++) getCard(hand[j],&sizeh[j]);	
}

void showCard(int hand[2][13], int order){
	switch(hand[0][order]){
			case 0:
				printf(" A");
				break;
			case 10:
				printf(" J");
				break;
			case 11:
				printf(" Q");
				break;
			case 12:
				printf(" K");
				break;
			default:
				printf("%2d",hand[0][order]+1);
		}
		switch(hand[1][order]){
			case 0:
				printf("\3");
				break;
			case 1:
				printf("\4");
				break;
			case 2:
				printf("\5");
				break;
			case 3:
				printf("\6");
				break;
		}
		printf("\t");
}

void showHand(int hand[2][13], int sizeh){
	for(int i = 0; i<=sizeh-1;i++) showCard(hand, i);
}

void getCard(int hand[2][13], int *sizeh){
	int newCardValue;
	int newCardRank;		
	do{
		newCardValue = rand()%13;
		newCardRank	= rand()%4;
	}while(deck[newCardRank][newCardValue]==0);
	hand[0][*sizeh]=newCardValue;
	hand[1][*sizeh]=newCardRank;
	(*sizeh)++;
	deck[newCardRank][newCardValue]=0;
}

void blackjack(int number, int bot){	
	int dealer;
	getCardAll(2,number);
	
	for(int i = 0; i<=bot-1; i++){
		int oBot = getBot();
		getCard(botHand[oBot],&sizeBoth[oBot]);
		getCard(botHand[oBot],&sizeBoth[oBot]);	
	}
	
	//Choose dealer
	while(1==1){
		printf("\nWho do you want to be a dealer ?\n");
		for(int i = 0; i<=number-1;i++) printf("\n%d. %s",i+1,pName[i]);
		for(int i = 0; i<=bot-1;i++) printf("\n%d. Bot%d",i+number+1,i+1);
		printf("\n");
		scanf("%d",&dealer);
		if(getchar()==10 && dealer <=number+bot) break;
		printf("\nInvalid value. This value should be an integer between 1 and %d",number+bot);
	}
	int turn = dealer;
	//Built turn-base structure
BEGIN:
	system("cls");
	printf("\n\3\4\5\6\n");
	int play = turn%(number+bot)+1;
	if(dealer <= number ){
		printf("\n%s's (Dealer) hand\n",pName[dealer-1]);
		if (play!=dealer) showCard(botHand[dealer-1],0);
		else showHand(hand[dealer-1],sizeh[dealer-1]);	
	
	}else{
		printf("\nBot%d (Dealer) hand:\n",dealer-number);
		if (play!=dealer) showCard(hand[dealer-number-1],0);
		else showHand(botHand[dealer-number-1],sizeBoth[dealer-number-1]);	
	}

	for(int i = 0; i<=number-1; i++){
		if(dealer-1!= i){
			printf("\n%s's hand:\n",pName[i]);
			showHand(hand[i],sizeh[i]);	
		}
	}
	for(int i = 0; i<=bot-1;i++){
		if(dealer-number-1!=i){
			printf("\nBot%d's hand:\n",i+1);
			showHand(botHand[i],sizeBoth[i]);
		}
	}
	printf("\n=========================================\n");
	if(play<=number){
		printf("\n%s's turn:\n",pName[play-1]);	
		while(1==1){
			char c;
			fflush(stdin);
			printf("\nDo you want to end your turn ? (Y/N)\n");
			if((c=toupper(getchar()))=='Y'){
				if(play==dealer) {
					checkWinBj(number,bot,dealer);
					printf("\nThank you for playing !! - MyLL");	
					return;
				}
				turn++;
				goto BEGIN;
			}
			else if(c=='N'){
				playBlackjack(hand[play-1],&sizeh[play-1]);
				break;
			}
			printf("\nInvalid value!");
			fflush(stdin);
		}
	}
	else{
		if(play==dealer){
			checkWinBj(number, bot, dealer);
			return;
		}
		printf("\nBot%d's turn",play-number);
		botBj(botHand[play-number-1],&sizeBoth[play-number-1]);
		turn++;
	}
	goto BEGIN;
}

void playBlackjack(int hand[2][13], int *sizeh){
	char c;
	while(1==1){
		fflush(stdin);
		printf("Do you want to draw a card? (Y/N)\n");
		if((c=toupper(getchar()))=='Y'){
			getCard(hand,sizeh);
			return;	
		}
		else return;
		fflush(stdin);
		printf("\nInvalid Value!");
	}
}

void checkWinBj(int number, int bot, int dealer){
	int sum[number+bot];
	int player[number+bot];
	int winList[number+bot];
	
	//setup

	for(int i = 0; i<=number-1; i++){
		sum[i]= sumCard(hand[i],sizeh[i]);
		player[i]=i+1;
		winList[i]=0;	
	}
	for(int i = 0; i<=bot-1;i++){
		sum[number+i]=sumCard(botHand[i],sizeBoth[i]);
		player[number+i]=number+i+1;
		winList[number+i]=0;
	}

	//Phan Loai
	for(int i =0; i<=number+bot-1; i++){
		if(i<number){
			if(sizeh[i]==2 && (hand[i][0][0]==0 || hand[i][0][1]==0) && sum[i]==21) winList[i]=10;
			else if(sum[i]<=21) winList[i]=1;
		}
		else{
			if(sizeBoth[i]==2 && (botHand[i][0][0]==0 || botHand[i][0][1]==0) && sum[i]==21) winList[i]=10;
			else if(sum[i]<=21) winList[i]=1;
		}
	}

	//Sap xep phan loai
	for(int i = 0; i<=number+bot-2; i++){
		for(int j = i+1; j<=number+bot-1; j++){
			if(winList[i]<winList[j]){
				swap(&winList[i],&winList[j]);
				swap(&player[i],&player[j]);
				swap(&sum[i],&sum[j]);
			}
		}
	}

	
	//Sap xep dong loai
	for(int i = 0; i<=number+bot-2; i++){
		for(int j=i+1;j<=number+bot-1;j++){
			if(sum[i]<sum[j]&&winList[i]!=10){
				swap(&sum[i],&sum[j]);
				swap(&player[i],&player[j]);
				swap(&winList[i],&winList[j]);
			}
		}
	}

	//Diem thuong, Diem phat
	for(int i = number+bot-1;i>=0;i--){
		if(winList[i]!=10){
			if(winList[number+bot-1-i]==0)winList[number+bot-1-i]-=i;	
			if(winList[number+bot-1-i]==1)winList[number+bot-1-i]+=i;	
		}
	}

	
	//Sap xep phan loai
	for(int i = 0; i<=number+bot-2; i++){
		for(int j = i+1; j<=number+bot-1; j++){
			if(winList[i]<winList[j]){
				swap(&winList[i],&winList[j]);
				swap(&player[i],&player[j]);
				swap(&sum[i],&sum[j]);
			}
		}
	}

	//Xet truong hop cung gia tri
	for(int i = 0; i<=number+bot-2;i++) if(sum[i]==sum[i+1]) winList[i+1]=winList[i];

	//Tim vi tri cua dealer
	int pos1;
	for(int i = 0; i<=number+bot-1;i++) if(player[i]==dealer){
		pos1 = i;
		break;
	}

	int count = 0;
	for(int i = 0; i<=number+bot-1;i++){
		if(i!=pos1){
			if(winList[i]>winList[pos1]){
				count--;
				if(player[i]<=number){
					printf("\n%s + 500$",pName[player[i]-1]);	
					account[player[i]-1]+=500;
				}
			}
			if(winList[i]<winList[pos1]){
				count++;
				if(player[i]<=number){
					printf("\n%s- 500$",pName[player[i]-1]);
					account[player[i]-1]-=500;	
				}
			}
			if(winList[i]==winList[pos1] && player[i]<=number) printf("\n%s + 0$",pName[player[i]-1]);	
		}
	}
	if(dealer<=number){
		if(count>=0) printf("\n%s (Dealer) + %d$",pName[dealer-1],count*500);
		else printf("\n%s (Dealer) - %d$",pName[dealer-1],-count*500);
		account[dealer-1]+=count*500;		
	}
	for(int i = 0; i<= number-1; i++) printf("\n%s's balance:	%d",pName[i], account[i]);
}

int sumCard(int hand[2][13], int sizeh){
	int sum=0;
	int flag = 0;
	for(int i = 0; i<=sizeh-1;i++){
		if(hand[0][i]>=9) sum+=9;
		else if(hand[0][i]==0) flag++;
		else sum+=hand[0][i];
	}
	for(int i = 1; i<=flag;i++){
		sum+=10;
		if(sum+sizeh>21) sum-=1;
		if(sum+sizeh>21) sum-=9;
	}
	return sum+sizeh;
}

void swap(int* a, int* b){
	int t = *a;
	*a = *b;
	*b = t;
}

void swapHand(int order1, int order2){
	int maxsizeh = sizeh[order1] >= sizeh[order2] ? sizeh[order1]:sizeh[order2];
	for(int i = 0; i<=maxsizeh-1;i++) for(int j = 0; j<=1;j++) swap(&hand[order1][j][i],&hand[order2][j][i]);	
	swap(&sizeh[order1],&sizeh[order2]);
}

void botBj(int botHandB[2][13], int *sizeBothB){

	printf("\nDo you want to end your turn? (Y/N)\n");
	sleep(1.25);
	while(1==1){
		if(sumCard(botHandB,*sizeBothB)>=14){
			printf("Y");
			sleep(1.25);
			return;	
		}
		else{
			printf("N");
			sleep(1.25);
			printf("\nDo you want to draw a card? (Y/N)\n");
			sleep(1.25);	
			printf("Y");
			sleep(1.25);
			getCard(botHandB,sizeBothB);
			return;
		}	
	}
}

int getBot(){
	for(int i = 0; i<=8; i++) if(botManage[i]==0){
		botManage[i]=1;
		return i;
	}
}

void menu(){
	system("cls");
	printf("\n\3\4\5\6\n");
	printf("\nWelcome to blackjack program created by MyLL");
	printf("\n\nThis is our menu\n\n");
	printf("\n1. Play Blackjack.");
	printf("\n2. Setup game.");
	printf("\n3. Update Player's Information.");
	printf("\n");
	char choice;
	scanf("%c",&choice);
	fflush(stdin);
	switch (choice){
		case '1':
			if(setup==0) setupGame();
			if(update==0)updatePName();
			blackjack(number,bot);
			oldcount=count;
			count++;
			break;
		case '2':
			setupGame();
			break;
		case '3':
			if (setup==0) setupGame(); 
			updatePName();
			break;
		default:
			printf("\nInvalid Value! ");
	}
	return;
}

void setupGame(){
	while(1==1){
		printf("\nHow many players want to play Blackjack?\n");
		scanf("%d",&number);
		if(getchar()==10&&number>=0&&number<9) break;
		printf("\nInvalid value! The value should be an integer number between 0 and 8\n");
	}
	while(number+bot<8){
		char c;
		printf("\nDo you want to add bot(s) into this lobby ? (Y/N)\n");
		if((c=toupper(getchar()))=='Y'){
			while(1==1){
				printf("\nHow many bots do you want to add?\n");
				scanf("%d",&bot);
				if(getchar()!=10 || bot+number>8)	printf("\nInvalid value. This field requires an integer, and there can only 8 hands in a lobby.");
				else{
					setup=1;
					return;	
				}
			}
		}
		else if(c=='N'){
			setup=1;
			break;
		}
		else printf("\nInvalid value.");
	}
}

void updatePName(){
	system("cls");
	if(update==1){
		int choice;
		while(1==1){
			printf("Which player do you want to update information?\n\n");
			for(int i = 0; i<=number-1; i++) printf("\n%d. %s",i, pName[i-1]);
			scanf("%d",&choice);
			if(getchar()==10) break;
			else printf("\nInvalid value! ");
		}
		printf("\nNew name of %s is :	",pName[choice-1]);
		gets(pName[choice-1]);
		printf("New name updated!");	
	}
	else{
		if(setup==0)setupGame();
		for(int i = 0; i<=number-1;i++){
			fflush(stdin);
			printf("\nPlease input player %d's name:	",i+1);
			gets(pName[i]);
			
		}
	}
	update=1;
}










//#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
//#include <string.h>
//#include <unistd.h>
//
///* run this program using the console pauser or add your own getch, system("pause") or input loop */
////1
//void setupDeck(int number);
////1
//void setupAccount();
////1
//void getCardAll(int sizehand, int number);
////1
//void showCard(int hand[2][13], int order);
////1
//void showHand(int hand[2][13], int sizeh);
////1
//void getCard( int hand[2][13], int *sizeh);
////1
//void blackjack(int number, int bot);
////1
//void playBlackjack(int hand[2][13], int *sizeh);
////1
//void checkWinBj(int number, int bot, int dealer);
////1
//int sumCard(int hand[2][13], int sizeh);
////1
//void swap(int*a, int*b);
////1
//void swapHand(int order1, int order2);
////1
//void botBj(int botHand[2][13], int *sizeBoth);
////1
//void betsystem();
////1
//int getBot();
////1
//void menu();
////1
//void updatePName();
////1
//void setupGame();
//
//int deck[4][13];
//int hand[8][2][13];
//int sizeh[8];
//int account[8];
//int botHand[8][2][13];
//int sizeBoth[8];
//int botManage[8]= {0,0,0,0,0,0,0,0};
//char pName[8][15];
//int number=0;
//int count = 0;
//int bot = 0;
//int oldcount;
//int setup=0;
//int update=0;
////1		
//int main() {
//	srand(time(NULL));	
//	setupAccount();
//	setupDeck(number);
//	while(1==1){
//
//		menu();
//		if(count!=oldcount){
//			while(1==1){
//				char c;
//				fflush(stdin);
//				printf("\nDo you want to continue? (Y/N)\n");
//				if((c=toupper(getchar()))=='N') return 0;
//				else if(c=='Y') break;
//				printf("\nInvalid value!");
//				fflush(stdin);
//			}
//		}
//	}
//}
////1
//void setupAccount(){
//	for(int i = 0; i<=7;i++) account[i]=2000;
//}
////1
//void setupDeck(int number){
//	for(int i = 0; i<=3;i++) for(int j = 0; j<=12;j++) deck[i][j]=1;
//	for(int i = 0; i<=7;i++) sizeh[i]=0;
//	for(int i = 0; i<=7;i++) sizeBoth[i]=0;
//}
////1
//void getCardAll(int sizehand, int number) {
//	for(int i = 0; i<= sizehand-1;i++) for(int j = 0; j<=number-1; j++) getCard(hand[j],&sizeh[j]);	
//}
////1
//void showCard(int hand[2][13], int order){
//	switch(hand[0][order]){
//			case 0:
//				printf(" A");
//				break;
//			case 10:
//				printf(" J");
//				break;
//			case 11:
//				printf(" Q");
//				break;
//			case 12:
//				printf(" K");
//				break;
//			default:
//				printf("%2d",hand[0][order]+1);
//		}
//		switch(hand[1][order]){
//			case 0:
//				printf("\3");
//				break;
//			case 1:
//				printf("\4");
//				break;
//			case 2:
//				printf("\5");
//				break;
//			case 3:
//				printf("\6");
//				break;
//		}
//		printf("\t");
//}
////1
//void showHand(int hand[2][13], int sizeh){
//	for(int i = 0; i<=sizeh-1;i++) showCard(hand, i);
//}
////1
//void getCard(int hand[2][13], int *sizeh){
//	int newCardValue;
//	int newCardRank;		
//	do{
//		newCardValue = rand()%13;
//		newCardRank	= rand()%4;
//	}while(deck[newCardRank][newCardValue]==0);
//	hand[0][*sizeh]=newCardValue;
//	hand[1][*sizeh]=newCardRank;
//	(*sizeh)++;
//	deck[newCardRank][newCardValue]=0;
//}
////1
//void blackjack(int number, int bot){	
//	int dealer;
//	getCardAll(2,number);
//	
//	for(int i = 0; i<=bot-1; i++){
//		int oBot = getBot();
//		getCard(botHand[oBot],&sizeBoth[oBot]);
//		getCard(botHand[oBot],&sizeBoth[oBot]);	
//	}
//	
//	//Choose dealer
//	while(1==1){
//		printf("\nWho do you want to be a dealer ?\n");
//		for(int i = 0; i<=number-1;i++) printf("\n%d. %s",i+1,pName[i]);
//		for(int i = 0; i<=bot-1;i++) printf("\n%d. Bot%d",i+number+1,i+1);
//		printf("\n");
//		scanf("%d",&dealer);
//		if(getchar()==10 && dealer <=number+bot) break;
//		printf("\nInvalid value. This value should be an integer between 1 and %d",number+bot);
//	}
//	int turn = dealer;
//	//Built turn-base structure
//BEGIN:
//	system("clear");
//	printf("\n\3\4\5\6\n");
//	int play = turn%(number+bot)+1;
//	if(dealer <= number ){
//		printf("\n%s's (Dealer) hand\n",pName[dealer-1]);
//		if (play!=dealer) showCard(botHand[dealer-1],0);
//		else showHand(hand[dealer-1],sizeh[dealer-1]);	
//	
//	}else{
//		printf("\nBot%d (Dealer) hand:\n",dealer-number);
//		if (play!=dealer) showCard(hand[dealer-number-1],0);
//		else showHand(botHand[dealer-number-1],sizeBoth[dealer-number-1]);	
//	}
//
//	for(int i = 0; i<=number-1; i++){
//		if(dealer-1!= i){
//			printf("\n%s's hand:\n",pName[i]);
//			showHand(hand[i],sizeh[i]);	
//		}
//	}
//	for(int i = 0; i<=bot-1;i++){
//		if(dealer-number-1!=i){
//			printf("\nBot%d's hand:\n",i+1);
//			showHand(botHand[i],sizeBoth[i]);
//		}
//	}
//	printf("\n=========================================\n");
//	if(play<=number){
//		printf("\n%s's turn:\n",pName[play-1]);	
//		while(1==1){
//			char c;
//			fflush(stdin);
//			printf("\nDo you want to end your turn ? (Y/N)\n");
//			if((c=toupper(getchar()))=='Y'){
//				if(play==dealer) {
//					checkWinBj(number,bot,dealer);
//					printf("\nThank you for playing !! - MyLL");	
//					return;
//				}
//				turn++;
//				goto BEGIN;
//			}
//			else if(c=='N'){
//				playBlackjack(hand[play-1],&sizeh[play-1]);
//				break;
//			}
//			printf("\nInvalid value!");
//			fflush(stdin);
//		}
//	}
//	else{
//		if(play==dealer){
//			checkWinBj(number, bot, dealer);
//			return;
//		}
//		printf("\nBot%d's turn",play-number);
//		botBj(botHand[play-number-1],&sizeBoth[play-number-1]);
//		turn++;
//	}
//	goto BEGIN;
//}
////1
//void playBlackjack(int hand[2][13], int *sizeh){
//	char c;
//	while(1==1){
//		fflush(stdin);
//		printf("Do you want to draw a card? (Y/N)\n");
//		if((c=toupper(getchar()))=='Y'){
//			getCard(hand,sizeh);
//			return;	
//		}
//		else return;
//		fflush(stdin);
//		printf("\nInvalid Value!");
//	}
//}
////1
//void checkWinBj(int number, int bot, int dealer){
//	int sum[number+bot];
//	int player[number+bot];
//	int winList[number+bot];
//	
//	//setup
//
//	for(int i = 0; i<=number-1; i++){
//		sum[i]= sumCard(hand[i],sizeh[i]);
//		player[i]=i+1;
//		winList[i]=0;	
//	}
//	for(int i = 0; i<=bot-1;i++){
//		sum[number+i]=sumCard(botHand[i],sizeBoth[i]);
//		player[number+i]=number+i+1;
//		winList[number+i]=0;
//	}
//
//	//Phan Loai
//	for(int i =0; i<=number+bot-1; i++){
//		if(i<number){
//			if(sizeh[i]==2 && (hand[i][0][0]==0 || hand[i][0][1]==0) && sum[i]==21) winList[i]=10;
//			else if(sum[i]<=21) winList[i]=1;
//		}
//		else{
//			if(sizeBoth[i]==2 && (botHand[i][0][0]==0 || botHand[i][0][1]==0) && sum[i]==21) winList[i]=10;
//			else if(sum[i]<=21) winList[i]=1;
//		}
//	}
//
//	//Sap xep phan loai
//	for(int i = 0; i<=number+bot-2; i++){
//		for(int j = i+1; j<=number+bot-1; j++){
//			if(winList[i]<winList[j]){
//				swap(&winList[i],&winList[j]);
//				swap(&player[i],&player[j]);
//				swap(&sum[i],&sum[j]);
//			}
//		}
//	}
//
//	
//	//Sap xep dong loai
//	for(int i = 0; i<=number+bot-2; i++){
//		for(int j=i+1;j<=number+bot-1;j++){
//			if(sum[i]<sum[j]&&winList[i]!=10){
//				swap(&sum[i],&sum[j]);
//				swap(&player[i],&player[j]);
//				swap(&winList[i],&winList[j]);
//			}
//		}
//	}
//
//	//Diem thuong, Diem phat
//	for(int i = number+bot-1;i>=0;i--){
//		if(winList[i]!=10){
//			if(winList[number+bot-1-i]==0)winList[number+bot-1-i]-=i;	
//			if(winList[number+bot-1-i]==1)winList[number+bot-1-i]+=i;	
//		}
//	}
//
//	
//	//Sap xep phan loai
//	for(int i = 0; i<=number+bot-2; i++){
//		for(int j = i+1; j<=number+bot-1; j++){
//			if(winList[i]<winList[j]){
//				swap(&winList[i],&winList[j]);
//				swap(&player[i],&player[j]);
//				swap(&sum[i],&sum[j]);
//			}
//		}
//	}
//
//	//Xet truong hop cung gia tri
//	for(int i = 0; i<=number+bot-2;i++) if(sum[i]==sum[i+1]) winList[i+1]=winList[i];
//
//	//Tim vi tri cua dealer
//	int pos1;
//	for(int i = 0; i<=number+bot-1;i++) if(player[i]==dealer){
//		pos1 = i;
//		break;
//	}
//
//	int count = 0;
//	for(int i = 0; i<=number+bot-1;i++){
//		if(i!=pos1){
//			if(winList[i]>winList[pos1]){
//				count--;
//				if(player[i]<=number){
//					printf("\n%s + 500$",pName[player[i]-1]);	
//					account[player[i]-1]+=500;
//				}
//			}
//			if(winList[i]<winList[pos1]){
//				count++;
//				if(player[i]<=number){
//					printf("\n%s- 500$",pName[player[i]-1]);
//					account[player[i]-1]-=500;	
//				}
//			}
//			if(winList[i]==winList[pos1] && player[i]<=number) printf("\n%s + 0$",pName[player[i]-1]);	
//		}
//	}
//	if(dealer<=number){
//		if(count>=0) printf("\n%s (Dealer) + %d$",pName[dealer-1],count*500);
//		else printf("\n%s (Dealer) - %d$",pName[dealer-1],-count*500);
//		account[dealer-1]+=count*500;		
//	}
//	for(int i = 0; i<= number-1; i++) printf("\n%s's balance:	%d",pName[i], account[i]);
//}
////1
//int sumCard(int hand[2][13], int sizeh){
//	int sum=0;
//	int flag = 0;
//	for(int i = 0; i<=sizeh-1;i++){
//		if(hand[0][i]>=9) sum+=9;
//		else if(hand[0][i]==0) flag++;
//		else sum+=hand[0][i];
//	}
//	for(int i = 1; i<=flag;i++){
//		sum+=10;
//		if(sum+sizeh>21) sum-=1;
//		if(sum+sizeh>21) sum-=9;
//	}
//	return sum+sizeh;
//}
////1
//void swap(int* a, int* b){
//	int t = *a;
//	*a = *b;
//	*b = t;
//}
////1
//void swapHand(int order1, int order2){
//	int maxsizeh = sizeh[order1] >= sizeh[order2] ? sizeh[order1]:sizeh[order2];
//	for(int i = 0; i<=maxsizeh-1;i++) for(int j = 0; j<=1;j++) swap(&hand[order1][j][i],&hand[order2][j][i]);	
//	swap(&sizeh[order1],&sizeh[order2]);
//}
////1
//void botBj(int botHandB[2][13], int *sizeBothB){
//
//	printf("\nDo you want to end your turn? (Y/N)\n");
//	sleep(1.25);
//	while(1==1){
//		if(sumCard(botHandB,*sizeBothB)>=14){
//			printf("Y");
//			sleep(1.25);
//			return;	
//		}
//		else{
//			printf("N");
//			sleep(1.25);
//			printf("\nDo you want to draw a card? (Y/N)\n");
//			sleep(1.25);	
//			printf("Y");
//			sleep(1.25);
//			getCard(botHandB,sizeBothB);
//			return;
//		}	
//	}
//}
////1
//int getBot(){
//	for(int i = 0; i<=8; i++) if(botManage[i]==0){
//		botManage[i]=1;
//		return i;
//	}
//}
////1
//void menu(){
//	system("clear");
//	printf("\n\3\4\5\6\n");
//	printf("\nWelcome to blackjack program created by MyLL");
//	printf("\n\nThis is our menu\n\n");
//	printf("\n1. Play Blackjack.");
//	printf("\n2. Setup game.");
//	printf("\n3. Update Player's Information.");
//	printf("\n");
//	char choice;
//	scanf("%c",&choice);
//	fflush(stdin);
//	switch (choice){
//		case '1':
//			if(setup==0) setupGame();
//			if(update==0)updatePName();
//			blackjack(number,bot);
//			oldcount=count;
//			count++;
//			break;
//		case '2':
//			setupGame();
//			break;
//		case '3':
//			if (setup==0) setupGame(); 
//			updatePName();
//			break;
//		default:
//			printf("\nInvalid Value! ");
//	}
//	return;
//}
////1
//void setupGame(){
//	while(1==1){
//		printf("\nHow many players want to play Blackjack?\n");
//		scanf("%d",&number);
//		if(getchar()==10&&number>=0&&number<9) break;
//		printf("\nInvalid value! The value should be an integer number between 0 and 8\n");
//	}
//	while(number+bot<8){
//		char c;
//		printf("\nDo you want to add bot(s) into this lobby ? (Y/N)\n");
//		if((c=toupper(getchar()))=='Y'){
//			while(1==1){
//				printf("\nHow many bots do you want to add?\n");
//				scanf("%d",&bot);
//				if(getchar()!=10 || bot+number>8)	printf("\nInvalid value. This field requires an integer, and there can only 8 hands in a lobby.");
//				else{
//					setup=1;
//					return;	
//				}
//			}
//		}
//		else if(c=='N'){
//			setup=1;
//			break;
//		}
//		else printf("\nInvalid value.");
//	}
//}
////1
//void updatePName(){
//	system("clear");
//	if(update==1){
//		int choice;
//		while(1==1){
//			printf("Which player do you want to update information?\n\n");
//			for(int i = 0; i<=number-1; i++) printf("\n%d. %s",i, pName[i-1]);
//			scanf("%d",&choice);
//			if(getchar()==10) break;
//			else printf("\nInvalid value! ");
//		}
//		printf("\nNew name of %s is :	",pName[choice-1]);
//		gets(pName[choice-1]);
//		printf("New name updated!");	
//	}
//	else{
//		if(setup==0)setupGame();
//		for(int i = 0; i<=number-1;i++){
//			printf("\nPlease input player %d's name:	",i+1);
//			gets(pName[i]);
//		}
//	}
//	update=1;
//}
