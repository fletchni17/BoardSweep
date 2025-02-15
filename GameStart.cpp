//
// Created by nflet on 12/9/2022.
//

#include "GameStart.h"
#include <math.h>
#include <iostream>
using namespace std;


Board*  initializeBoardFromFen(const char fen[]){
    //"rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"

    Board  * board = new Board;

    for(int i = 0; i<14;i++){
        board->bitboards[i] =0;
    }
    unsigned int currentSquare = 56;
    unsigned int charCount =0;
    unsigned int numSquaresVisited =0;


    //load the squares
    while(numSquaresVisited <64){

        char occupancy = fen[charCount];


        if(occupancy == '/'){
            currentSquare-=16;
        }
        else{
            //ascii value of 9 is 57
            if(occupancy <57){
                currentSquare+= (occupancy - '0');
                numSquaresVisited +=(occupancy - '0');

            }
            else{
                board->bitboards[occupancy] |= 1ULL << currentSquare;
                currentSquare++;
                numSquaresVisited++;
            }
        }

        charCount++;

    }
    board->bitboards[allWhite] = board->bitboards[P] | board->bitboards[B] | board->bitboards[N] |
            board->bitboards[R] | board->bitboards[Q] | board->bitboards[K];

    board->bitboards[allBlack] = board->bitboards[p] | board->bitboards[b] | board->bitboards[n] |
                                 board->bitboards[r] | board->bitboards[q] | board->bitboards[k];



    charCount++;
    if(fen[charCount] == 'w'){
        board->sideToMove = white;
    }
    else board->sideToMove = black;
    charCount+=2;

    while(!(fen[charCount] == ' ')){
        char castleChar = fen[charCount];
        switch(castleChar){

            case 'K':
                board->castleRights |= 1L <<3;
                break;
            case 'Q':
                board->castleRights |= 1L<<2;
                break;
            case 'k':
                board->castleRights |= 1L <<1;
                break;
            case 'q':
                board->castleRights |= 1L;
                break;
            default:
                break;
        }
        charCount++;
    }
    charCount++;

    if(fen[charCount] == '-'){
        board->enPassSquare = 64;
        charCount++;
    }
    else{
        char file = fen[charCount];
        charCount++;
        char rank = fen[charCount];

        board->enPassSquare = ((rank - '0')*8)+ (file - '`'-9);
        charCount++;

    }


    charCount++;

    int decimalCountHalf =0;
    int halfMoveCount =0;
    while(!(fen[charCount] == ' ')){
        decimalCountHalf++;
        charCount++;
    }

    charCount-=decimalCountHalf;


    for(int i =decimalCountHalf-1; i>=0;i--){

        //cout << pow(10,i)* (fen[charCount]-'0')<<endl;
        halfMoveCount += pow(10,i)* (fen[charCount]-'0');
        charCount++;
    }

    board->halfMoveCount = halfMoveCount;
    charCount++;

    int decimalCountFull =0;
    int fullMoveCount =0;
    while(!(fen[charCount] == '\0')){
        decimalCountFull++;
        charCount++;
    }

    charCount-=decimalCountFull;


    for(int i =decimalCountFull-1; i >=0;i--){

        fullMoveCount += pow(10,i)* (fen[charCount]-'0');
        charCount++;
    }

    board->fullMoveCount = fullMoveCount;


    return board;

}