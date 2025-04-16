/**
 * @file player_example.cpp
 * @author Matthew Getgen
 * @brief The starter file for making your own AI.
 * @date 2022-11-22
 */

#include "Nate_Dave.h"


// Write your AI's name here. Please don't make it more than 64 bytes.
#define AI_NAME "Player Example C++"

// Write your name(s) here. Please don't make it more than 64 bytes.
#define AUTHOR_NAMES "Nate Paarlberg & Manzi Dave Rugari"


/*================================================================================
 * Starts up the entire match. Do NOT change anything here unless you really understand
 * what you are doing.
 *================================================================================*/
int main(int argc, char *argv[]) {
    // player must have the socket path as an argument.
    if ( argc != 2 ) {
        printf("%s Error: Requires socket name! (line: %d)\n", AI_NAME, __LINE__);
        return -1;
    }
    char *socket_path = argv[1];
    
    // set random seed
    srand(getpid());

    Nate_Dave my_player = Nate_Dave();
    return my_player.play_match(socket_path, AI_NAME, AUTHOR_NAMES);
}

Nate_Dave::Nate_Dave():Player() {
    return;
}

Nate_Dave::~Nate_Dave() {
    return;
}

/*================================================================================
 * This is like a constructor for the entire match.
 * You probably don't want to make changes here unless it is something that is done once at the beginning 
 * of the entire match..
 *================================================================================*/
void Nate_Dave::handle_setup_match(PlayerNum player, int board_size) {
    this->player = player;
    this->board_size = board_size;
    minShipLen = 0;
    create_boards();
    return;
}

/*================================================================================
 * This is like a constructor for one game/round within the entire match.
 * Add anything here that you need to do at the beginning of each game.
 *================================================================================*/
void Nate_Dave::handle_start_game() {
    clear_boards();
    return;
}

/*================================================================================
 * Example of how to decide where to place a ship of length ship_length and
 * inform the contest controller of your decision.
 *
 * If you place your ship even a bit off the board or collide with a previous ship
 * that you placed in this round, you instantly forfeit the round.
 *
 * TLDR: set ship.len, ship.row, and ship.col to good values and return ship.
 *================================================================================*/
Ship Nate_Dave::choose_ship_place(int ship_length) {
    num_ships_placed++;
    
    int total = 0;
    int best_total = 10000;

    Ship ship;
    ship.len = ship_length;
    //ship.row = 0;
    //ship.col = 0;
    ship.dir = HORIZONTAL;
    while(true){
        ship.dir= random()%2==0?HORIZONTAL:VERTICAL;
        //ship.col = random()%board_size;
        ship.row = random()%board_size;
       /* while(ship.row > 2 && ship.row < 7){
           ship.row = random()%board_size;
        }
        while(ship.col > 2 && ship.col < 7){
            ship.col = random()%board_size;
        }*/
        ship.col = random()%board_size;
        
        ship.len = ship_length;
        
        for (int r=0; r<board_size-ship.len; r++){
            for (int c=0; c<board_size-ship.len; c++){
                total =0;
                
                for (int i=0; i<=ship.len; i++){
                    total += this->their_shots[r][c+i];
                }    
                    if (total < best_total){
                        best_total = total;
                        ship.row = r;
                        ship.col = c;
                    }
                
            }
        }

        for (int c=0; c<board_size-ship.len; c++){
            for (int r=0; r<board_size-ship.len; r++){
            total =0;

            for (int i=0; i<=ship.len; i++){
                total += this->their_shots[r+i][c];
            }
                if (total < best_total){
                    best_total = total;
                    ship.row = r;
                    ship.col = c;
                }

            }
        }




        if (safeShip(ship)){
            markShip(ship);
            return ship;
        }
    }
    return ship;

}
    
    
bool Nate_Dave::safeShip(Ship ship){
    if(ship.dir==HORIZONTAL){
            if(ship.col+ship.len>board_size)return false;
                for (int i=0; i<ship.len; i++){
                    if(ship_board[ship.row][ship.col+i]!=WATER){
                        return false;// =ship 
                    }   
                }
      return true;
     } 
     else{
         if(ship.row+ship.len>board_size)return false;
            for (int i=0; i<ship.len; i++){
                if(ship_board[ship.row+i][ship.col]!=WATER){
                return false; 
                }              
            }
     return true;
     }
}
Ship Nate_Dave::markShip(Ship ship){
    if(ship.dir == HORIZONTAL){
        for (int len = 0; len < ship.len; len++) {
            this->ship_board[ship.row][ship.col+len] = SHIP;
        }
        return ship;
    }
    else{
        for (int len = 0; len < ship.len; len++) {
            this->ship_board[ship.row+len][ship.col] = SHIP;
            
        }
        return ship;
    }
}


