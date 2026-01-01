const sqlite3 = require("sqlite3").verbose();
const path = require("path");
const tempDb = require("./database_dict.js");

// Ensure the model is loaded to set up the database schema
require("../models/questionModel.js");

console.log("Loaded database.");

let db = null;

const initializeDatabase = () => {
  if (db) return db;
  
  const dbPath = path.resolve(__dirname, "question_data.db");
  db = new sqlite3.Database(dbPath, (err) => {
    if (err) {
      console.error("Error connecting to database:", err.message);
    } else {
      console.log("Connected to the SQLite database.");
    }
  });

  // Enable foreign key constraints
  db.run("PRAGMA foreign_keys = ON;");
  return db;
};

// Add these helper functions at the top
const logError = (operation, error, details = {}) => {
  console.error('\x1b[31m%s\x1b[0m', `❌ ${operation} failed:`);
  console.error('Error:', error.message);
  console.error('Details:', JSON.stringify(details, null, 2));
};

const logSuccess = (message) => {
  console.log('\x1b[32m%s\x1b[0m', `✅ ${message}`);
};

const logInfo = (message) => {
  console.log('\x1b[36m%s\x1b[0m', `ℹ️ ${message}`);
};

const recordExists = (table, column, value) => {
  return new Promise((resolve, reject) => {
    const sql = `SELECT 1 FROM ${table} WHERE ${column} = ? LIMIT 1`;

    db.get(sql, [value], (err, row) => {
      if (err) {
        console.error(`Error checking if record exists in ${table}:`, err.message);
        return reject(err);
      }
      resolve(!!row); // Return true if a matching record exists, false otherwise
    });
  });
};

const recordIsSame = (table, columns, values) => {
  return new Promise((resolve, reject) => {
    const sql = `SELECT ${columns.join(", ")} FROM ${table} WHERE ${columns[0]} = ? LIMIT 1`;

    db.get(sql, [values[0]], (err, row) => {
      if (err) {
        console.error(`Error checking if record is the same in ${table}:`, err.message);
        return reject(err);
      }

      if (!row) {
        return resolve(false); // Record does not exist
      }

      // Compare each column value
      const isSame = columns.every((col, index) => {
        const dbValue = row[col];
        const providedValue = values[index];
        return dbValue === providedValue;
      });

      resolve(isSame); // Return true if all values match, false otherwise
    });
  });
};

const insertData = async (table, columns, values) => {
  return new Promise((resolve, reject) => {
    const placeholders = columns.map(() => "?").join(", ");
    const sql = `INSERT INTO ${table} (${columns.join(", ")}) VALUES (${placeholders})`;
    db.run(sql, values, function (err) {
      if (err) {
        console.error(`Error inserting into ${table}:`, err.message);
        reject(err);
      } else {
        resolve(this.lastID);
      }
    });
  });
};

const updateData = async (table, columns, values, conditions, conditionValues) => {
  return new Promise((resolve, reject) => {
    const updates = columns.map((col) => `${col} = ?`).join(", ");
    const conditionString = conditions.map((col) => `${col} = ?`).join(" AND ");
    const sql = `UPDATE ${table} SET ${updates} WHERE ${conditionString}`;

    db.run(sql, [...values, ...conditionValues], function (err) {
      if (err) {
        console.error(`Error updating ${table}:`, err.message);
        return reject(err);
      }
      resolve(this.changes); // Return the number of rows updated
    });
  });
};

const insertUnits = async () => {
  for (const unit of tempDb.units) {
    const exists = await recordExists("units", "id", unit.id);
    const new_record = [
      unit.id,
      unit.ascii_name,
      JSON.stringify(unit.accepted_answer),
      unit.precision
    ];
    if (exists) {
      const isSame = await recordIsSame("units", ["id", "ascii_name", "accepted_answer", "precision"], new_record);
      if (!isSame) {
        await updateData("units", ["ascii_name", "accepted_answer", "precision"], [unit.ascii_name, JSON.stringify(unit.accepted_answer), unit.precision], ["id"], [unit.id] );
        console.log(`✅ Updated unit with ID ${unit.id}`);
      } else {
        console.log(`✅ Unit with ID ${unit.id} already exists and is the same.`);
      }
    } else {
      await insertData( "units", ["id", "ascii_name", "accepted_answer", "precision"], new_record );
      console.log(`✅ Inserted unit with ID ${unit.id}`);
    }
  }
  console.log("✅ Units processed (inserted or updated).");
};

