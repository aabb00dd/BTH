const { updateNumQuestions } = require("../models/questionModel");
const { 
    initializeDatabase,
    insertUnits, 
    insertQtypes, 
    insertCourses, 
    insertMedicines, 
    insertQuestions,
    closeDatabase 
} = require("./insertDatabase");


const clearQuestionTable = () => {
    return new Promise((resolve, reject) => {
        const db = initializeDatabase();
        db.serialize(() => {
            // Disable foreign key constraints
            db.run("PRAGMA foreign_keys = OFF;", (err) => {
                if (err) {
                    console.error("Error disabling foreign keys:", err);
                    return reject(err);
                }
            });

            // Clear the question_data table
            db.run("DELETE FROM question_data", (err) => {
                if (err) {
                    console.error("Error clearing question table:", err);
                    return reject(err);
                } else {
                    console.log("✅ Question table cleared successfully");
                }
            });

            // Re-enable foreign key constraints
            db.run("PRAGMA foreign_keys = ON;", (err) => {
                if (err) {
                    console.error("Error re-enabling foreign keys:", err);
                    return reject(err);
                }
                resolve();
            });
        });
    });
};

const runCommand = async () => {
    try {
        console.log("\n=== Starting Database Update ===\n");

        // Initialize database connection
        initializeDatabase();

        // Insert/update database records
        await insertUnits();
        await insertQtypes();
        await insertCourses();
        await insertMedicines();
        
        // Clear questions table before inserting new questions
        console.log("\n--- Clearing Question Table ---");
        await clearQuestionTable();
        
        // Insert questions
        await insertQuestions();

        // Update question counts
        console.log("\n--- Updating Question Counts ---");
        await updateNumQuestions();

        console.log("\n✅ Database commands executed successfully.");
        
        // Close database connection
        await closeDatabase();
        
        process.exit(0);
    } catch (err) {
        console.error("\n❌ Error running database commands:");
        console.error(err);
        console.error("\nStack trace:", err.stack);
        
        try {
            await closeDatabase();
        } catch (closeError) {
            console.error("Error while closing database:", closeError);
        }
        
        process.exit(1);
    }
};

// Run the command
runCommand();