/*================================================================================
 * Example of how to decide where to shoot and inform the contest controller
 * of your decision.
 *================================================================================*/
Shot Nate_Dave::choose_shot() {
    int best_prob = 0; 
    int best_row = 0; 
    int best_col = 0;

    Shot shot;
    int prob[board_size][board_size];
    for(int r=0; r < board_size; r++){
        for(int c=0; c < board_size; c++){
            prob[r][c] = hprob(r,c)+vprob(r,c);
         
            if(prob[r][c] > best_prob){ 
                best_prob = prob[r][c];
                best_row = r;
                best_col = c;
            }
        }
    }
    shot.row=best_row;
    shot.col= best_col;
    return shot;
  
}
    
int Nate_Dave::hprob(int r, int c){
    int hBonusPoints = 0;
    if(shot_board[r][c]!=WATER){
        return 0;
    }
    int width = 1;
    if((c-1 >=0 && this->shot_board[r][c-1]==HIT) || (c+1<this->board_size && this->shot_board[r][c+1]==HIT)) hBonusPoints+=40;
    if((c-2 >=0 && this->shot_board[r][c-2]==HIT) || (c+2<this->board_size && this->shot_board[r][c+2]==HIT)) hBonusPoints+=20;
    if((c-3 >=0 && this->shot_board[r][c-3]==HIT) || (c+3<this->board_size && this->shot_board[r][c+3]==HIT)) hBonusPoints+=10;

    if((c-1 >=0 && this->shot_board[r][c-1]==WATER) && (c+1<this->board_size && this->shot_board[r][c+1]==WATER)) hBonusPoints+=5;
    if((c-1 >=0 && this->shot_board[r][c-1]==HIT) && (c+1<this->board_size && this->shot_board[r][c+1]==HIT)) hBonusPoints+=10;


    //subtract points for in-line misses
    if((c-1 >=0 && this->shot_board[r][c-1]==MISS) || (c+1<this->board_size && this->shot_board[r][c+1]==MISS)) hBonusPoints-=5;
    //if((c-2 >=0 && this->shot_board[r][c-2]==MISS) || (c+2<this->board_size && this->shot_board[r][c+2]==MISS)) hBonusPoints-=10;
    char ch;
   
    for(int i = 1; c-i >= 0 && i<=5; i++){
        ch = shot_board[r][c-i];
        if(ch == KILL || ch == MISS){
            break;
        }
        width++;
    }
   
    for(int i = 1; c + i < this->board_size && i <= 5; i++){
        ch = shot_board[r][c+i];
        if(ch == KILL || ch == MISS){
            break;
        }
        width++;
    }
    
    if(width<3) return 0;
    return width+hBonusPoints;
}

    
int Nate_Dave::vprob(int r, int c){
    int vBonusPoints = 0;
    if(shot_board[r][c]!=WATER){
        return 0;
    }
    int height = 1;


    if((r-1 >=0 && this->shot_board[r-1][c]==HIT) || (r+1 < this->board_size && this->shot_board[r+1][c]==HIT)) vBonusPoints+=40;
    if((r-2 >=0 && this->shot_board[r-2][c]==HIT) || (r+2 < this->board_size && this->shot_board[r+2][c]==HIT)) vBonusPoints+=20;
    if((r-3 >=0 && this->shot_board[r-3][c]==HIT) || (r+3 < this->board_size && this->shot_board[r+3][c]==HIT)) vBonusPoints+=10;


    if((r-1 >=0 && this->shot_board[r-1][c]==WATER) && (r+1 < this->board_size && this->shot_board[r+1][c]==WATER)) vBonusPoints+=5;
    if((r-1 >=0 && this->shot_board[r-1][c]==HIT) && (r+1 < this->board_size && this->shot_board[r+1][c]==HIT)) vBonusPoints+=10;
    //subtraction
    if((r-1 >=0 && this->shot_board[r-1][c]==MISS) || (r+1<this->board_size && this->shot_board[r+1][c]==MISS)) vBonusPoints-=5;
    //if((r-2 >=0 && this->shot_board[r-2][c]==MISS) || (r+2<this->board_size && this->shot_board[r+2][c]==MISS)) vBonusPoints-=10;

    char ch;

    for(int i = 1; r-i >= 0 && i<=5; i++){
        ch = shot_board[r-i][c];
        if(ch == KILL || ch == MISS){
            break;
        }
        height++;
    }
    
    for(int i = 1; r+i < this->board_size  && i <= 5; i++){
        ch = shot_board[r+i][c];
        if(ch == KILL || ch == MISS){
            break;
        }
        height++;
    }
    
    if(height<3) return 0;
    return height+vBonusPoints;
}