const insertQtypes = async () => {
  for (const qtype of tempDb.qtypes) {
    const exists = await recordExists("qtype", "id", qtype.id); // Check if the record exists
    const new_record = [qtype.id, qtype.name];

    if (exists) {
      const isSame = await recordIsSame("qtype", ["id", "name"], new_record); // Check if the record is the same
      if (!isSame) {
        await updateData("qtype", ["name"], [qtype.name], ["id"], [qtype.id]); // Update if different
        console.log(`✅ Updated qtype with ID ${qtype.id}`);
      } else {
        console.log(`✅ Qtype with ID ${qtype.id} already exists and is the same.`);
      }
    } else {
      await insertData("qtype", ["id", "name"], new_record); // Insert if it doesn't exist
      console.log(`✅ Inserted qtype with ID ${qtype.id}`);
    }
  }
  console.log("✅ Qtypes processed (inserted or updated).");
};

const insertCourses = async () => {
  for (const course of tempDb.courses) {
    const new_record = [
      course.course_code,
      course.course_name || "Unnamed Course",
      JSON.stringify(course.question_types || []),
    ];

    const exists = await recordExists("course", "course_code", course.course_code); // Check if the record exists

    if (exists) {
      const isSame = await recordIsSame(
        "course",
        ["course_code", "course_name", "question_types"],
        new_record
      ); // Check if the record is the same

      if (!isSame) {
        await updateData(
          "course",
          ["course_name", "question_types"],
          [course.course_name || "Unnamed Course", JSON.stringify(course.question_types || [])],
          ["course_code"],
          [course.course_code]
        ); // Update if different
        console.log(`✅ Updated course with code ${course.course_code}`);
      } else {
        console.log(`✅ Course with code ${course.course_code} already exists and is the same.`);
      }
    } else {
      await insertData(
        "course",
        ["course_code", "course_name", "question_types"],
        new_record
      ); // Insert if it doesn't exist
      console.log(`✅ Inserted course with code ${course.course_code}`);
    }
  }
  console.log("✅ Courses processed (inserted or updated).");
};

const insertMedicines = async () => {
  for (const medicine of tempDb.medicine) {
    try {
      // Validate variating_values is valid JSON
      try {
        if (typeof medicine.variating_values === 'string') {
          JSON.parse(medicine.variating_values);
        }
      } catch (jsonError) {
        logError('JSON Validation', jsonError, {
          id: medicine.id,
          namn: medicine.namn,
          variating_values: medicine.variating_values
        });
        continue;
      }

      const new_record = [
        medicine.id,
        medicine.namn,
        medicine.fass_link,
        medicine.variating_values
      ];

      const exists = await recordExists("medicine", "id", medicine.id);

      if (exists) {
        const isSame = await recordIsSame(
          "medicine",
          ["id", "namn", "fass_link", "variating_values"],
          new_record
        );

        if (!isSame) {
          await updateData(
            "medicine",
            ["namn", "fass_link", "variating_values"],
            [medicine.namn, medicine.fass_link, medicine.variating_values],
            ["id"],
            [medicine.id]
          );
          logSuccess(`Updated medicine with ID ${medicine.id}`);
        }
      } else {
        await insertData(
          "medicine",
          ["id", "namn", "fass_link", "variating_values"],
          new_record
        );
        logSuccess(`Inserted new medicine with ID ${medicine.id}`);
      }
    } catch (error) {
      logError('Medicine Processing', error, {
        id: medicine.id,
        namn: medicine.namn
      });
    }
  }
  logSuccess("Medicines processing completed");
};

