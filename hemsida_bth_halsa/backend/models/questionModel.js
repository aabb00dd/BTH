// backend/models/questionModel.js
const { initializeTable, db } = require("../db/initdatabase");

// Initialize the "units" table
initializeTable("units", [
    { name: "id", type: "INTEGER PRIMARY KEY AUTOINCREMENT" },
    { name: "ascii_name", type: "TEXT NOT NULL UNIQUE" },
    { name: "precision", type: "INT DEFAULT NULL" },
    { name: "accepted_answer", type: "TEXT NOT NULL CHECK(json_valid(accepted_answer))" }
]);

// Initialize the "course" table
initializeTable("course", [
    { name: "course_code", type: "TEXT PRIMARY KEY NOT NULL UNIQUE" },
    { name: "course_name", type: "TEXT NOT NULL" },
    { name: "question_types", type: "TEXT CHECK(json_valid(question_types))" },
    { name: "num_questions", type: "INTEGER DEFAULT 0" }
]);

// Initialize the "medicine" table
initializeTable("medicine", [
    { name: "id", type: "INTEGER PRIMARY KEY AUTOINCREMENT" },
    { name: "namn", type: "TEXT UNIQUE NOT NULL" },
    { name: "fass_link", type: "TEXT NOT NULL" },
    { name: "variating_values", type: "TEXT CHECK(json_valid(variating_values))" }
]);

// Initialize the "qtype" table
initializeTable("qtype", [
    { name: "id", type: "INTEGER PRIMARY KEY AUTOINCREMENT" },
    { name: "name", type: "TEXT NOT NULL UNIQUE" }
]);

// Initialize the "question_data" table
initializeTable("question_data", [
    { name: "id", type: "INTEGER PRIMARY KEY AUTOINCREMENT" },
    { name: "question", type: "TEXT NOT NULL" },
    { name: "answer_unit_id", type: "INTEGER NOT NULL" },
    { name: "answer_formula", type: "TEXT NOT NULL" },
    { name: "variating_values", type: "TEXT NOT NULL" },
    { name: "course_codes", type: "TEXT NOT NULL CHECK(json_valid(course_codes))" },
    { name: "question_type_id", type: "INTEGER NOT NULL" },
    { name: "hints", type: "TEXT NOT NULL DEFAULT '[]' CHECK(json_valid(hints))" }
]);

// Initialize the "answer_history" table
initializeTable("answer_history", [
    { name: "id", type: "INTEGER PRIMARY KEY AUTOINCREMENT" },
    { name: "question_id", type: "INTEGER NOT NULL" },
    { name: "correct", type: "INTEGER NOT NULL CHECK(correct IN (1, -1))" },
    { name: "course_code", type: "TEXT NOT NULL" },
    { name: "date", type: "DATETIME DEFAULT CURRENT_TIMESTAMP" }
]);

// ------------------------- Error Handling -----------------------------
/**
 * Handles database insert errors and returns appropriate messages.
 * @param {Error} err - The SQLite error object.
 * @returns {Object} - An error message and status code.
 */
const handleInsertError = (err) => {
    if (!err) return null;
    if (err.message.includes("FOREIGN KEY constraint failed")) {
        return { message: "Foreign key constraint failed. Check that referenced data exists.", status: 400 };
    }
    if (err.message.includes("UNIQUE constraint failed")) {
        return { message: "Duplicate entry. This record already exists.", status: 409 };
    }
    return { message: "Database error occurred.", status: 500 };
};

// ------------------------- Utility Functions -----------------------------
/**
 * Updates the `num_questions` column in the course table and logs the results.
 */
const updateNumQuestions = () => {
    return new Promise((resolve, reject) => {
        const updateSql = `
            WITH QuestionCounts AS (
                SELECT json_each.value as course_code, COUNT(*) as question_count
                FROM question_data, json_each(question_data.course_codes)
                GROUP BY json_each.value
            )
            UPDATE course
            SET num_questions = COALESCE((
                SELECT question_count
                FROM QuestionCounts
                WHERE QuestionCounts.course_code = course.course_code
            ), 0)
        `;

        db.serialize(() => {
            // First run the update
            db.run(updateSql, [], (err) => {
                if (err) {
                    console.error("Error updating num_questions:", err.message);
                    return reject(err);
                }
                console.log("num_questions column updated successfully.");

                // Then fetch and log the results
                const logSql = `
                    SELECT course_code, course_name, num_questions 
                    FROM course 
                    ORDER BY course_code
                `;
                
                db.all(logSql, [], (err, rows) => {
                    if (err) {
                        console.error("Error fetching results:", err.message);
                        return reject(err);
                    }
                    console.log("\nQuestion counts per course:");
                    console.log("-----------------------------");
                    rows.forEach(row => {
                        console.log(`${row.course_code}: ${row.num_questions} questions`);
                    });
                    console.log("-----------------------------\n");
                    resolve();
                });
            });
        });
    });
};

