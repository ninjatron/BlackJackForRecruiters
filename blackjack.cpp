#include <iostream>
#include "blackjack.h"

// don't do this in large projects
using namespace std;

void announceScore(Blackjack *g) {
    cout << "\nDealer has " << g->getDealer()->wins << " wins, " << g->getGamer()->name 
         << " has " << g->getGamer()->wins << " wins\n"; 
}

void announceDealEnd(Blackjack *g) {
    if (g->determineResult()) {
      cout << "\nTurn is over, winner is " << g->determineResult()->name << "\n";
      g->updateScore(g->determineResult());
    } else {
      cout << "\nTurn is over, it is a draw.\n";
    }
}

int main() {
  string name = "Playa";
  
  cout << "Welcome to BlackJack, enter player name: ";
  cin >> name;
  cout << "\nAlright, " << name << " let's start the game. " 
       << "To quit at any time enter Q when prompted\n";
  
  Blackjack game(name);
  
  while (true) {
    // game starts, deck, player hand and dealer hand is ready
    char ch = '\0';
    game.start();
    cout << "\nHands have been dealt, dealer's open card is: ";
    game.showDealerTopCard();
    
    while ((ch == '\0' || ch == 'h') && game.getGamer()->handValue < 22) {
      game.showHand(game.getGamer());
      cout << "\nTo get a new card enter H, to stand press any other keys: ";
      cin >> ch;
      ch = tolower(ch);
      if (ch == 'h') {
        game.hitOneCard(game.getGamer());
      }
      if (ch == 'q') {
        cout << "Thanks for playing BlackJack\n";
        announceScore(&game);
        cout << "Game over\n";
        cout << endl; // flush the buffer
        return 0;
      }
    }

    if (game.getGamer()->handValue < 22 || game.getDealer()->handValue < 17) {
      game.showHand(game.getDealer());
      game.dealerSimulate();
    }

    // show at least once even if player is bust beforehand
    game.showHand(game.getDealer());

    announceDealEnd(&game);
    announceScore(&game);
    cout << "##############################################################\n";
    cout << "Dealing the next hand\n";

  } // end of while

  return 0;

} // end of main