'use strict';

const express = require('express');
const cors = require('cors');
const app = express();
const io = require('socket.io')(app);
const PORT = process.env.PORT || 8080;

// app.use(cors());
app.use(function(req, res, next) {
  res.header("Access-Control-Allow-Origin", "*");
  res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  next();
});
app.use(express.static(`${__dirname}/dist`));

//listen on every connection
io.on('connection', (socket) => {
	console.log('New user connected');

	//default username
	socket.username = "Anonymous";

    //listen on change_username
    socket.on('change_username', (data) => {
        socket.username = data.username;
    });

    //listen on new_message
    socket.on('new_message', (data) => {
        //broadcast the new message
        console.log(data);
        io.sockets.emit('new_message', {message : data.message, username : socket.username});
    });

    //listen on typing
    socket.on('typing', (data) => {
    	socket.broadcast.emit('typing', {username : socket.username});
    });
});

app.listen(PORT, () => {
  console.log(`listening on port ${PORT}`);
});


// 'use strict';

// const express = require('express');
// const cors = require('cors');
// const app = express();
// const io = require('socket.io')(app);
// const PORT = process.env.PORT || 8080;

// const ClientManager = require('./ClientManager');
// const ChatroomManager = require('./ChatroomManager');
// const makeHandlers = require('./handlers');

// const clientManager = ClientManager();
// const chatroomManager = ChatroomManager();

// // app.use(cors());
// app.use(function(req, res, next) {
//   res.header("Access-Control-Allow-Origin", "*");
//   res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
//   next();
// });
// app.use(express.static(`${__dirname}/dist`));

// io.on('connection', function (client) {
//   const {
//     handleRegister,
//     handleJoin,
//     handleLeave,
//     handleMessage,
//     handleGetChatrooms,
//     handleGetAvailableUsers,
//     handleDisconnect
//   } = makeHandlers(client, clientManager, chatroomManager);

//   console.log('client connected...', client.id);
//   clientManager.addClient(client);

//   client.on('register', handleRegister);

//   client.on('join', handleJoin);

//   client.on('leave', handleLeave);

//   client.on('message', handleMessage);

//   client.on('chatrooms', handleGetChatrooms);

//   client.on('availableUsers', handleGetAvailableUsers)

//   client.on('disconnect', function () {
//     console.log('client disconnect...', client.id);
//     handleDisconnect();
//   })

//   client.on('error', function (err) {
//     console.log('received error from client:', client.id);
//     console.log(err);
//   });
// });

// app.listen(PORT, () => {
//   console.log(`listening on port ${PORT}`);
// });

// module.exports = io;

// 'use strict'
// const http = require('http');
// const app = require('./config');
// const Server = http.Server(app);
// const PORT = process.env.PORT || 8080;
// const io = require('socket.io')(Server);

// Server.listen(PORT, () => console.log('Game server running on:', PORT));

// const players = {};

// io.on('connection', socket => {
//   // When a player connects
//   console.log("conneection");
//   socket.on('new-player', state => {
//     console.log('New player joined with state:', state);
//     players[socket.id] = state;
//     // Emit the update-players method in the client side
//     io.emit('update-players', players);
//   });

//   socket.on('disconnect', state => {
//     delete players[socket.id];
//     io.emit('update-players', players);
//   });

//   // When a player moves
//   socket.on('move-player', data => {
//     const { x, y, angle, playerName, speed } = data;

//     // If the player is invalid, return
//     if (players[socket.id] === undefined) {
//       return ;
//     }

//     // Update the player's data if he moved
//     players[socket.id].x = x;
//     players[socket.id].y = y;
//     players[socket.id].angle = angle;
//     players[socket.id].playerName = {
//       name: playerName.name,
//       x: playerName.x,
//       y: playerName.y
//     };
//     players[socket.id].speed = {
//       value: speed.value,
//       x: speed.x,
//       y: speed.y
//     };

//     // Send the data back to the client
//     io.emit('update-players', players);
//   })
// });