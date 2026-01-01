
const request = require("supertest");
const express = require("express");
const bodyParser = require("body-parser");
const router = require("../routes/question");
const sqlite3 = require("sqlite3").verbose();

// Mock DB for testing
const db = new sqlite3.Database(":memory:");

const app = express();
app.use(bodyParser.json());
app.use("/question", router);

beforeAll((done) => {
  db.serialize(() => {
    db.run(\`
      CREATE TABLE question_data (
        id INTEGER PRIMARY KEY,
        question TEXT,
        preamble TEXT,
        question_data TEXT,
        variating_values TEXT,
        course_codes TEXT
      );
    \`);
    db.run(\`
      CREATE TABLE answer_submissions (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        question_id INTEGER,
        question_data_index INTEGER,
        user_uid TEXT,
        timestamp TEXT,
        user_answer TEXT,
        expected_answer TEXT,
        correct INTEGER
      );
    \`);
    db.run(\`
      INSERT INTO question_data (id, question, question_data, variating_values, course_codes)
      VALUES (?, ?, ?, ?, ?)
    \`, [
      1,
      "Vad heter patienten? %%name%%",
      JSON.stringify([{ question: "Vad heter patienten? %%name%%", answer_unit_id: 0 }]),
      "{}",
      "KM1423"
    ], done);
  });
});

test("POST /question/validate returns correct answer result", async () => {
  const res = await request(app)
    .post("/question/validate")
    .send({
      question_id: 1,
      question_data_index: 0,
      user_answer: "alice",
      user_uid: "test-user",
      timestamp: "2025-05-12T08:37:00Z"
    });

  expect(res.statusCode).toBe(200);
  expect(res.body).toHaveProperty("correct");
  expect(res.body).toHaveProperty("expected");
  expect(res.body).toHaveProperty("accepted_answers");
  expect(res.body).toHaveProperty("user_uid", "test-user");
});
