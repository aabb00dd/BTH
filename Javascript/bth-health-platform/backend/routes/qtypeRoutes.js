// backend/routes/qtypeRoutes.js
const express = require("express");
const router = express.Router();
const { addRecord, updateRecord, deleteRecord, getTable } = require("../models/questionModel");

// ----------------------------- Get All QTypes -----------------------------
router.get("/all", async (req, res) => {
    try {
        const records = await getTable("qtype");
        return res.status(200).json({ success: true, records });
    } catch (err) {
        return res.status(500).json({ success: false, message: "Error retrieving qtype records" });
    }
});

// -----------------------------------------
// Add a new QType
// -----------------------------------------
router.post("/add", async (req, res) => {
    try {
        let { name } = req.body; // Only allow `name`

        // Validate name
        if (!name || typeof name !== "string") {
            return res.status(400).json({ success: false, message: "Invalid or missing 'name' (must be a string)" });
        }

        // Insert into database (right_answers, wrong_answers default to 0, history_json default to `{}`)
        try {
            const result = await addRecord(
                "qtype",
                ["name", "history_json"],
                [name, "{}"] // Default history_json to an empty object "{}"
            );
            return res.status(201).json({ success: true, message: "QType successfully added", id: result.id });
        } catch (err) {
            if (err.message && err.message.includes("UNIQUE constraint failed")) {
                return res.status(409).json({ success: false, message: "QType name must be unique. The provided value already exists." });
            }
            throw err;
        }
    } catch (err) {
        console.error("Error in /qtype/add route:", err);
        res.status(500).json({ success: false, message: "Internal server error" });
    }
});

// -----------------------------------------
// Edit an existing QType
// -----------------------------------------
router.put("/edit", async (req, res) => {
    try {
        let { id, name } = req.body; // Require both id and new name

        // Validate input
        if (!id || isNaN(parseInt(id))) {
            return res.status(400).json({ success: false, message: "Invalid or missing 'id'" });
        }
        if (!name || typeof name !== "string") {
            return res.status(400).json({ success: false, message: "Invalid or missing 'name' (must be a string)" });
        }

        // Update record in the database
        await updateRecord("qtype", id, { name });
        return res.status(200).json({ success: true, message: "QType successfully updated" });
    } catch (err) {
        if (err.message && err.message.includes("UNIQUE constraint failed")) {
            return res.status(409).json({ success: false, message: "QType name must be unique. The provided value already exists." });
        }
        console.error("Error in /qtype/edit route:", err);
        return res.status(500).json({ success: false, message: "Internal server error" });
    }
});

// -----------------------------------------
// Delete an existing QType
// -----------------------------------------
router.delete("/delete/:id", async (req, res) => {
    try {
        const { id } = req.params;
        if (!id || isNaN(parseInt(id))) {
            return res.status(400).json({ success: false, message: "Invalid or missing 'id'" });
        }

        // Delete record from the database
        const result = await deleteRecord("qtype", "id", id);
        return res.status(200).json({ success: true, message: "QType successfully deleted", details: result });
    } catch (err) {
        console.error("Error in /qtype/delete route:", err);
        return res.status(500).json({ success: false, message: err.message || "Internal server error" });
    }
});

module.exports = router;
