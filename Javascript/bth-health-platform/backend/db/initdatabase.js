const sqlite3 = require("sqlite3").verbose();
const path = require("path");
const { 
    initializeDatabase,
    insertUnits, 
    insertQtypes, 
    insertCourses, 
    insertMedicines, 
    insertQuestions,
    closeDatabase 
} = require("./insertDatabase");

// Path to the database file
const dbPath = path.resolve(__dirname, "question_data.db");
const db = new sqlite3.Database(dbPath, (err) => {
    if (err) {
        console.error("Error connecting to the database:", err.message);
    } else {
        console.log("Connected to the database.");
    }
});

// Enable foreign key constraints
db.run("PRAGMA foreign_keys = ON;");

// Function to initialize or modify a single table
const initializeTable = (tableName, columns) => {
    const columnDefinitions = columns.map(col => `${col.name} ${col.type}`).join(", ");
    const sql = `CREATE TABLE IF NOT EXISTS ${tableName} (${columnDefinitions})`;
    db.run(sql, (err) => {
        if (err) {
            console.error(`Error creating table ${tableName}:`, err.message);
        } else {
            console.log(`✅ Table ${tableName} initialized.`);
        }
    });
};

// // Initialize the "units" table
// initializeTable("units", [
//     { name: "id", type: "INTEGER PRIMARY KEY AUTOINCREMENT" },
//     { name: "ascii_name", type: "TEXT NOT NULL UNIQUE" },
//     { name: "accepted_answer", type: "TEXT NOT NULL CHECK(json_valid(accepted_answer))" }
// ]);

module.exports = {
  db,
  initializeTable
};