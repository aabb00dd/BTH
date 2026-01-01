const express = require("express");
const router = express.Router();
const { 
    addRecord, 
    getTable, 
    updateRecordByKey, 
    deleteRecord 
} = require("../models/questionModel");
const { validateCourseCode, validateQuestionTypes } = require("../helpers/courseHelpers");

// ----------------------------- Get All Courses -----------------------------
/**
 * @route GET /course/all
 * @desc Retrieves all course records.
 */
router.get("/all", async (req, res) => {
    try {
        const records = await getTable("course");
        res.status(200).json({ success: true, records });
    } catch (err) {
        res.status(err.status || 500).json({ success: false, message: err.message || "Error retrieving records" });
    }
});

// ----------------------------- Add Course -----------------------------
/**
 * @route POST /course/add
 * @desc Adds a new course record to the database.
 */
router.post("/add", async (req, res) => {
    try {
        let { course_code, course_name, question_types } = req.body;
        if (!course_code || !course_name || !question_types) {
            return res.status(400).json({ success: false, message: "Missing required fields" });
        }

        course_code = course_code.toUpperCase(); // Convert to uppercase before validation and storage

        if (!validateCourseCode(course_code)) {
            return res.status(400).json({ success: false, message: "Invalid course_code format. Must be two uppercase letters followed by four digits (e.g., CS1001)." });
        }
        if (!await validateQuestionTypes(question_types)) {
            return res.status(400).json({ success: false, message: "Invalid question_types format or contains non-existent qtype IDs." });
        }
        
        try {
            const result = await addRecord("course", ["course_code", "course_name", "question_types"], [course_code, course_name, question_types]);
            res.status(201).json({ success: true, message: "Record successfully added", id: result.id });
        } catch (err) {
            if (err.message && err.message.includes("UNIQUE constraint failed")) {
                return res.status(409).json({ success: false, message: "course_code must be unique. The provided value already exists." });
            }
            throw err;
        }
    } catch (err) {
        res.status(err.status || 500).json({ success: false, message: err.message || "Error adding record" });
    }
});

// ----------------------------- Update Course -----------------------------
/**
 * @route PUT /course/update
 * @desc Updates an existing course record.
 * @param {string} original_course_code - The current course code (to locate the record).
 * @param {string} [course_code] - The new course code (optional).
 * @param {string} [course_name] - The new course name (optional).
 * @param {string} [question_types] - The updated question types as a JSON string (optional).
 */
router.put("/update", async (req, res) => {
    try {
        let { original_course_code, course_code, course_name, question_types } = req.body;
        if (!original_course_code) {
            return res.status(400).json({ success: false, message: "Original course_code is required" });
        }
        
        // If a new course_code is provided, validate it; otherwise, default to original.
        course_code = course_code ? course_code.toUpperCase() : original_course_code;
        if (!validateCourseCode(course_code)) {
            return res.status(400).json({ success: false, message: "Invalid course_code format. Must be two uppercase letters followed by four digits (e.g., CS1001)." });
        }
        if (question_types && !await validateQuestionTypes(question_types)) {
            return res.status(400).json({ success: false, message: "Invalid question_types format or contains non-existent qtype IDs." });
        }
        
        // Build the updates object with provided fields.
        const updates = {};
        if (course_code) updates.course_code = course_code;
        if (course_name) updates.course_name = course_name;
        if (question_types) updates.question_types = question_types;
        
        await updateRecordByKey("course", "course_code", original_course_code, updates);
        res.status(200).json({ success: true, message: "Course updated successfully" });
    } catch (err) {
        res.status(err.status || 500).json({ success: false, message: err.message || "Error updating course" });
    }
});

// ----------------------------- Delete Course -----------------------------
/**
 * @route DELETE /course/delete
 * @desc Deletes a course record from the database.
 * @param {string} course_code - The course code of the record to be deleted.
 */
router.delete("/delete", async (req, res) => {
    try {
        const { course_code } = req.body;
        if (!course_code) {
            return res.status(400).json({ success: false, message: "course_code is required" });
        }
        await deleteRecord("course", "course_code", course_code);
        res.status(200).json({ success: true, message: "Course deleted successfully" });
    } catch (err) {
        res.status(err.status || 500).json({ success: false, message: err.message || "Error deleting course" });
    }
});

// ----------------------------- Export Routes -----------------------------
module.exports = router;
