#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>



//represents a card -- 52 per deck
typedef struct card {
	char * suit;
	char * rank;
} card;

//defines a player of the game
typedef struct player {
	char * playersName;
	int playersChipsInUSD;
	int currentBet;
	int handValue;
} player;

//defines a dealer 
typedef struct dealer {
	int handValue;
} dealer;

//score any card and add it 
//to the players total
int scoreCard (card * c) {
	char * rank =  c->rank; 
	
	if(!strcmp(rank, "1")) return 1; 
	if(!strcmp(rank, "2")) return 2;
	if(!strcmp(rank, "3")) return 3;
	if(!strcmp(rank, "4")) return 4;
	if(!strcmp(rank, "5")) return 5;
	if(!strcmp(rank, "6")) return 6;
	if(!strcmp(rank, "7")) return 7;
	if(!strcmp(rank, "8")) return 8;
	if(!strcmp(rank, "9")) return 9;
	if(!strcmp(rank, "10")) return 10;
	if(!strcmp(rank, "J")) return 10;
	if(!strcmp(rank, "Q")) return 10;
	if(!strcmp(rank, "K")) return 10;
	// if(!strcmp(rank, "A")) scoreAce();
	
}

//makes a new deck of cards
card * makeDeck() {
	int sizeOfDeck = 52;
    int numOfSuits = 4;
    int numOfRanks = 13;
	char * suitsArray[] = {"C", "D", "H", "S"};
	char * rankArray[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
	//allocate memory on the heap for an array of card structs (a deck of cards)
	card * deck = malloc(sizeof(card) * sizeOfDeck);
	int i, j;
	int k = 0;
	for(i = 0; i < numOfSuits; i++){
		for(j = 0; j < numOfRanks; j++){
			deck[k].suit = suitsArray[i];
			deck[k].rank = rankArray[j];
			k++;
		}
	}
	return deck;
}

//prints the deck of cards. Useful for debugging if necessary
void printDeck(card * theDeckPointer) {
	int i = 0;
	for (i = 0; i < 52;  i++) {
		printf("%s%s\n", theDeckPointer[i].rank, theDeckPointer[i].suit);
	}
}

//deal a random card from the deck any time a player or dealer 
//asks to hit or is dealt hand
card * dealRandomCard(card * aDeck) {
	card * cardDealt;
	//static float i = 0;
	int x = rand() % 53;
	cardDealt = &(aDeck[x]);

	return cardDealt;
}

//free the memory allocated in the creation of the deck and player
void freeAll(card * deckToFree, player * playerToFree) {
	int i = 0;
	while (i < 52) {
		free(deckToFree + i);
		i++;
	}
	free(playerToFree);
}

//the welcom message to be printed at the start of the program
void printWelcome() {
	printf("WELCOME TO THE GAME OF 21!\n");
}

//prints the current player and his/her 
//chip count to the screen
void printPlayer(player * playerToPrint) {
	printf("The current player's name is: %s", playerToPrint->playersName);
	printf("The current player's chip count is: $%d\n", playerToPrint->playersChipsInUSD);
	printf("The current player's bet is: $%d\n", playerToPrint->currentBet);
}

//welcomes and initializes a new player with a name and 
//an initial chip count of 500 USD
player * newPlayer() {
	player * newPlyr = malloc(sizeof(player));
	char * name = malloc(sizeof(char) * 50);

	printf("Please enter your name (and press enter): \n");
	fgets(name, sizeof(name) * 50, stdin);
	printf("\n");
	printf("You'll start with $500 worth of chips. You may bet as little or as much as you'd like on each hand. \n"); 
	printf("A win returns 1X your total bet, while blackjack returns 1.5X your total bet.\n"); 
	printf("The dealer will hit on anything less than 17 and stand on any hand greater or equal to 17.\n");
	printf("Good luck %s\n", name);
	newPlyr->playersName = name;
	newPlyr->playersChipsInUSD = 500;

	return newPlyr;
}

//print a card
void printCard(card * c) {
	printf("Rank: %s\n", c->rank);
	printf("Suit: %s\n", c->suit);
}

//initializes the game with creation of a deck 
//and printing of the welcome message
card * initializeGame() {
	printf("\n");
	card * deckOfCardsPointer;
	deckOfCardsPointer = makeDeck();
	printWelcome();
	return deckOfCardsPointer;	
}

void placeBet(player * p) {
	char line[25];
	int bet;
	
	printf("Enter your bet (then hit 'enter'): \n");
	if (fgets(line, sizeof(line), stdin)) {
    	if (1 == sscanf(line, "%d", &bet)) {
        	p->currentBet = bet;
    	}
	}
}

void dealDealer(dealer * d, card * deckOfCards) {
	d->handValue = 0;
	card * firstCard = dealRandomCard(deckOfCards);
	int firstVal = scoreCard(firstCard);
	d->handValue = firstVal;
	
	printf("Dealer's first card dealt was %s%s. (Other's will remain hidden.)\n\n", firstCard->rank, firstCard->suit);

	while (d->handValue < 17) {
		card * newCard = dealRandomCard(deckOfCards);
		int val = scoreCard(newCard);
		d->handValue = d->handValue + val;
		newCard = NULL;
	}

}

void dealPlayer(player * p, card * deckOfCards) {
	char hitOrStay[6];
	p->handValue = 0;
	card * dealt = dealRandomCard(deckOfCards);
	int firstVal = scoreCard(dealt);
	d->handValue = firstVal;
	
	while(1) {
		printf("You were dealt a %s%s\n", dealt->rank, dealt->suit);
		printf("Would you like to hit or stay?\n");
		fgets(hitOrStay, sizeof(char) * 6, stdin);
		if (strcmp(hitOrStay, "hit")) {
			break;
		}
		dealt = dealRandomCard(deckOfCards);
		int val = scoreCard(dealt);
		d->handValue = d->handValue + val;
		printf("%d ", d->handValue);
	}

}

void playNewHand(player * p, dealer * d, card * deckOfCards) {

	placeBet(p);
	dealDealer(d, deckOfCards);
	dealPlayer(p, deckOfCards);
	//deal both dealer and player
	//check for duplicates
	//score both hands once player stays
	//recalculate players bank account and reset to defaults
	p->handValue = 0;
	d->handValue = 0;
	//free hands

}

void cashOut() {

}

//the standard main method, for the blackjack game
void main() {
	srand(time(NULL));
	card * deckOfCards = initializeGame();
	player * p = newPlayer();
	dealer * d = malloc(sizeof(dealer));
	
	// char * keepPlaying;

	// while () {
		playNewHand(p, d, deckOfCards);
	// }
	// cashOut();

	// //free deck and player
	// return 1;
}