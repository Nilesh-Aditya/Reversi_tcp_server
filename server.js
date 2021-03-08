const net = require('net');

let clients = [];

let clientObj = {
    1: {},
    2: {}
};

function check_exit(check_data) {
    if (check_data === 'exit' || check_data === 'Exit' || check_data === 'EXIT') {
        return true;
    }
    return false;
}

const server = net.createServer(conn => {

    conn.on('data', data => {
        server.getConnections((err, count)=> console.log(count))
        console.log(conn.localAddress + `:: ${conn.remotePort} :: ${conn.remoteFamily}`);
        
        let check_data1 = data.toString().slice(0, data.length -2);
        let check_data2 = data.toString().slice(0, data.length -1);
        console.log(check_data1);
        
        if (check_exit(check_data1) || check_exit(check_data2)) {
            console.log('Closing all the open clients....');
            for (let i = 0; i < clients.length; i++){
                clients[i].write('exit');
            }
        }
        else if (clients.length > 1) {
            let index = clients.indexOf(conn);
            if (index == 0) {
                conn.emit('socket2', { data });
            }
            else {
                conn.emit('socket1', { data });
            }
        }
        else {
            conn.write('waiting for 2nd connection !!');
        }
        // conn.emit('newline', {data});

    });

    conn.on('socket1', ({ data }) => {
        clients[0].write(data.toString() + '\r');
    })

    conn.on('socket2', ({ data }) => {
        clients[1].write(data.toString() + '\r');
    })

    conn.on('error', (err) => {
        clients.forEach(client => {
            client.destroy();
        });
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
    socket.setEncoding('utf-8');
    clients.push(socket);
    console.log(clients.length);
    
    let len = clients.length;
    console.log(len.toString());
    socket.write(len.toString() + '\r\n');

    clientObj[clients.length] = socket;
})

server.on('error', err => {
    clients.forEach(client => {
        client.destroy();
    });
    console.log(err.message);
})

server.listen({
    host: 'localhost',
    port: 8080,
    exclusive: true
}, () => server.getConnections((err, count)=> console.log(count)));