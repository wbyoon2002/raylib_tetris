const { saveUserFromClient, getSortedRankings } = require("./userStore");

function socketHandler(io) {
  io.on("connection", (socket) => {
    console.log(`새 클라이언트 연결됨: ${socket.id}`);

    socket.on("disconnect", () => {
      console.log(`클라이언트 연결 종료됨: ${socket.id}`);
    });

    socket.on("update_user", async (userDataFromClient) => {
        const savedUser = await saveUserFromClient(userDataFromClient);
    });

    socket.on("get_ranking", async (userDataFromClient) => {
        const ranking = await getSortedRankings(userDataFromClient);
        socket.emit("get_ranking", ranking);
    });

    socket.on("test_server", (data) => {
      const { userId, nickname } = data || {};
      socket.emit("test_client", { message: `hello, ${nickname}` });
    });
  });
}

module.exports = socketHandler;
