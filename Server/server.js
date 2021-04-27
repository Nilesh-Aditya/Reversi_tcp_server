const net = require('net');
const clients = require('./players');
const Game = require('./game');
const Contract = require('./contract');
const { EMPTY, BLACK, WHITE, TIE } = require('./constants');
const { sign } = require('crypto');

let cnt = 0;

let game = new Game();

function check_exit(check_data) {
    if (check_data.includes('exit') || check_data.includes('Exit') || check_data.includes('EXIT')) {
        return true;
    }
    return false;
}

const server = net.createServer(conn => {

    conn.on('data', data => {
        console.log(data);
        if (game.connections) {
            console.log('inside');
            let y = data[0] - '0';
            let x = data[1] - '0';
            let color = EMPTY;
            if (cnt % 2 == 0) {
                game.makeMove(BLACK, y, x);
                color = BLACK;
            }
            else {
                game.makeMove(WHITE, y, x);
                color = WHITE;
            }
            cnt++;
    
            if (game.signal) {
                console.log(game.signal);
                let signal = y.toString() + x.toString() + color + ' ' + game.signal;
                game.signal = "";
                game.sendSignal(signal);
            }
            else {
                game.sendSignal("Exit");
            }
            
        }
        // if (check_exit(data)) {
        //     game.sendSignal("exit");
        //     game.emit('diconnect');
        // }
    });

    conn.on('error', (err) => {
        game.emit('disconnect');
    })

    conn.once('end', () => {
        server.close((err) => {
            if (err) {
                console.log(err.message);
            }
            console.log('server closed');
        })
        console.log('client left');
    });
});

server.on('connection', (socket) => {
    console.log(`$client ${socket.remoteAddress} : ${socket.remotePort} has just entered the chat`);
    game.addClients(new clients(socket));
    socket.setEncoding('utf-8');
    // socket.pause();
    socket.write(game.clients.length.toString() + '\r\n');
});


game.on('connected', () => {
    // game.clients.forEach(client => {
    //     client.socket.resume();
    // });
    console.log('connected');
});

game.on('disconnect', () => {
    game.disconnect();
});

server.listen({
    host: 'localhost',
    port: 8080,
    exclusive: true
}, () => server.getConnections((err, count)=> console.log(count)));