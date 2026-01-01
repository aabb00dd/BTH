const axios = require("axios");

// ----------------------------- Medicine Helper Functions -----------------------------
async function isValidUrl(url) {
    try {
        const response = await axios.head(url);
        return response.status >= 200 && response.status < 300;
    } catch (error) {
        return false;
    }
}

// Export all helper functions
module.exports = {
    isValidUrl
};
