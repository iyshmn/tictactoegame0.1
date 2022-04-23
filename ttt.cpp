#include <iostream>
using namespace std;
class Board{
    public:
        Board();
        bool isFull() const;
        bool isLegalMove(int move) const;
        bool isWinner(char piece) const;
        void dislpay() const;
        void reset();
        void receiveMove(char piece, int move);
        static const int NUM_SQUARES = 9;
        static const char EMPTY = ' ';
    private:
        static const int NUM_COMBOS=8;
        static const int NUM_IN_COMBO=3;
        static const int WINNING_COMBOS[NUM_COMBOS][NUM_IN_COMBO];
        char m_Sqaures[NUM_SQUARES];
};

class Player{
        public:
        Player();
        virtual ~Player();
        char GetPeice() const;
        char getOpponentPiece() const;
        virtual void makeMove(Board &aBoard) const = 0;
        private:
        static const int NUM_PIECES =2;
        static const int FIRST = 0;
        static const int SECOND = 1;
        static const char PIECES[NUM_PIECES];
        static int currrent;
        char m_Piece;
};
const char Player::PIECES[NUM_PIECES] = {'X','0'};
int Player:: currrent = 0;
Player::Player(){
    this->m_Piece=this->PIECES[this->currrent];
    this->currrent=(this->currrent+1)%this->NUM_PIECES;
}
Player::~Player(){}
char Player::GetPeice() const{
    return this->m_Piece;
}
// void Player::MakeMove(Board& aBoard) const{
//     int move;
//     do{
//         cout << "Player " << this->GetPeice();
//         cout << ", where would you like to move?(0-8): ";
//         cin >> move;
//     }while(!aBoard.isLegalMove(move));
//     aBoard.receiveMove(GetPeice(),move);
// }
char Player::getOpponentPiece() const{
    char piece;
    if(m_Piece==PIECES[FIRST]){
        piece = PIECES[SECOND];
    }else{
        piece = PIECES[FIRST];
    }
    return piece;
}
class PlayerHuman : public Player{
    public:
    virtual void makeMove(Board &aBoard) const;
};
void PlayerHuman::makeMove(Board &aBoard) const{
     int move;
    do{
        cout << "Player " << this->GetPeice();
        cout << ", where would you like to move?(0-8): ";
        cin >> move;
    }while(!aBoard.isLegalMove(move));
    aBoard.receiveMove(GetPeice(),move);
}
class PlayerComputer : public Player{
    virtual void makeMove(Board &aBoard) const;
};
void PlayerComputer::makeMove(Board &aBoard) const{
     int move=0;
    bool found=false;
    while(!found && move < aBoard.NUM_SQUARES){
        if(aBoard.isLegalMove(move)){
        aBoard.receiveMove(this->GetPeice(),move);
        found = aBoard.isWinner(this->GetPeice());
        aBoard.receiveMove(aBoard.EMPTY,move);
        }
        if(!found){
        ++move;
        }
    }
    if(!found){
        move=0;
        while(!found && move < aBoard.NUM_SQUARES){
            if(aBoard.isLegalMove(move)){
                aBoard.receiveMove(this->getOpponentPiece(),move);
                found = aBoard.isWinner(this->getOpponentPiece());
                aBoard.receiveMove(aBoard.EMPTY,move);
            }
            if(!found){
                ++move;
            }
        }
    }
    if(!found){
        move=0;
        int i=0;
        const int BEST_MOVES[]={4,0,2,6,8,1,3,5,7};
        while(!found && i < aBoard.NUM_SQUARES){
            move=BEST_MOVES[i];
            if(aBoard.isLegalMove(move)){
                found=true;
            }
            ++i;
        }
    }
    cout << "I, player " << GetPeice() << ",shall take square number " << move << "." << endl;
     aBoard.receiveMove(GetPeice(),move);
}
const int Board::WINNING_COMBOS[NUM_COMBOS][NUM_IN_COMBO]={
    {0,1,2},
    {3,4,5},
    {6,7,8},
    {0,3,6},
    {1,4,7},
    {2,5,8},
    {0,4,8},
    {2,4,6}
};
Board::Board(){
    this->reset();
}
bool Board::isFull() const{
    bool full = true;
    int i=0;
    while(full && i<this->NUM_SQUARES){
        if(this->m_Sqaures[i]==this->EMPTY){
            full=false;
        }
        ++i;
    }
    return full;
}
bool Board::isLegalMove(int move) const{
    return (move>=0 && move < this->NUM_SQUARES && this->m_Sqaures[move]==this->EMPTY);
}
bool Board::isWinner(char piece) const{
    bool winner = false;
    int i=0;
    while(!winner && i < this->NUM_COMBOS){
        int piecesInCombo = 0;
        for(int j=0; j<NUM_IN_COMBO;++j){
            if(this->m_Sqaures[this->WINNING_COMBOS[i][j]]==piece){
                ++piecesInCombo;
            }
        }
        if(piecesInCombo==NUM_IN_COMBO){
            winner=true;
        }
        ++i;
    }
    return winner;
}
void Board::dislpay() const{
    cout << endl << "\t" << this->m_Sqaures[0] << " | " << this->m_Sqaures[1];
    cout << " | " << this->m_Sqaures[2];
    cout << endl << "\t" << "----------";
    cout << endl << "\t" << this->m_Sqaures[3] << " | " << this->m_Sqaures[4];
    cout << " | " << this->m_Sqaures[5];
    cout << endl << "\t" << "----------";
    cout << endl << "\t" << this->m_Sqaures[6] << " | " << this->m_Sqaures[7];
    cout << " | " << this->m_Sqaures[8];
    cout << endl << endl;
}
void Board::reset(){
    for(int i=0;i<NUM_SQUARES;++i){
        this->m_Sqaures[i]=this->EMPTY;
    }
}
void Board::receiveMove(char piece, int move){
    this->m_Sqaures[move] = piece;
}
class Game{
    public:
        Game();
        ~Game();
        bool isPlaying() const;
        bool isTie() const;
        void displayInstructions() const;
        void nextPlayer();
        void clearPlayers();
        void setPlayers();
        void announceWinner() const;
        void Play();
    private:
        static const int NUM_PLAYERS=2;
        static const int FIRST=0;
        static const int SECOND=1;
        Board mBoard;
        //Player mPlayer[NUM_PLAYERS];
        Player* m_pPlayers[NUM_PLAYERS];
        int m_Current;
};
Game::Game():m_Current(this->FIRST){
    for(int i=0;i<NUM_PLAYERS;i++){
        this->m_pPlayers[i]=NULL;
    }
}
Game::~Game(){
    clearPlayers();
}
void Game::clearPlayers(){
    for(int i=0;i<NUM_PLAYERS;i++){
        delete this->m_pPlayers[i];
        this->m_pPlayers[i]=NULL;
    }
}
void Game::setPlayers(){
    clearPlayers();
    cout << "Who shall wage this epic fight ?" << "declare the opponents ... " << endl;
    for(int i=0;i<NUM_PLAYERS;i++){
        cout << "Player " << i+1  << "- human or computer (h/c)";
        char playerType;
        cin >> playerType;
        if(playerType=='h'){
            this->m_pPlayers[i] = new PlayerHuman();
        }else{
            this->m_pPlayers[i] = new PlayerComputer();        
    }
}
}
bool Game::isPlaying() const{
    return(
        !this->mBoard.isFull() && !this->mBoard.isWinner(this->m_pPlayers[FIRST]->GetPeice())&&
        !this->mBoard.isWinner(m_pPlayers[SECOND]->GetPeice())
    );
}
bool Game::isTie() const{
    return(
        mBoard.isFull() && ! mBoard.isWinner(m_pPlayers[FIRST]->GetPeice()) && !mBoard.isWinner(m_pPlayers[SECOND]->GetPeice())
    );
}
void Game::displayInstructions() const{
    cout << "In progress" << endl;
}
void Game::nextPlayer(){
    m_Current=(m_Current+1)%this->NUM_PLAYERS;
}
void Game::announceWinner() const{
    cout << "the raging battle has to a final end";
    cout << endl;
    if(this->isTie()){
        cout << "Sadly, no player emerged victarious" << endl;
    
    }else{
        cout << "the winner of this climatic confrontation is player :" ;
        if(mBoard.isWinner(m_pPlayers[FIRST]->GetPeice())){
            cout << m_pPlayers[FIRST]->GetPeice() << "!";
            cout << endl;
        }else{
            cout << m_pPlayers[SECOND]->GetPeice() << endl;
            cout << endl;
        }
    }
}
void Game::Play() {
    this->m_Current=FIRST;
    mBoard.reset();
    while(isPlaying()){
        mBoard.dislpay();
        m_pPlayers[m_Current]->makeMove(mBoard);
        nextPlayer();
        
    }
    mBoard.dislpay();
    announceWinner();
}
int main(){
    Game tictactoe;
    tictactoe.displayInstructions();
    char again;
    do{
        tictactoe.setPlayers();
        tictactoe.Play();
        cout << endl << "play again?(y/n): " ;
        cin >> again;
    }while (again!='n');
    return 0;
    
}