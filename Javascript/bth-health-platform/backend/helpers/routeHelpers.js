
// ----------------------------- General Fuctions -----------------------------
/**
 * Validates if a string is a valid JSON array.
 * @param {string} jsonString - The JSON string to validate.
 * @returns {boolean} - Returns true if valid, otherwise false.
 */
const isValidJsonArray = (jsonString) => {
    try {
        const parsed = JSON.parse(jsonString);
        return Array.isArray(parsed);
    } catch {
        return false;
    }
};


/**
 * Validates that a value is a positive integer.
 * @param {any} value - The value to check.
 * @returns {boolean} - Returns true if it's a valid positive integer.
 */
const isPositiveInteger = (value) => {
    return Number.isInteger(value) && value > 0;
};


/**
 * Selects a random element from an array.
 * @param {Array} array - The array to pick from.
 * @returns {any} - A random element from the array.
 */
const getRandomElement = (array) => {
    return array[Math.floor(Math.random() * array.length)];
};



// Export all helper functions
module.exports = {
    isValidJsonArray,
    isPositiveInteger,
    getRandomElement
};