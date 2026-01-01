const sqlite3 = require("sqlite3").verbose();
const path = require("path");

const dbPath = path.resolve(__dirname, "question_data.db");
const db = new sqlite3.Database(dbPath, (err) => {
  if (err) {
    console.error("Failed to connect to the database:", err.message);
    process.exit(1);
  } else {
    console.log("Connected to the SQLite database.");
  }
});

function getRandomDate(i, totalRecords) {
  const daysAgo = Math.floor((1 - i / totalRecords) * 90);
  const date = new Date();
  date.setDate(date.getDate() - daysAgo);
  date.setHours(8 + Math.floor(Math.random() * 14));
  date.setMinutes(Math.floor(Math.random() * 60));
  date.setSeconds(Math.floor(Math.random() * 60));
  return date.toISOString().replace("T", " ").split('.')[0];
}

function getRandomCorrect(i, totalRecords) {
  const chance = 0.4 + 0.5 * (i / totalRecords);
  return Math.random() < chance ? 1 : -1;
}

function isWeekend(dateString) {
  const day = new Date(dateString).getDay();
  return day === 0 || day === 6;
}

db.all("SELECT id, course_codes FROM question_data", (err, rows) => {
  if (err) {
    console.error("Error fetching question data:", err.message);
    db.close();
    process.exit(1);
  }

  if (!rows.length) {
    console.log("No question data found. Aborting.");
    db.close();
    return;
  }

  const totalRecords = 1000;

  db.serialize(() => {
    const insertStmt = db.prepare(`
      INSERT INTO answer_history (question_id, correct, date, course_code)
      VALUES (?, ?, ?, ?)
    `);

    let inserted = 0;
    let i = 0;

    while (inserted < totalRecords && i < totalRecords * 2) {
      const randomDate = getRandomDate(i, totalRecords);
      if (isWeekend(randomDate) && Math.random() < 0.3) {
        i++;
        continue;
      }

      const question = rows[Math.floor(Math.random() * rows.length)];
      const questionId = question.id;

      let courseCodes = [];
      try {
        courseCodes = JSON.parse(question.course_codes);
        if (!Array.isArray(courseCodes) || courseCodes.length === 0) throw new Error();
      } catch {
        console.warn(`⚠️ Invalid or missing course_codes for question ID ${questionId}. Skipping.`);
        i++;
        continue;
      }

      const randomCourseCode = courseCodes[Math.floor(Math.random() * courseCodes.length)];
      const correctValue = getRandomCorrect(i, totalRecords);

      insertStmt.run(questionId, correctValue, randomDate, randomCourseCode, (err) => {
        if (err) console.error("Insert error:", err.message);
      });

      inserted++;
      i++;
    }

    insertStmt.finalize((err) => {
      if (err) {
        console.error("Finalize error:", err.message);
      } else {
        console.log(`✅ Inserted ${inserted} dummy records into answer_history.`);
      }
      db.close();
    });
  });
});
