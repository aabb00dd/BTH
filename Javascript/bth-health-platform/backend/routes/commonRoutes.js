const express = require("express");
const router = express.Router();
const { getTable } = require("../models/questionModel");


// The valid tables
const tables = ["units", "course", "medicine", "qtype"];


tables.forEach(table => {
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
            const records = await getTable(tableName);
            console.log("Fetched", tableName + ":", records);
            res.status(200).json({ success: true, records:  records});
        } catch (err) {
            res.status(500).json({ success: false, message: "Error fetching records" });
        }
    });
});

// ----------------------------- Export Routes -----------------------------
module.exports = router;