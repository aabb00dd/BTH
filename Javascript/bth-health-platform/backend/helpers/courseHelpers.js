// backend/helpers/courseHelpers.js
/**
 * Validates the 'course_code' field to ensure it follows the pattern XX0000.
 * @param {string} courseCode - The course code to validate.
 * @returns {boolean} - Returns true if valid, otherwise false.
 */
const validateCourseCode = (courseCode) => {
    return /^[A-Z]{2}\d{4}$/.test(courseCode);
};

/**
 * Validates the 'question_types' field to ensure it is a JSON array of existing qtype names.
 * @param {string} questionTypes - The question_types field as a JSON string.
 * @returns {boolean} - Returns true if valid, otherwise false.
 */
const validateQuestionTypes = async (questionTypes) => {
    try {
        const parsed = JSON.parse(questionTypes);
        // Make sure it's an array of strings.
        if (!Array.isArray(parsed) || !parsed.every(item => typeof item === "string")) return false;
        
        // Check if each name exists in the qtype table.
        // Use getTable from questionModel.
        const { getTable } = require("../models/questionModel");
        const existingTypes = await getTable("qtype");
        const existingNames = new Set(existingTypes.map(q => q.name));
        return parsed.every(name => existingNames.has(name));
    } catch {
        return false;
    }
};

// Export the helper functions.
module.exports = {
    validateCourseCode,
    validateQuestionTypes
};