*/
/*================================================================================
 * This function is called to inform your AI of the result of a previous shot,
 * as well as where the opponent has shot.
 *================================================================================*/
void Nate_Dave::handle_shot_return(PlayerNum player, Shot &shot) {
    // Results of your AI's shot was returned, store it
    if ( player == this->player ) {
        this->shot_board[shot.row][shot.col] = shot.value;
        //this->our_shots[shot.row][shot.col]++;
    }

    // Your AI is informed of where the opponent AI shot, store it
    // NOTE: Opponent shots are stored in ship_board, not shot_board
    else {
        this->ship_board[shot.row][shot.col] = shot.value;
        this->their_shots[shot.row][shot.col]++;
    }
    return;
}

/*================================================================================
 * This function is called to update your shot_board (results of your shots at
 * opponent) when an opponent ship has been killed, OR to update your ship_board
 * (where you keep track of your ships) to show that your ship was killed.
 *================================================================================*/
void Nate_Dave::handle_ship_dead(PlayerNum player, Ship &ship) {
    // store the ship that was killed
    for (int i = 0; i < ship.len; i++) {
        if ( player == this->player ) { // your ship is dead
            if      (ship.dir == HORIZONTAL) this->ship_board[ship.row][ship.col+i] = KILL;
            else if (ship.dir == VERTICAL)   this->ship_board[ship.row+i][ship.col] = KILL;
        } else {             // their ship is dead
            if      (ship.dir == HORIZONTAL) this->shot_board[ship.row][ship.col+i] = KILL;
            else if (ship.dir == VERTICAL)   this->shot_board[ship.row+i][ship.col] = KILL;
        }
    }
    return;
}

/*================================================================================
 * This function runs at the end of a particular game/round.
 * Do anything here that needs to be done at the end of a game/round in the match.
 *================================================================================*/
void Nate_Dave::handle_game_over() {
    return;
}

/*================================================================================
 * This function is called by the AI's destructor and runs at the end of the entire match.
 *================================================================================*/
void Nate_Dave::handle_match_over() {
    delete_boards();
    return;
}

/*================================================================================
 * This function sets up all boards at the beginning of the whole match.
 * Add setup here for any boards you create.
 *================================================================================*/
void Nate_Dave::create_boards() {
    int size = this->board_size;
    // dynamically create an array of pointers.
    this->ship_board = new char*[size];
    this->shot_board = new char*[size];
    this->their_shots = new int*[size];
    //this->our_shots = new int*[size];

    // dynamically allocate memory of size `board_size` for each row.
    for (int i = 0; i < size; i++) {
        this->prob[i] = new int[size];
        this->ship_board[i] = new char[size];
        this->shot_board[i] = new char[size];
        this->their_shots[i] = new int[size];
        for (int j=0;j<size; j++){
            this->prob[i][j] = 0;
            this->their_shots[i][j]=0;
            //this->our_shots[i][j];
        }
        
    }
    return;
}

/*================================================================================
 * This function resets boards between rounds.
 *================================================================================*/
void Nate_Dave::clear_boards() {
    game ++;
    // assign WATER to the boards
    for (int i = 0; i < this->board_size; i++) {
        for (int j = 0; j < this->board_size; j++) {
            this->ship_board[i][j] = WATER;
            this->shot_board[i][j] = WATER;
            this->prob[i][j] = 0;
          //  if (game==50 || game==100 || game ==150 || game==200 || game==250 || game ==300 || game == 350 || game == 400 || game == 450){
              //}  
            this->their_shots[i][j] = 0;
              //this->our_shots[i][j] = 0;
            
        }
    }
    return;
}

/*================================================================================
 * This function is called by the AI's destructor and runs at the end of the entire match.
 *================================================================================*/
void Nate_Dave::delete_boards() {
    // deallocates memory using the delete operator

    for (int i = 0; i < this->board_size; i++) {
        delete[] this->ship_board[i];
        delete[] this->shot_board[i];
        delete[] this->their_shots[i];
        delete[] this->prob[i];
        //delete[] this->our_shots[i];
    }
    delete[] this->ship_board;
    delete[] this->shot_board;
    delete[] this->prob;
    delete[] this->their_shots;
    //delete[] this->our_shots;
    return;
}

