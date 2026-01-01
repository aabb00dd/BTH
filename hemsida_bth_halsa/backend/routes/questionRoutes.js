// backend/routes/questionRoutes.js
const express = require("express");
const router = express.Router();

const sqlite3 = require("sqlite3").verbose();
const path = require("path");
const dbPath = path.resolve(__dirname, "../db/question_data.db");
const db = new sqlite3.Database(dbPath, (err) => {
    if (err) {
        console.error("Error connecting to DB:", err.message);
    } else {
        console.log("Connected to SQLite DB in questionRoutes.");
    }
});

const { addRecord, getTable, getRecordWhere, getRecordById } = require("../models/questionModel");
const { isValidJsonArray, isPositiveInteger, getRandomElement } = require("../helpers/routeHelpers");
const { generateValues, formatQuestionText, checkAnswer, calculateAnswer } = require("../helpers/questionHelpers");

// ----------------------------- Add Entry -----------------------------
//
// POST /question/add
router.post("/add", async (req, res) => {
    try {
        let { question, answer_unit_id, answer_formula, variating_values, course_codes, question_type_id, hints } = req.body;

        // Validate required fields
        if (!question || !answer_formula || !variating_values || !course_codes || !answer_unit_id || !question_type_id) {
            return res.status(400).json({ success: false, message: "Missing required fields" });
        }

        if (!Array.isArray(course_codes) || course_codes.length === 0) {
            return res.status(400).json({ success: false, message: "course_codes must be a non-empty array." });
        }
        // Convert all course codes to uppercase
        course_codes = course_codes.map(code => code.toUpperCase());

        if (!isPositiveInteger(answer_unit_id) || !isPositiveInteger(question_type_id)) {
            return res.status(400).json({ success: false, message: "Invalid answer_unit_id or question_type_id format. Must be positive integers." });
        }

        if (!isValidJsonArray(variating_values)) {
            return res.status(400).json({ success: false, message: "variating_values must be a valid JSON array." });
        }

        // Use provided hints or default to an empty array
        if (!hints) {
            hints = [];
        }

        try {
            const result = await addRecord(
                "question_data",
                ["question", "answer_unit_id", "answer_formula", "variating_values", "course_codes", "question_type_id", "hints"],
                [question, answer_unit_id, answer_formula, variating_values, course_codes, question_type_id, JSON.stringify(hints)]
            );
            res.status(201).json({ success: true, message: "Question successfully added", id: result.id });
        } catch (err) {
            if (err.message.includes("FOREIGN KEY constraint failed")) {
                return res.status(400).json({
                    success: false,
                    message: "Invalid foreign key reference. Ensure that referenced data exists."
                });
            }
            throw err;
        }
    } catch (err) {
        res.status(err.status || 500).json({
            success: false,
            message: err.message || "Error adding question"
        });
    }
});

// ----------------------------- Get Question -----------------------------
//
// GET /question/random
// Expects a query parameter: course_code
router.get("/random", async (req, res) => {
  try {
    const { course_code, count = 1 } = req.query;
    const questionCount = parseInt(count);
    // ---- Validate input parameters ----
    if (!course_code) {
      return res.status(400).json({ 
        success: false, 
        message: "A course code must be provided." 
      });
    }

    // Fetch number of questions for the course
    const course_row = await new Promise((resolve, reject) => {
      db.get(
        `SELECT num_questions FROM course WHERE course_code = ?`,
        [course_code.toUpperCase()],
        (err, row) => {
          if (err) return reject(err);
          resolve(row);
        }
      );
    });

    if (!course_row) {
      return res.status(404).json({
        success: false,
        message: `Course with code "${upperCourseCode}" not found.`
      });
    }

    if (questionCount < 1 || questionCount > course_row.num_questions) {
      return res.status(400).json({ 
        success: false,
        message: `Count mus be between 1 and ${course_row.num_questions} for ${course_code}.`
      });
    }

    // ---- Fetch random questions ----
    const sql = `
      SELECT *
      FROM question_data
      WHERE EXISTS (
        SELECT 1 FROM json_each(question_data.course_codes)
        WHERE json_each.value = ?
      )
      ORDER BY RANDOM()
      LIMIT ?;
    `;

    const getQuestions = () => {
      return new Promise((resolve, reject) => {
        db.all(sql, [course_code.toUpperCase(), questionCount], (err, questions) => {
          if (err) {
            reject({ status: 500, message: "Database error", error: err.message });
            return;
          }
          if (!questions || questions.length === 0) {
            reject({ status: 404, message: "No questions found." });
            return;
          }
          resolve(questions);
        });
      });
    };

    // Fetch questions from the database
    const questions = await getQuestions();


    const rendered = await Promise.all(
      questions.map(async (q) => {
        // 1) generateValues does parsing, DB‐fetch, placeholder replacement, formula eval
        const { question_text, generatedValues, medicineInfo } = await generateValues(q);
        // console.log("Generated Question text:", question_text);
        // console.log("Generated values:", generatedValues);
        // console.log("Medicine info:", medicineInfo);

        const correctAnswer = calculateAnswer(q.answer_formula, generatedValues);

        // 2) format the final question text
        const renderedText = formatQuestionText(question_text, generatedValues);

        // 3) lookup the answer unit record
        const unitRecord = await getRecordById("units", q.answer_unit_id);

        // 4) parse the JSON columns
        const parsedHints = JSON.parse(q.hints || "[]");

        const resultObj = {
            id:             q.id,
            question:       renderedText,
            generated_values: generatedValues,
            answer_formula: q.answer_formula,
            answer_units:   unitRecord,
            hints:          parsedHints,
            medicine_link:  medicineInfo,
            computed_answer: correctAnswer
            };
        console.log("Question (", q.id, "): " , renderedText, "\n");
        return resultObj;
      })
    );
    console.log("\n\n-----New Questions-----\n\n");
    return res.json({ success: true, data: rendered });
  } catch (err) {
    console.error("Error in /random:", err);
    return res.status(500).json({ success: false, message: "Server error." });
  }
});