const insertQuestions = async () => {
  for (const q of tempDb.question_data) {
    if (q.preamble && Array.isArray(q.question_data)) {
      for (const sub of q.question_data) {
        const fullQuestion = `${q.preamble} ${sub.question}`;
        const new_record = [
          fullQuestion,
          sub.answer_unit_id,
          sub.answer_formula,
          JSON.stringify(q.variating_values || {}),
          JSON.stringify(q.course_codes || []),
          q.question_type_id || 1,
          JSON.stringify(sub.hints || []),
        ];

        const exists = await recordExists("question_data", "question", fullQuestion); // Check if the record exists

        if (exists) {
          const isSame = await recordIsSame(
            "question_data",
            [
              "question",
              "answer_unit_id",
              "answer_formula",
              "variating_values",
              "course_codes",
              "question_type_id",
              "hints",
            ],
            new_record
          ); // Check if the record is the same

          if (!isSame) {
            await updateData(
              "question_data",
              [
                "answer_unit_id",
                "answer_formula",
                "variating_values",
                "course_codes",
                "question_type_id",
                "hints",
              ],
              [
                sub.answer_unit_id,
                sub.answer_formula,
                JSON.stringify(q.variating_values || {}),
                JSON.stringify(q.course_codes || []),
                q.question_type_id || 1,
                JSON.stringify(sub.hints || []),
              ],
              ["question"],
              [fullQuestion]
            ); // Update if different
            const questionId = await getQuestionId(fullQuestion);
            console.log(`✅ Q${questionId}`);
          }
        } else {
          const lastId = await insertData(
            "question_data",
            [
              "question",
              "answer_unit_id",
              "answer_formula",
              "variating_values",
              "course_codes",
              "question_type_id",
              "hints",
            ],
            new_record
          ); // Insert if it doesn't exist
          console.log(`✅ Q${lastId}`);
        }
      }
    } else {
      const new_record = [
        q.question,
        q.answer_unit_id,
        q.answer_formula,
        JSON.stringify(q.variating_values || {}),
        JSON.stringify(q.course_codes || []),
        q.question_type_id || 1,
        JSON.stringify(q.hints || []),
      ];

      const exists = await recordExists("question_data", "question", q.question);

      if (exists) {
        const isSame = await recordIsSame(
          "question_data",
          [
            "question",
            "answer_unit_id",
            "answer_formula",
            "variating_values",
            "course_codes",
            "question_type_id",
            "hints",
          ],
          new_record
        );

        if (!isSame) {
          await updateData(
            "question_data",
            [
              "answer_unit_id",
              "answer_formula",
              "variating_values",
              "course_codes",
              "question_type_id",
              "hints",
            ],
            [
              q.answer_unit_id,
              q.answer_formula,
              JSON.stringify(q.variating_values || {}),
              JSON.stringify(q.course_codes || []),
              q.question_type_id || 1,
              JSON.stringify(q.hints || []),
            ],
            ["question"],
            [q.question]
          );
          const questionId = await getQuestionId(q.question);
          console.log(`✅ Q${questionId}`);
        }
      } else {
        const lastId = await insertData(
          "question_data",
          [
            "question",
            "answer_unit_id",
            "answer_formula",
            "variating_values",
            "course_codes",
            "question_type_id",
            "hints",
          ],
          new_record
        );
        console.log(`✅ Q${lastId}`);
      }
    }
  }
  console.log("✅ Questions processed");
};

// Add this helper function
const getQuestionId = (question) => {
  return new Promise((resolve, reject) => {
    db.get('SELECT id FROM question_data WHERE question = ?', [question], (err, row) => {
      if (err) reject(err);
      resolve(row ? row.id : null);
    });
  });
};

const closeDatabase = () => {
  return new Promise((resolve, reject) => {
    if (!db) {
      resolve();
      return;
    }

    db.close((err) => {
      if (err) {
        console.error("Error closing database:", err);
        reject(err);
      } else {
        console.log("Database closed successfully");
        db = null;
        resolve();
      }
    });
  });
};

const runInserts = async () => {
  try {
    db = initializeDatabase();
    await insertUnits();
    await insertQtypes();
    await insertCourses();
    await insertMedicines();
    await insertQuestions();
    console.log("🎉 All data inserted successfully!");
    await closeDatabase();
  } catch (error) {
    console.error("❌ Error inserting data:", error);
    await closeDatabase();
  }
};

// Export the functions
module.exports = {
  initializeDatabase,
  insertUnits,
  insertQtypes,
  insertCourses,
  insertMedicines,
  insertQuestions,
  closeDatabase
};
