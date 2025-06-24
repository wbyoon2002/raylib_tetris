const express = require('express');
const http = require('http');
const socketIo = require('socket.io');
const socketHandler = require('./socketHandler');

const app = express();
const server = http.createServer(app);
const io = socketIo(server);

socketHandler(io);

const PORT = process.env.PORT || 3000;
server.listen(PORT, () => {
    console.log(`서버 실행 중 (포트: ${PORT})`);
});