// ----------------------------- Check Answer -----------------------------
//
// POST /check-answer
router.post("/check-answer", async (req, res) => {
    const { question_id, answer, correctAnswer, correctUnit, formula, course_code } = req.body;
    if (!question_id || !answer || !correctAnswer || !correctUnit || !formula || !course_code) {
        return res.status(400).json({ correct: false, message: "Missing required fields." });
    }

    const result = checkAnswer(answer, correctAnswer, correctUnit, formula);

    // Log the answer into answer_history (1 for correct, -1 for wrong) along with the selected course.
    try {
        await addRecord("answer_history", ["question_id", "correct", "course_code"], [question_id, (result.correct ? 1 : -1), course_code]);
    } catch (err) {
        console.error("Failed to insert answer record:", err);
        // We won't fail the endpoint because of logging issues.
    }

    if (!result.correct && result.message_type === "Fel accepted_answers format.") {
        return res.status(500).json(result);
    }
    
    res.json(result);
});

// GET /question/all
router.get("/all", async (req, res) => {
    try {
      const questions = await getTable("question_data");
      res.status(200).json({ success: true, records: questions });
    } catch (err) {
      res.status(500).json({ success: false, message: "Error retrieving questions" });
    }
});

// GET /question/counts
router.get("/counts", (req, res) => {
    const sql = `
        SELECT c.course_code, COUNT(DISTINCT qd.id) as count
        FROM course c
        LEFT JOIN question_data qd ON EXISTS (
            SELECT 1 
            FROM json_each(qd.course_codes) 
            WHERE json_each.value = c.course_code
        )
        GROUP BY c.course_code
    `;

    db.all(sql, [], (err, rows) => {
        if (err) {
            console.error("Error getting question counts:", err.message);
            return res.status(500).json({ success: false, message: "Database error" });
        }

        // Convert array of rows to an object with course codes as keys
        const counts = rows.reduce((acc, row) => {
            acc[row.course_code] = row.count;
            return acc;
        }, {});

        res.json({ success: true, counts });
    });
});



// Hämta en fråga för edit-läge
router.get('/questions/:id', async (req, res, next) => {
  try {
    const { id } = req.params;
    const q = await db.get(
      `SELECT id, question, answer_unit_id, answer_formula,
              variating_values, course_codes, question_type_id, hints
       FROM question_data
       WHERE id = ?`, [id]
    );
    if (!q) return res.status(404).json({ error: 'Fråga hittades ej' });
    // course_codes och hints är JSON-strängar i DB – pars’a dem
    q.course_codes     = JSON.parse(q.course_codes);
    q.hints            = JSON.parse(q.hints);
    q.variating_values = JSON.parse(q.variating_values);
    res.json(q);
  } catch (err) {
    next(err);
  }
});

// Uppdatera en befintlig fråga
router.put('/questions/:id', async (req, res, next) => {
  try {
    const { id } = req.params;
    const {
      question,
      answer_unit_id,
      answer_formula,
      variating_values,
      course_codes,
      question_type_id,
      hints
    } = req.body;

    await db.run(
      `UPDATE question_data
         SET question         = ?,
             answer_unit_id   = ?,
             answer_formula   = ?,
             variating_values = ?,
             course_codes     = ?,
             question_type_id = ?,
             hints            = ?
       WHERE id = ?`,
      [
        question,
        answer_unit_id,
        answer_formula,
        JSON.stringify(variating_values),
        JSON.stringify(course_codes),
        question_type_id,
        JSON.stringify(hints),
        id
      ]
    );

    res.json({ success: true });
  } catch (err) {
    next(err);
  }
});


// ----------------------------- Export Routes -----------------------------
module.exports = router;
