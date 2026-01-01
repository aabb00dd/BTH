const express = require("express");
const bodyParser = require("body-parser");
const cors = require("cors");
const questionsRoutes = require("./routes/questionRoutes");
const courseRoutes = require('./routes/courseRoutes');
const unitRoutes = require('./routes/unitsRoutes');
const medicineRoutes = require('./routes/medicineRoutes');
const QtypeRoutes = require('./routes/qtypeRoutes');
const feedbackRoutes = require("./routes/feedbackRoutes");
const answerHistoryRoutes = require("./routes/answerHistoryRoutes");
const commonRoutes = require("./routes/commonRoutes");

const app = express();

// -------------------------- Middleware -----------------------------
app.use(cors());
app.use(bodyParser.json());

// ------------------
// Dummy Login Route
// ------------------
app.post("/api/login", (req, res) => {
    const { username, password } = req.body;
    res.status(200).json({ success: true, message: "Login successful", user: { username } });
});

app.use("/api/question", questionsRoutes);
app.use('/api/course', courseRoutes);
app.use('/api/unit', unitRoutes);
app.use('/api/medicine', medicineRoutes);
app.use('/api/qtype', QtypeRoutes);
app.use("/api/answer/history", answerHistoryRoutes);
app.use("/api/feedback", feedbackRoutes);
app.use("/api", commonRoutes);

// -------------------------- Start Backend -----------------------------
const PORT = process.env.PORT || 5000;
app.listen(PORT, () => console.log(`Server running on port ${PORT}`));
