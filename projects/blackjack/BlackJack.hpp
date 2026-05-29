#ifndef BLACKJACK_HPP_INCLUDE_GUARD
#define BLACKJACK_HPP_INCLUDE_GUARD

#include <string>
#include <map>
#include <vector>
#include <unordered_set>
#include <iostream>
#include <stdlib.h>

namespace bj {

using namespace std;

enum game_state {
    BET,
    DEAL,
    HIT,
    SHOW
};


enum card_suit {
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES,
    SUIT_MAX
};

enum card_rank {
    ACE = 1,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    RANK_MAX
};

class Card {
    private:
    public:
	// Card() : suit(""), rank(){}
	static map<card_suit, string> suit_map;
	card_suit suit;
	card_rank rank;
	// Constructor
	Card(card_suit s, card_rank r) : suit(s), rank(r) {
	    	}
	
	bool operator==(Card c2) {
	    if (suit == c2.suit && rank == c2.rank) return true;
	    return false;
	}

	string print () {
	    string suit_str = suit_map[suit];
	    string rank_str = "";
	    if (rank > ACE && rank <= TEN) {
		rank_str = to_string((int)rank);
	    }
	    else {
            switch (rank) {
                case (ACE):
                rank_str = "A";
                break;
                case (JACK):
                rank_str = "J";
                break;
                case (QUEEN):
                rank_str = "Q";
                break;
                case (KING):
                rank_str = "K";
                break;
                default:
                break;
            }
	    }
	    return (suit_str + "_" + rank_str);
	}
};

map<card_suit, string> Card::suit_map = {
		{HEARTS, "hearts"},
		{DIAMONDS, "diamonds"},
		{CLUBS, "clubs"},
		{SPADES, "spades"}
	    };


class Deck {
    private:
	vector<Card> deck;
    public:
	Deck() {
	    for (int s = HEARTS; s < (int)SUIT_MAX; s++){
		for (int r = ACE; r < (int)RANK_MAX; r++) {
		    deck.push_back(Card((card_suit)s,(card_rank)r));
		}
	    }
	}

	void printDeck() {
	    for (auto c = deck.begin(); c != deck.end(); c++) {
		    c->print();
	    }
	}

	void shuffle() {
	    for (int i = 0; i < deck.size(); i++) {
		Card c1 = deck[i];
		int j = rand() % deck.size(); // new placement from 0...deck.size()
		if (j == i) return;

		deck[i] = deck[j];
		deck[j] = c1;
	    }
	}

	bool verify() {
	    if (deck.size() != 52) return false;

	    map<card_suit, unordered_set<int>> s;
	    
	    // deck[1] = deck[0];

	    for (int i = 0; i < deck.size(); i++) {
		Card c = deck[i];
		if (s[c.suit].count(c.rank) != 0) {
		    return false;
		}
		s[c.suit].insert(c.rank);
	    }
	    return true;
	}

	Card removeOne() {
	    Card c = deck[0];
	    deck.erase(deck.begin());
	    cout << "\ndeal one card...\n";
	    c.print();
	    cout << "\n";
	    return c;
	}
};

class Hand {
    private:
	vector<Card> hand;
    public:
	vector<string> print() {
	    vector<string> ret;
	    for (auto c = hand.begin(); c != hand.end(); c++) {
		    ret.push_back(c->print());
	    }
	    return ret;
	}

	void insert(Card c) {
	    hand.push_back(c);
	}

	int totalValue() {
	    int numAces = 0;
	    int total = 0;
	    for (auto c = hand.begin(); c != hand.end(); c++) {
            if (c->rank == ACE) {
                numAces++;
            }
            else if (c->rank == KING || c->rank == QUEEN || c->rank == JACK) {
                total += 10;
            }
            else {
                total += (int)c->rank;
            }
	    }

	    if (total > 21 || total + (numAces * 10) <= 21) {
            return total + (numAces * 10);
        }

	    int numOnes = 1;
	    int numTens = numAces - 1;
	    while (numTens > 0 && (numTens * 10 + numOnes + total > 21)) {
            numTens--;
            numOnes++;
	    }

	    return (numTens * 10) + numOnes + total;
	}
};

class Blackjack {
    private:
	int players;
	Deck deck;
	Hand dealerHand;
	vector<Hand> playerHands;
	

