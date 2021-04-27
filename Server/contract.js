class contract {
    constructor() {
        this.gameStateChange = {
            gameUnchanged: 0,
            gameStarted: 1,
            gameEnded: 2
        };

        this.gameChanges = {
            gameWin: 0,
            gameTie: 1
        };

        this.playerTurns = {
            B: 0,
            W: 1
        };
    }
}

module.exports = Contract = new contract();