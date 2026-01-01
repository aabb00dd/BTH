// backend/routes/answerHistoryRoutes.js
const express = require("express");
const router = express.Router();
const sqlite3 = require("sqlite3").verbose();
const path = require("path");
const dbPath = path.resolve(__dirname, "../db/question_data.db");

const db = new sqlite3.Database(dbPath, (err) => {
  if (err) {
    console.error("Error connecting to DB:", err.message);
  } else {
    console.log("Connected to SQLite DB in answerHistoryRoutes.");
  }
});

// GET /api/answer/history/aggregated
// Supports optional filters: question_id, question_type, course, start_date, end_date, aggregation
router.get("/aggregated", (req, res) => {
  const { question_id, question_type, course, start_date, end_date, aggregation } = req.query;
  
  // Determine the date format based on the aggregation interval.
  let dateSelect;
  if (aggregation === "weekly") {
    dateSelect = "strftime('%Y-%W', a.date)";
  } else if (aggregation === "monthly") {
    dateSelect = "strftime('%Y-%m', a.date)";
  } else {
    // Default to daily aggregation.
    dateSelect = "date(a.date)";
  }

  let sql = `
    SELECT 
      ${dateSelect} as answer_date,
      SUM(CASE WHEN a.correct = 1 THEN 1 ELSE 0 END) as correct_count,
      SUM(CASE WHEN a.correct = -1 THEN 1 ELSE 0 END) as wrong_count
    FROM answer_history a
  `;
  const params = [];
  let joinClause = "";
  const whereClauses = [];

  // If filtering by question_type, join question_data table for that filter.
  if (question_type && question_type !== "all") {
    joinClause = " JOIN question_data q ON a.question_id = q.id ";
    whereClauses.push("q.question_type_id = ?");
    params.push(question_type);
  }
  // If filtering by course, filter directly on a.course_code.
  if (course && course !== "all") {
    whereClauses.push("a.course_code = ?");
    params.push(course.toUpperCase());
  }
  if (question_id && question_id !== "all") {
    whereClauses.push("a.question_id = ?");
    params.push(question_id);
  }
  if (start_date) {
    whereClauses.push("date(a.date) >= ?");
    params.push(start_date);
  }
  if (end_date) {
    whereClauses.push("date(a.date) <= ?");
    params.push(end_date);
  }

  sql += joinClause;
  if (whereClauses.length > 0) {
    sql += " WHERE " + whereClauses.join(" AND ");
  }
  sql += ` GROUP BY ${dateSelect} ORDER BY answer_date`;

  db.all(sql, params, (err, rows) => {
    if (err) {
      return res.status(500).json({ success: false, message: err.message });
    }
    res.json({ success: true, aggregatedData: rows });
  });
});

module.exports = router;
