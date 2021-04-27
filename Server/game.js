const EventEmitter = require('events');
const Contract = require('./contract');
const GameLogic = require('./gameLogic');
const clients = require('./players');
const { EMPTY, BLACK, WHITE, TIE } = require('./constants');


class Game extends GameLogic {
    constructor() {
        super();
        this.clients = [];
        this.winner = null;
        this.signal = "";
        this.connections = false;
    }

    addClients(client) {
        this.clients.push(client);
        if (this.clients.length == 2) {
            this.connections = true;
            this.emit('connected');
        }
    }

    sendSignal(signal) {
        this.clients.forEach((client) => {
            client.socket.write(signal + "\r\n");
        });
    }

    makeMove(state, y, x) {
        if (this.isValidIndex(y, x)) {
            // console.log('valid index');
            this.xpos = x;
            this.ypos = y;
            this.turn = state;
            if (this.isValidMove(y, x)) {
                this.board[y][x] = state;
                for (let i = 0; i < 9; i++){
                    if (this.dirs[i]) {
                        let dirY = this.setDirY(i);
                        let dirX = this.setDirX(i);
                        // console.log(dirX, dirY, y, x);
                        this.flipDiscs(y + dirY, x + dirX, dirY, dirX);
                    }

                    // console.log("outside  : " + this.message);
                    if (this.message.length) {
                        // console.log('1  ' + this.message);
                        this.signal += this.message;
                        this.message = "";
                    }
                }
            }
        }
    }

    showBoard() {
        for (let i = 0; i < 8; i++){
            let s = "";
            console.log('========================');
            for (let j = 0; j < 8; j++){
                s += (this.board[i][j] + '||');
            }
            console.log(s);
        }
        console.log('========================');
        console.log(this.signal);
    }

    disconnect() {
        this.clients.forEach((client) => {
            client.socket.destroy();
        });
    }
}

// a little testing

// let game = new Game();
// let y = 5, x = 4;
// game.makeMove(BLACK, y, x);
// game.signal = "";

// game.makeMove(WHITE, y, x - 1);
// game.showBoard();
// game.signal = "";

// game.makeMove(BLACK, y, x - 2);
// game.showBoard();
// game.signal = "";

// game.makeMove(WHITE, y + 1, x - 1);
// game.makeMove(BLACK, y + 1, x);
// game.showBoard();

module.exports = Game;