const net = require('net');

let clients = [];

const server = net.createServer(conn => {

    conn.on('data', data => {
        server.getConnections((err, count)=> console.log(count))
        console.log(conn.localAddress + `:: ${conn.remotePort} :: ${conn.remoteFamily}`);
        conn.write('hello' + '\r\n');
        // setTimeout(() => {
        //     console.log('timeout');
        // }, 3000);
        conn.emit('newline', {data});
    });

    conn.on('newline', ({ data }) => {
        clients[0].write('$ Client 1 : ' + data + '\r\n');
        // clients[1].write('$ Client 2 : ' + data + '\r\n');
        console.log(clients.length);
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
})

server.on('error', err => {
    console.log(err.message);
})

server.listen({
    host: 'localhost',
    port: 8080,
    exclusive: true
}, () => server.getConnections((err, count)=> console.log(count)));

// const server2 = net.createServer(conn => {
//     console.log('new client');
    
//     conn.on('data', data => {
//         console.log(data.toString());
//         console.log(server2.listeners());
//         console.log(conn.listenerCount());
//         conn.write('hello' + '\r\n');
//     });

//     conn.on('end', () => {
//         server2.close((err) => {
//             if (err) {
//                 console.log(err.message);
//             }
//             console.log('server2 closed');
//         })
//         console.log('client left');
//     });
// });

// server2.listen({
//     host: 'localhost',
//     port: 9090,
//     exclusive: true
// });