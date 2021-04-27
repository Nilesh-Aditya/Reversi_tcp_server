const EventEmitter = require('events');
const { EMPTY, BLACK, WHITE, TIE } = require('./constants');

class GameLogic extends EventEmitter {
    constructor() {
        super();
        this.message = "";
        this.turn = EMPTY;
        this.breadth = 8;
        this.length = 8;
        this.board = makeEmptyboard();
        this.xpos = -1;
        this.ypos = -1;
        this.dirs = [...new Array(9)].map(e => false);
        this.whiteDisks = 2;
        this.blackDisks = 2;
    }

    isValidIndex(y, x) {
        if (y < 0 || x < 0 || y >= this.breadth || x >= this.length) {
            console.log(y, x);
            return false;
        }
        return true;
    }

    isNeighbour(y, x) {
        for (let j = -1; j <= 1; j++){
            for (let i = -1; i <= 1; i++){
                if (i == 0 && j == 0) {
                    continue;
                }
                if (!this.isValidIndex(y + j, x + i)) {
                    continue;
                }
                if (this.board[y + j][x + i] == EMPTY) {
                    return true;
                }
            }
        }

        return false;
    }

    isValidMove(y, x) {
        if (this.board[y][x] != EMPTY) {
            return false;
        }
        if (!this.isNeighbour(y, x)) {
            return false;
        }
        let count = 0;
        let dirsCopy = [...new Array(9)].map(e => false);

        for (let j = -1; j <= 1; ++j){
            for (let i = -1; i <= 1; ++i) {
                if (j == 0 && i == 0) {
                    dirsCopy[count++] = false;
                    continue;
                }

                if (this.isValidIndex(y + j, x + i)) {
                    if (this.board[y + j][x + i] == EMPTY) {
                        dirsCopy[count] = false;
                    }
                    else if (this.board[y + j][x + i] == this.turn) {
                        dirsCopy[count] = false;
                    }
                    else {
                        dirsCopy[count] = true;
                    }
                }
                else {
                    dirsCopy[count] = false;
                }
                // if (dirsCopy[count]) {
                //     console.log(j, i, count);
                // }
                ++count;
            }
        }
        let s = "";
        for (let i = 0; i < 9; i++){
            s += dirsCopy[i].toString() + " ";
        }
        console.log(s);

        let searchDirX = -1, searchDirY = -1;
        let flag = false;
        
        for (let k = 0; k < 9; k++){
            if (dirsCopy[k]) {
                searchDirX = this.setDirX(k);
                searchDirY = this.setDirY(k);

                if (this.noOfFlippedDiscs(y + searchDirY, x + searchDirX, searchDirY, searchDirX, 0) > 0) {
                    flag = true;
                }
            }
        }

        if (this.isAtCurrPos(y, x)) {
            for (let i = 0; i < 9; i++){
                this.dirs[i] = dirsCopy[i];
            }
        }
        return flag;
    }

    flipDiscs(currY, currX, dirY, dirX) {
        let arr = [];
        // console.log(currY, currX);
        // console.log(this.board[5][4] == this.turn);

        if (!this.isValidIndex(currY, currX)) {
            console.log('hi');
            return false;
        }
        if (this.board[currY][currX] == this.turn) {
            console.log('hello');
            return true;
        }

        let col = this.turn == BLACK ? WHITE : BLACK;
        
        if (this.board[currY][currX] == col) {
            this.board[currY][currX] = this.turn;
            arr.push([currY, currX, this.board[currY][currX]]);
            this.message += currY.toString() + currX.toString() + this.turn + ' ';
            // console.log(this.message);
        }

        if (this.turn == WHITE) {
            ++this.whiteDisks;
            --this.blackDisks;
        }
        else if (this.turn == BLACK) {
            ++this.blackDisks;
            --this.whiteDisks;
        }

        let res = this.flipDiscs(currY + dirY, currX + dirX, dirY, dirX);
        if (!res) {
            this.board[currY][currX] = EMPTY;
            for (let i = 0; i < arr.length; i++){
                this.board[arr[i][0]][arr[i][1]] = arr[i][2];
            }
            this.message = "";
        }

        // console.log(this.message, res);

        return res;
    }

    setDirY(k) {
        switch (k)
        {
        case 0:
        case 1:
        case 2:
            return -1;
        case 3:
        case 5:
            return 0;
        case 6:
        case 7:
        case 8:
            return +1;
        }
        return 0;
    }

    setDirX(k) {
        switch (k)
        {
        case 0:
        case 3:
        case 6:
            return -1;
        case 1:
        case 7:
            return 0;
        case 2:
        case 5:
        case 8:
            return +1;
        }
        return 0;
    }

    isAtCurrPos(j, i) {
        return this.xpos == i && this.ypos == j;
    }

    noOfFlippedDiscs(currY, currX, dirY, dirX, flippedDiscs = 0) {
        if (!this.isValidIndex(currY, currX)) {
            return 0;
        }
        if (this.board[currY][currX] == EMPTY) {
            return 0;
        }
        if (this.board[currY][currX] == this.turn) {
            return flippedDiscs;
        }
        else {
            return this.noOfFlippedDiscs(currY + dirY, currX + dirX, dirY, dirX, flippedDiscs + 1);
        }
    }

    flip(state) {
        if (state == EMPTY) {
            return state;
        }
        if (state == BLACK) {
            // state = WHITE;
            return WHITE;
        }
        if (state == WHITE) {
            // state = BLACK;
            return BLACK;
        }
    }
}


function makeEmptyboard() {
    let arr = [];

    for (let i = 0; i < 8; i++){
        arr.push([]);
        for (let j = 0; j < 8; j++){
            arr[i].push(EMPTY);
        }
    }

    arr[3][3] = WHITE;
    arr[4][4] = WHITE;
    arr[3][4] = BLACK;
    arr[4][3] = BLACK;

    return arr;
}

// console.log(new GameLogic().setDirY(6));

module.exports = GameLogic;