#include "cppgraphics.hpp"
#include "BlackJack.hpp"

#include <vector>

// Set number of circles in the background.
const int N = 5000;

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 450

using namespace std;
using namespace bj;

//enum button_press {
//    HIT,
//    STAY,
//    NONE
//};

void draw_buttons()
{
    cg::set_fill_color(cg::Yellow);
    cg::set_thickness(0.);
    double width = 40.;
    double height = 40.;
    double y = 400.;
    cg::rectangle(600., y, width, height);
    cg::rectangle(700., y, width, height);
    cg::set_color(cg::Black);
    cg::text("HIT",  600., y, width, height);
    cg::text("STAY", 700., y, width, height);
    
}

int start_screen() {
    cg::clear();
    cg::set_color(cg::Black);
    int x = SCREEN_WIDTH / 2;
    int y = SCREEN_HEIGHT / 2;
    cg::text_centered("Welcome to Blackjack", x, y, 60);
    cg::text_centered("Enter number of players", x, y + 70, 40);
    cg::image("./images/king of hearts.jpg", 0,0, 50, 70);
    string num_players = cg::read_line(x, y + 110, 40, false, 2);
    return stoi(num_players);
    
}

void show_hands(bj::Blackjack& game) {
    int x = 0;
    int y = 0;
    // vector<vector<string>> hands = game.showHands();
    
    cg::text("Dealer:", x, y);
    x = 170;
    Hand dealer_hand = game.getDealerHand();
    vector<string> dhp = dealer_hand.print();
    for (int i = 0; i < dhp.size(); i++) { 
	    //y += 25;
        cg::image("./images/playing-cards/" + dhp[i] + ".png", x, y, 60, 80);
	    x += 70;
        // cg::text(dhp[i], x, y);
    }

    // show the dealer's other card
    if (game.state == HIT) {
        cg::image("./images/playing-cards/back_dark.png", x, y, 60, 80); 
    }
    
    x = 0;
    y += 25;
    cg::text("Total: " + to_string(dealer_hand.totalValue()), x, y);
    
    vector<Hand> player_hands = game.getPlayerHands();
    for (int i = 0; i < player_hands.size(); i++) {
        y += 60;
        vector<string> php = player_hands[i].print();
        if (i == game.currentPlayer && game.state == HIT) {
            cg::set_color(cg::Yellow);
        }
        else {
            cg::set_color(cg::Black);
        }
        cg::text("Player " + to_string(i) + ":", x, y);
        x = 170;
        for (int j = 0; j < php.size(); j++) {
	        // y += 25;
            cg::image("./images/playing-cards/" + php[j] + ".png", x, y, 60, 80);
            x += 70;
	        // cg::text(php[j], x, y);
	    }
        x = 0;
        y += 25;
        cg::text("Total: " + to_string(player_hands[i].totalValue()), x, y);
    }
    
    y = SCREEN_HEIGHT - 50;
    x = 0;
    
    cg::set_color(cg::Black);
    if (game.state == HIT) {
        cg::text("Player " + to_string(game.currentPlayer) + ", hit or stay?", x, y);
    }
}

int main()
{
    // Create our window and generate N random circles:
    cg::create_window("Blackjack", SCREEN_WIDTH, SCREEN_HEIGHT);
    cg::set_background_color(cg::Green);
    int mx = 0.0;
    int my = 0.0;
    
    int players = start_screen();
    bj::Blackjack my_game(players);
    my_game.Start();

    while (cg::refresh() && my_game.state == HIT) {
        cg::clear();
        draw_buttons();
        cg::set_color(cg::Black);
        show_hands(my_game);
        
        cg::text("Mouse X: " + to_string(mx), 600., 0.);
        cg::text("Mouse Y: " + to_string(my), 600., 30.);
        int kp = cg::wait_until_mouse(-1);
        if (kp == cg::MouseLeft) {
            mx = cg::get_mouse_x();
            my = cg::get_mouse_y();
        }
        if (mx > 600 && mx < 640 && my > 400 && my < 440) {
            my_game.inputHit(true);
        }
        else if (mx > 700 && mx < 740 && my > 400 && my < 440) {
            my_game.inputHit(false);
        }
    }
    
    cg::clear();
    cg::set_color(cg::Black);
    show_hands(my_game);
    vector<int> winners = my_game.getWinners();
    int x = 0;
    int y = SCREEN_HEIGHT - 50;
    
    string ws = "";
    for (int i = 0; i < winners.size(); i++) {
        ws += to_string(winners[i]);
        ws += i == winners.size() - 1 ? "" : ", ";
    }
    cg::text("Winners: " + ws, x, y);
    
    cg::wait_until_mouse(-1);
    // Better, wasn't it?

    ///////////////////////////////////////////////////////////////////////////

    return 0;
}
