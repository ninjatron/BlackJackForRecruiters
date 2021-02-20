#ifndef BLACKJACK
#define BLACKJACK

#include <cctype>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <random>

using namespace std;

unordered_map<string, int> cards{
  {"Two", 2},
  {"Three", 3},
  {"Four", 4},
  {"Five", 5},
  {"Six", 6},
  {"Seven", 7},
  {"Eight", 8},
  {"Nine", 9},
  {"Ten", 10},
  {"Jack", 10},
  {"Queen", 10},
  {"King", 10},
  {"Ace", 1}
};

struct Card {
  string suit;
  string rank;
  int value;
};

struct Player {
  Player(string playername) : hasAce(0), handValue(0), wins(0), name(playername) {};
  int hasAce;
  int handValue;
  int wins;  
  string name;
  vector<Card> hand;
};

class Blackjack {
  public:
    Blackjack(string username) : DECK_SIZE(52), BUST(22), deck(DECK_SIZE), 
                                 dealer("Dealer"), gamer(username) {};
    void start() {
      resetDeck();
      initHand(&dealer);
      initHand(&gamer);
    }

    void resetDeck() {
      deck.clear();
      for (auto p : cards) {
        // initialize 4 of each cards
        for (int j = 0; j < 4; ++j) {
          Card c = { Suit[j], p.first, p.second };
          deck.push_back(c);
        }
      }
      
      random_device randev;
      mt19937 mersenne_gen(randev());
      shuffle(begin(deck), end(deck), mersenne_gen);
    }

    void initHand(Player *p) {
      p->hand.clear();
      p->handValue = 0;
      p->hasAce = 0;
      for (int i = 0; i < 2; ++i) {
        if (deck.back().rank == "Ace") p->hasAce = 10;
        p->handValue += deck.back().value;
        p->hand.push_back(deck.back());
        deck.pop_back();
      }
    }
    
    void hitOneCard(Player *p) {
      cout << p->name << " has been dealt " << deck.back().rank << " of "
           << deck.back().suit << "\n"; 
      if (deck.back().rank == "Ace") p->hasAce = 10;
      p->handValue += deck.back().value;
      p->hand.push_back(deck.back());
      deck.pop_back();
      if (p->handValue >= BUST) {
        cout << p->name << " busted.\n\n";
      }
    }

    Player * determineResult() {
      if (gamer.handValue + gamer.hasAce < BUST) gamer.handValue += gamer.hasAce;
      if (dealer.handValue + dealer.hasAce < BUST) dealer.handValue += dealer.hasAce; 
      if (gamer.handValue >= BUST && dealer.handValue >= BUST) return nullptr;
      else if (gamer.handValue >= BUST) return &dealer;
      else if (dealer.handValue >= BUST) return &gamer;
      else if (dealer.handValue > gamer.handValue) return &dealer;
      else if (gamer.handValue > dealer.handValue) return &gamer;
      else return nullptr;
    }

    bool dealerContinuesPlay() {
      int gamerWithoutAce = gamer.handValue;
      int gamerWithAce = gamer.handValue + gamer.hasAce;
      int dealerWithoutAce = dealer.handValue;
      int dealerWithAce =  dealer.handValue + dealer.hasAce;
      // if it is less than 17 dealer has to hit regardless player bust
      if (dealerWithAce < 17) return true;
      else if (dealerWithAce >= BUST) { // has to play without aces here
        if (dealerWithoutAce < 17) return true; // again has to hit
        else if (dealerWithoutAce >= BUST) return false; // busted should stop play
        else if (gamerWithAce >= BUST) { // gamerWithAce is also bust
          if (dealerWithoutAce < gamerWithoutAce && gamerWithoutAce < BUST) return true;
          else return false;
        }  
        else if (dealerWithoutAce < gamerWithAce) return true;
        return false;
      }
      else if (dealerWithAce < gamerWithAce) {
        if (gamerWithAce < BUST) return true;
        else if (gamerWithAce >= BUST) return false;
        else if (gamerWithoutAce >= BUST) return false;
        else if (dealerWithAce < gamerWithoutAce) return true;
        else return false;
      } 
      
      return false;
    }

    void dealerSimulate() {
      while (dealerContinuesPlay()) {
        hitOneCard(&dealer);
      }
    }

    void updateScore(Player *p) {
      if (p) p->wins++;
    }

    // show 1 open card to player
    void showDealerTopCard() {
      cout << "\n" << dealer.hand[0].rank << " of " << dealer.hand[0].suit << "\n\n";
    }

    void showHand(Player *p) {
      cout << p->name << "'s hand is:\n";
      for (auto c : p->hand) {
        cout << c.rank << " of " << c.suit << "\n";
      }
      // cout << "for a total hand value of " <<  p->handValue << "\n";
    }

    Player * getGamer() {
      return &gamer;
    }

    Player * getDealer() {
      return &dealer;
    }

  private:
    const int DECK_SIZE;
    const int BUST;
    vector<string> Suit{"Diamonds", "Hearts", "Spades", "Clubs"};
    vector<Card> deck;
    Player dealer;
    Player gamer;
};

#endif