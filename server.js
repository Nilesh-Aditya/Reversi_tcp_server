const net = require('net');

let clients = [];

let clientObj = {
    1: {},
    2: {}
};

function check_exit(check_data) {
    if (check_data.includes('exit') || check_data.includes('Exit') || check_data.includes('EXIT')) {
        return true;
    }
    return false;
}

const server = net.createServer(conn => {

    conn.on('data', data => {
        server.getConnections((err, count)=> console.log(count))
        console.log(conn.localAddress + `:: ${conn.remotePort} :: ${conn.remoteFamily}`);

        let checkData = data.toString();
        // console.log(check_data);
        
        if (check_exit(checkData)) {
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
        console.log("socket 1 : " ,data);
        clients[0].write(data.toString() + '\r\n');
    })

    conn.on('socket2', ({ data }) => {
        console.log("socket 2 : " ,data);
        clients[1].write(data.toString() + '\r\n');
    })

    conn.on('error', (err) => {
        clients.forEach(client => {
            client.end();
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
        client.end();
    });
    console.log(err.message);
})

server.listen({
    host: 'localhost',
    port: 8080,
    exclusive: true
}, () => server.getConnections((err, count)=> console.log(count)));