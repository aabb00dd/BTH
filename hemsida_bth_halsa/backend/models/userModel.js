    // Start of Usr DB jusr notes for now

    // db.run(`
    //     CREATE TABLE IF NOT EXISTS user (
    //         id INTEGER PRIMARY KEY AUTOINCREMENT,
    //         username TEXT UNIQUE NOT NULL,
    //         salt TEXT UNIQUE NOT NULL,
    //         hashed_password TEXT NOT NULL,
    //         course_list TEXT,
    //         last_activity DATE
    //     )
    // `);

    // db.run(`
    //     CREATE TABLE IF NOT EXISTS registration_keys (
    //         id INTEGER PRIMARY KEY AUTOINCREMENT,
    //         code TEXT UNIQUE NOT NULL,
    //         start_date TEXT NOT NULL,
    //         end_date TEXT NOT NULL,
    //         times_used INTEGER DEFAULT 0,
    //         user_id INTEGER NOT NULL,
    //         FOREIGN KEY (user_id) REFERENCES user(id) ON DELETE CASCADE
    //     )
    // `);