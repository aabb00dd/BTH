const express = require("express");
const router = express.Router();
const { addRecord, getRecords, getRecordById, updateRecord, deleteRecord } = require("../../../backend/models/questionModel");

const tables = ["units", "course", "medicine", "qtype", "question_data"];

tables.forEach(table => {
    // -------------------------- Add New Record -----------------------------
    /**
     * @route POST /api/:table/add
     * @desc Adds a new record to the specified table
     */
    router.post(`/:table/add`, async (req, res) => {
        try {
            const tableName = req.params.table;
            if (!tables.includes(tableName)) {
                return res.status(400).json({ success: false, message: "Invalid table name" });
            }
            const keys = Object.keys(req.body);
            const values = Object.values(req.body);
            if (keys.length === 0) {
                return res.status(400).json({ success: false, message: "Missing required fields" });
            }
            const result = await addRecord(tableName, keys, values);
            res.status(201).json({ success: true, message: "Record successfully added", id: result.id });
        } catch (err) {
            res.status(err.status || 500).json({ success: false, message: err.message || "Error adding record" });
        }
    });

    // -------------------------- Get All Records -----------------------------
    /**
     * @route GET /api/:table/all
     * @desc Retrieves all records from the specified table
     */
    router.get(`/:table/all`, async (req, res) => {
        try {
            const tableName = req.params.table;
            if (!tables.includes(tableName)) {
                return res.status(400).json({ success: false, message: "Invalid table name" });
            }
            const records = await getRecords(tableName);
            res.status(200).json({ success: true, records });
        } catch (err) {
            res.status(500).json({ success: false, message: "Error fetching records" });
        }
    });

    // -------------------------- Get Record by ID -----------------------------
    /**
     * @route GET /api/:table/:id
     * @desc Retrieves a specific record by ID
     */
    router.get(`/:table/:id`, async (req, res) => {
        try {
            const tableName = req.params.table;
            if (!tables.includes(tableName)) {
                return res.status(400).json({ success: false, message: "Invalid table name" });
            }
            const record = await getRecordById(tableName, req.params.id);
            res.status(200).json({ success: true, record });
        } catch (err) {
            res.status(err.status || 500).json({ success: false, message: err.message || "Error fetching record" });
        }
    });

    // -------------------------- Update Record -----------------------------
    /**
     * @route PUT /api/:table/update/:id
     * @desc Updates a specific record by ID
     */
    router.put(`/:table/update/:id`, async (req, res) => {
        try {
            const tableName = req.params.table;
            if (!tables.includes(tableName)) {
                return res.status(400).json({ success: false, message: "Invalid table name" });
            }
            const updates = req.body;
            if (Object.keys(updates).length === 0) {
                return res.status(400).json({ success: false, message: "No fields provided for update" });
            }
            await updateRecord(tableName, req.params.id, updates);
            res.status(200).json({ success: true, message: "Record updated successfully" });
        } catch (err) {
            res.status(err.status || 500).json({ success: false, message: err.message || "Error updating record" });
        }
    });

    // -------------------------- Delete Record -----------------------------
    /**
     * @route DELETE /api/:table/delete/:id
     * @desc Deletes a specific record by ID
     */
    router.delete(`/:table/delete/:id`, async (req, res) => {
        try {
            const tableName = req.params.table;
            if (!tables.includes(tableName)) {
                return res.status(400).json({ success: false, message: "Invalid table name" });
            }
            await deleteRecord(tableName, req.params.id);
            res.status(200).json({ success: true, message: "Record deleted successfully" });
        } catch (err) {
            res.status(err.status || 500).json({ success: false, message: err.message || "Error deleting record" });
        }
    });
});

// ----------------------------- Export Routes -----------------------------
module.exports = router;
