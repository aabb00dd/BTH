// ----------------------------- Units Helper Fuctions -----------------------------

/**
 * Validates the 'accepted_answer' field to ensure it is a JSON array of strings.
 * This function attempts to parse the input string as JSON and checks whether the resulting
 * value is an array where every element is a string. If parsing fails or if any element
 * is not a string, the function returns false.
 *
 * @param {string} acceptedAnswer - The accepted_answer field as a JSON-encoded string array.
 * @returns {boolean} - Returns true if the input is a valid JSON array of strings, otherwise false.
 */
const validateAcceptedAnswer = (acceptedAnswer) => {
    try {
        const parsed = JSON.parse(acceptedAnswer);
        return Array.isArray(parsed) && parsed.every(item => typeof item === "string");
    } catch {
        return false;
    }
};

// Export all helper functions
module.exports = {
    validateAcceptedAnswer
};