// ------------------------- Database Operations -----------------------------
/**
 * Adds a new record to the database.
 * @param {string} table - The table name.
 * @param {Array<string>} columns - The column names.
 * @param {Array<any>} values - The values to insert.
 * @returns {Promise<Object>} - The inserted record ID.
 */
const addRecord = (table, columns, values) => {
    return new Promise((resolve, reject) => {
        const formattedValues = values.map(value =>
            typeof value === "string" ? value.trim() :
            typeof value === "object" ? JSON.stringify(value) : value
        );
        const placeholders = values.map(() => "?").join(", ");
        const sql = `INSERT INTO ${table} (${columns.join(", ")}) VALUES (${placeholders})`;
        db.run(sql, formattedValues, function (err) {
            const error = handleInsertError(err);
            if (error) return reject(error);

            // Update num_questions after adding a question
            if (table === "question_data") {
                updateNumQuestions();
            }

            resolve({ id: this.lastID });
        });
    });
};

/**
 * Retrieves all records from a table.
 * @param {string} table - The table name.
 * @returns {Promise<Array>} - The retrieved records.
 */
const getTable = async (table) => {
    return new Promise((resolve, reject) => {
        db.all(`SELECT * FROM ${table}`, (err, rows) => {
            if (err) return reject({ message: "Database error", status: 500 });
            resolve(rows);
        });
    });
};

/**
 * Retrieves a specific record by ID.
 * @param {string} table - The table name.
 * @param {number} id - The record ID.
 * @returns {Promise<Object>} - The retrieved record.
 */
const getRecordById = (table, id) => {
    return new Promise((resolve, reject) => {
        db.get(`SELECT * FROM ${table} WHERE id = ?`, [id], (err, row) => {
            if (err) return reject({ message: "Database error", status: 500 });
            if (!row) return reject({ message: "Record not found", status: 404 });
            resolve(row);
        });
    });
};

/**
 * Retrieves a specific record by a given column value.
 * @param {string} table - The table name.
 * @param {string} column - The column name for the value.
 * @param {any} val - The value to query by.
 * @returns {Promise<Object>} - The retrieved record.
 */
const getRecordWhere = (table, column, val) => {
    return new Promise((resolve, reject) => {
        db.all(`SELECT * FROM ${table} WHERE ${column} = ?`, [val], (err, row) => {
            if (err) return reject({ message: "Database error", status: 500 });
            if (!row) return reject({ message: "Record not found", status: 404 });
            resolve(row);
        });
    });
};

/**
 * Updates a record in the database by id.
 * @param {string} table - The table name.
 * @param {number} id - The record ID.
 * @param {Object} updates - The fields to update.
 * @returns {Promise<Object>} - The update result.
 */
const updateRecord = (table, id, updates) => {
    return new Promise((resolve, reject) => {
        const formattedUpdates = Object.keys(updates).reduce((acc, key) => {
            acc[key] = typeof updates[key] === "string" ? updates[key].trim() :
                        typeof updates[key] === "object" ? JSON.stringify(updates[key]) : updates[key];
            return acc;
        }, {});

        const keys = Object.keys(formattedUpdates).map(key => `${key} = ?`).join(", ");
        const values = [...Object.values(formattedUpdates), id];
        const sql = `UPDATE ${table} SET ${keys} WHERE id = ?`;
        db.run(sql, values, function (err) {
            if (err) return reject({ message: "Database error", status: 500 });
            if (this.changes === 0) return reject({ message: "Record not found", status: 404 });
            resolve({ message: "Record updated" });
        });
    });
};

/**
 * Deletes a record from the database.
 * @param {string} table - The table name.
 * @param {string} column - The column name for the value.
 * @param {number} val - The record ID.
 * @returns {Promise<Object>} - The deletion result.
 */
const deleteRecord = (table, column, val) => {
    return new Promise((resolve, reject) => {
        db.run(`DELETE FROM ${table} WHERE ${column} = ?`, [val], function (err) {
            if (err) return reject({ message: "Database error", status: 500 });

            // Update num_questions after deleting a question
            if (table === "question_data") {
                updateNumQuestions();
            }

            resolve({ message: `Deleted ${this.changes} record(s).` });
        });
    });
};

// ------------------------- Export Functions -----------------------------
module.exports = {
    addRecord,
    getTable,
    getRecordById,
    getRecordWhere,
    updateRecord,
    deleteRecord,
    updateNumQuestions
};
