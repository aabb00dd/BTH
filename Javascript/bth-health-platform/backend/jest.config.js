module.exports = {
    rootDir: "..", // Move up one level from backend
    testMatch: ["<rootDir>/unit_tests/**/*.test.js"], // Match all test files inside unit_tests
    moduleDirectories: ["<rootDir>/backend/node_modules", "node_modules"],
    testEnvironment: "node",
  };
  