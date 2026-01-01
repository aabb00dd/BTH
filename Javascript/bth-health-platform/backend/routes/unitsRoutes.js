// backend/routes/unitsRoutes.js
const express = require("express");
const router = express.Router();
const { addRecord, updateRecord, deleteRecord, getTable } = require("../models/questionModel");
const { validateAcceptedAnswer } = require("../helpers/unitsHelpers");

// ----------------------------- Add Entry -----------------------------
router.post("/add", async (req, res) => {
    try {
        const { ascii_name, accepted_answer } = req.body;
        if (!ascii_name || !accepted_answer) {
            return res.status(400).json({ success: false, message: "Missing required fields" });
        }
        if (!validateAcceptedAnswer(accepted_answer)) {
            return res.status(400).json({
                success: false,
                message: "Invalid accepted_answer format. Must be a JSON array of strings."
            });
        }
        try {
            const result = await addRecord("units", ["ascii_name", "accepted_answer"], [ascii_name, accepted_answer]);
            res.status(201).json({ success: true, message: "Record successfully added", id: result.id });
        } catch (err) {
            if (err.message.includes("UNIQUE constraint failed")) {
                return res.status(409).json({
                    success: false,
                    message: "ascii_name must be unique. The provided value already exists."
                });
            }
            throw err;
        }
    } catch (err) {
        res.status(err.status || 500).json({ success: false, message: err.message || "Error adding record" });
    }
});

// ----------------------------- Get All Units -----------------------------
router.get("/all", async (req, res) => {
    try {
        const units = await getTable("units");
        res.status(200).json({ success: true, records: units });
    } catch (err) {
        res.status(err.status || 500).json({ success: false, message: err.message || "Error retrieving records" });
    }
});

// ----------------------------- Update Unit -----------------------------
router.put("/update/:id", async (req, res) => {
    try {
        const { ascii_name, accepted_answer } = req.body;
        if (!ascii_name || !accepted_answer) {
            return res.status(400).json({ success: false, message: "Missing required fields" });
        }
        if (!validateAcceptedAnswer(accepted_answer)) {
            return res.status(400).json({
                success: false,
                message: "Invalid accepted_answer format. Must be a JSON array of strings."
            });
        }
        const id = req.params.id;
        const updates = { ascii_name, accepted_answer };
        const result = await updateRecord("units", id, updates);
        res.status(200).json({ success: true, message: "Record updated", result });
    } catch (err) {
        res.status(err.status || 500).json({ success: false, message: err.message || "Error updating record" });
    }
});

// ----------------------------- Delete Unit -----------------------------
router.delete("/delete/:id", async (req, res) => {
    try {
        const id = req.params.id;
        const result = await deleteRecord("units", "id", id);
        res.status(200).json({ success: true, message: result.message });
    } catch (err) {
        res.status(err.status || 500).json({ success: false, message: err.message || "Error deleting record" });
    }
});

// ----------------------------- Export Routes -----------------------------
module.exports = router;