    public:
    game_state state;
    int currentPlayer;
    
	Blackjack (int p) {
	    players = p;
	    state = DEAL;
	    currentPlayer = 0;
	    srand(time(0));
	}
	
	vector<Hand> getPlayerHands() {
	    return playerHands;
	}
	
	Hand getDealerHand() {
	    return dealerHand;
	}
    
    // dealer hand is at index 0
	vector<vector<string>> showHands() {
	    vector<vector<string>> ret;
	    ret.push_back(dealerHand.print());
	    for (int i = 0; i < players; i++) {
		    ret.push_back(playerHands[i].print());
	    }
	    return ret;
	}
	
	void Start () {
	    // cout << "\nStarting Game! \n";
	    deck.shuffle();
	    dealerHand.insert(deck.removeOne());
	    
//	    cout << "dealer shows: \n";
//	    dealerHand.print();
//	    cout << "\n";

	    for (int i = 0; i < players; i++) {
		    playerHands.push_back(Hand());
		    playerHands[i].insert(deck.removeOne());
		    playerHands[i].insert(deck.removeOne());
	    }
	    
	    state = HIT;
	}
	
	// true for hit
	void inputHit (bool resp) {
	    if (state != HIT) {
	        return;
	    }
	    
	    if (resp == true) {
            Card c = deck.removeOne();
	        playerHands[currentPlayer].insert(c);
	    
	        if (playerHands[currentPlayer].totalValue() >= 21) {
	            currentPlayer++;
	        }
	    }
	    else {
	        currentPlayer++;
	    }
	    
        if (currentPlayer >= players) {
            state = SHOW;
            while (dealerHand.totalValue() < 17) {
                dealerHand.insert(deck.removeOne());
            }
        }
	}
	
	vector<int> getWinners () {
	    vector<int> winners = {};
	    cout << "winners size: " << winners.size() << "\n";
        int dv = dealerHand.totalValue();
	    for (int i = 0; i < players; i++) {
		    int pv = playerHands[i].totalValue();
		    
		    if (pv <= 21 && (pv > dv || dv > 21)) {
		        winners.push_back(i);
                cout << i << " is a winner\n";
		    }
	    }
	    cout << "There are " << winners.size() << " winners\n";
        return winners;
	}
	

//	    showHands();

//	    	    
//	    string ans = "y";
//
//	    for (int i = 0; i < players; i++) {
//	    	while (1) {
//		    cout << "Player" << i << " you have " << playerHands[i].totalValue()<< ", hit? (y/n): ";
//		    cin >> ans;
//		    cout << "\n";
//
//		    if (ans != "y" && ans != "Y") {
//			break;
//		    }
//		    Card c = deck.removeOne();
//		    playerHands[i].insert(c);
//
//		    if (playerHands[i].totalValue() > 21) {
//			cout << "BUST\n";
//			break;
//		    }
//		    else if (playerHands[i].totalValue() == 21) {
//			cout << "BLACKJACK\n";
//			break;
//		    }
//		    else {
//			showHands();
//			// cout << "player hand: \n";
//			// playerHands[i].print();
//			// cout << "total value: " << playerHands[i].totalValue() << "\n\n";
//		    }
//		}
//	    }
//	    
//	    while (dealerHand.totalValue() < 17) {
//		dealerHand.insert(deck.removeOne());
//		cout << "Dealer has " << dealerHand.totalValue() << "\n";
//	    }
//
//	    showHands();
//
//	    for (int i = 0; i < players; i++) {
//		int pv = playerHands[i].totalValue();
//		int dv = dealerHand.totalValue();
//
//		if (pv <= 21 && (pv > dv || dv > 21)) {
//		    cout << "\nPlayer " << i << " wins!\n";
//		}
//		else if (pv == dv) {
//		    cout << "\nPlayer " << i << " push!\n";
//		}
//		else {
//		    cout << "\nDealer beats Player " << i << "!\n";
//		}
//	    }
//
//
//	}
};

} //namespace bj
#endif // BLACKJACK_HPP_INCLUDE_GUARD
