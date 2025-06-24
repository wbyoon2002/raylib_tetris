const fs = require('fs').promises;
const USERS_FILE_PATH = './users.json';


async function readUsersData() {
  try {
    const data = await fs.readFile(USERS_FILE_PATH, 'utf8');
    return JSON.parse(data);
  } catch (error) {
    if (error.code === 'ENOENT') {
      console.log(`${USERS_FILE_PATH} 파일이 없음, 새로 생성합니다.`);
      await fs.writeFile(USERS_FILE_PATH, '{}', 'utf8');
      return {};
    }
    console.error('유저 데이터를 읽는 중 오류 발생:', error);
    return {};
  }
}

async function writeUsersData(usersMap) {
  try {
    await fs.writeFile(USERS_FILE_PATH, JSON.stringify(usersMap, null, 2), 'utf8');
  } catch (error) {
    console.error('유저 데이터를 저장하는 중 오류 발생:', error);
  }
}

async function saveUserFromClient(userData) {
  const users = await readUsersData();
  const userObj = JSON.parse(userData);
  const { uuid } = userObj;

  users[uuid] = userObj;
  await writeUsersData(users);
  return users[uuid];
}

async function getSortedRankings() {
  const users = await readUsersData();

  const difficulties = ['Easy', 'Medium', 'Hard', 'Extreme'];

  const marathon = {};
  for (const diff of difficulties) {
    marathon[diff.toLowerCase()] = [];
  }

  const sprint = [];

  for (const uuid in users) {
    const user = users[uuid];

    for (const diff of difficulties) {
      const score = user.scores.marathon?.[diff];
      if (typeof score === 'number') {
        marathon[diff.toLowerCase()].push({ nickname: user.nickname, uuid:user.uuid, score });
      }
    }

    const time = user.scores.sprint;
    if (typeof time === 'number' && time > 0) {
      sprint.push({ nickname: user.nickname, uuid:user.uuid, time });
    }
  }

  for (const diff of difficulties) {
    marathon[diff.toLowerCase()].sort((a, b) => b.score - a.score);
  }
  sprint.sort((a, b) => a.time - b.time);

  const result = {
    marathon,
    sprint,
  };

  return JSON.stringify(result);
}

module.exports = {
  readUsersData,
  writeUsersData,
  saveUserFromClient,
  getSortedRankings
};
