// backend/models/feedbackModel.js
const sqlite3 = require("sqlite3").verbose();
const path = require("path");
const dbPath = path.resolve(__dirname, "../db/question_data.db");
const db = new sqlite3.Database(dbPath, (err) => {
  if (err) {
    console.error("Failed to connect to the database:", err.message);
  } else {
    console.log("Connected to the SQLite database (feedbackModel).");
  }
});

// Create the feedback table if it doesn't exist
db.serialize(() => {
  db.run(`
    CREATE TABLE IF NOT EXISTS feedback (
      id INTEGER PRIMARY KEY AUTOINCREMENT,
      feedback_text TEXT NOT NULL,
      created_at DATETIME DEFAULT CURRENT_TIMESTAMP
    )
  `);
});

/**
 * Inserts a new feedback record into the feedback table.
 * @param {string} feedback_text - The feedback text from user.
 * @returns {Promise<object>} - The newly created feedback record ID.
 */
function addFeedback(feedback_text) {
  return new Promise((resolve, reject) => {
    const sql = `INSERT INTO feedback (feedback_text) VALUES (?)`;
    db.run(sql, [feedback_text], function (err) {
      if (err) {
        return reject(err);
      }
      resolve({ id: this.lastID });
    });
  });
}

/**
 * Retrieves all feedback records from the database (newest first).
 * @returns {Promise<Array>} - Array of all feedback rows.
 */
function getAllFeedback() {
  return new Promise((resolve, reject) => {
    const sql = `SELECT * FROM feedback ORDER BY created_at DESC`;
    db.all(sql, [], (err, rows) => {
      if (err) {
        return reject(err);
      }
      resolve(rows);
    });
  });
}

/**
 * Deletes a feedback record by ID.
 * @param {number} feedbackId
 * @returns {Promise<number>} - Number of rows deleted.
 */
function deleteFeedback(feedbackId) {
  return new Promise((resolve, reject) => {
    db.run(`DELETE FROM feedback WHERE id = ?`, [feedbackId], function (err) {
      if (err) return reject(err);
      resolve(this.changes); // 0 if not found, 1 if deleted
    });
  });
}

module.exports = {
  addFeedback,
  getAllFeedback,
  deleteFeedback
};