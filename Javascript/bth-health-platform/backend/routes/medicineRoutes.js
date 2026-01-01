const express = require("express");
const router = express.Router();
const { addRecord, getTable, getRecordById, updateRecord, deleteRecord } = require("../models/questionModel");
const { isValidUrl } = require("../helpers/medicineHelpers");

// ----------------------------- Add Entry -----------------------------
router.post("/add", async (req, res) => {
    try {
        let { name, fass_link, styrkor_doser } = req.body;

        // Input validation
        if (!name || !fass_link || !styrkor_doser) {
            return res.status(400).json({ success: false, message: "Missing required fields: name, fass_link, styrkor_doser" });
        }
        if (typeof name !== "string" || typeof fass_link !== "string") {
            return res.status(400).json({ success: false, message: "Invalid data type: name and fass_link must be strings" });
        }

        // Ensure base URL is correct
        try {
            let urlObj = new URL(fass_link.startsWith("http") ? fass_link : "https://" + fass_link);
            urlObj.protocol = "https:";
            urlObj.host = "www.fass.se";
            if (!urlObj.searchParams.has("userType")) {
                urlObj.searchParams.append("userType", "0");
            } else {
                urlObj.searchParams.set("userType", "0");
            }
            fass_link = urlObj.toString();
        } catch (err) {
            return res.status(400).json({ success: false, message: "Invalid URL format for fass_link" });
        }

        // Check if URL is reachable
        const isReachable = await isValidUrl(fass_link);
        if (!isReachable) {
            return res.status(400).json({ success: false, message: "fass_link is not a valid or reachable website" });
        }

        // Ensure styrkor_doser is valid JSON
        try {
            JSON.parse(styrkor_doser);
        } catch (err) {
            return res.status(400).json({ success: false, message: "styrkor_doser must be a valid JSON string" });
        }

        // Insert into database (use "namn" to match your DB schema)
        const result = await addRecord("medicine", ["namn", "fass_link", "styrkor_doser"], [name, fass_link, styrkor_doser]);
        return res.status(201).json({ success: true, message: "Record successfully added", id: result.id });
    } catch (err) {
        if (err.message.includes("UNIQUE constraint failed")) {
            return res.status(409).json({ success: false, message: "Medicine name must be unique. The provided value already exists." });
        }
        if (err.message.includes("FOREIGN KEY constraint failed")) {
            return res.status(400).json({ success: false, message: "Invalid foreign key reference" });
        }
        if (err.message.includes("CHECK constraint failed")) {
            return res.status(400).json({ success: false, message: "Data does not meet required constraints" });
        }
        console.error("Error in /medicine/add route:", err);
        return res.status(500).json({ success: false, message: "Internal server error" });
    }
});

// ----------------------------- Get All Medicines -----------------------------
router.get("/all", async (req, res) => {
    try {
        const records = await getTable("medicine");
        return res.status(200).json({ success: true, records });
    } catch (err) {
        console.error("Error in /medicine/all route:", err);
        return res.status(500).json({ success: false, message: "Internal server error" });
    }
});

// ----------------------------- Get Medicine by ID -----------------------------
router.get("/:id", async (req, res) => {
    try {
        const record = await getRecordById("medicine", req.params.id);
        return res.status(200).json({ success: true, record });
    } catch (err) {
        console.error("Error in /medicine/:id route:", err);
        return res.status(err.status || 500).json({ success: false, message: err.message || "Internal server error" });
    }
});

// ----------------------------- Update Medicine -----------------------------
router.put("/update/:id", async (req, res) => {
    try {
        let { name, fass_link, styrkor_doser } = req.body;
        const updateData = {};

        if (name !== undefined) {
            if (typeof name !== "string") {
                return res.status(400).json({ success: false, message: "Invalid data type for name" });
            }
            updateData.namn = name;
        }

        if (fass_link !== undefined) {
            if (typeof fass_link !== "string") {
                return res.status(400).json({ success: false, message: "Invalid data type for fass_link" });
            }
            try {
                let urlObj = new URL(fass_link.startsWith("http") ? fass_link : "https://" + fass_link);
                urlObj.protocol = "https:";
                urlObj.host = "www.fass.se";
                if (!urlObj.searchParams.has("userType")) {
                    urlObj.searchParams.append("userType", "0");
                } else {
                    urlObj.searchParams.set("userType", "0");
                }
                fass_link = urlObj.toString();
            } catch (err) {
                return res.status(400).json({ success: false, message: "Invalid URL format for fass_link" });
            }
            const isReachable = await isValidUrl(fass_link);
            if (!isReachable) {
                return res.status(400).json({ success: false, message: "fass_link is not a valid or reachable website" });
            }
            updateData.fass_link = fass_link;
        }

        if (styrkor_doser !== undefined) {
            try {
                JSON.parse(styrkor_doser);
            } catch (err) {
                return res.status(400).json({ success: false, message: "styrkor_doser must be a valid JSON string" });
            }
            updateData.styrkor_doser = styrkor_doser;
        }

        await updateRecord("medicine", req.params.id, updateData);
        return res.status(200).json({ success: true, message: "Record updated successfully" });
    } catch (err) {
        console.error("Error in /medicine/update/:id route:", err);
        return res.status(err.status || 500).json({ success: false, message: err.message || "Internal server error" });
    }
});

// ----------------------------- Delete Medicine -----------------------------
router.delete("/delete/:id", async (req, res) => {
    try {
        const result = await deleteRecord("medicine", "id", req.params.id);
        return res.status(200).json({ success: true, message: result.message });
    } catch (err) {
        console.error("Error in /medicine/delete/:id route:", err);
        return res.status(err.status || 500).json({ success: false, message: err.message || "Internal server error" });
    }
});

module.exports = router;
