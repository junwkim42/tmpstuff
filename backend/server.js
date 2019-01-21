'use strict'

require('dotenv').load();
const debug = require('debug')('backend:server');
const http = require('http');
const cors = require('cors');
const app = require('./config.js');
const Server = http.Server(app);
const PORT = process.env.PORT || 3000;
const io = require('socket.io')(Server);

// Server.use(cors({
//   credentials: true,
//   origin: process.env.CORS_ORIGINS,
// }));

const server = module.exports = Server.listen(PORT, () => {
  debug(`StarWars Backend is running on: ${PORT}`);
});

server.isRunning = true;

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