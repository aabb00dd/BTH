# 🛠️ helpers – Backend Utility Functions

This folder contains modular utility files used throughout the backend of the **Läkemedelsberäkningar** project. These helpers encapsulate core logic for manipulating question data, validating input, generating values, and preparing clean API responses.

---

## 📄 Module Descriptions

### [`courseHelpers.js`](./courseHelpers.js)
Handles parsing and validation of course-related data, especially `course_code`.  
**Key Functions:**
- `validateCourseCode(courseCode)` – Validates format and accepted patterns
- `normalizeCourseCode(courseCode)` – Converts course codes to a consistent casing/style

---

### [`medicineHelpers.js`](./medicineHelpers.js)
Manages medicine entries, especially for formatting strengths and accepted JSON values.  
**Key Functions:**
- `parseMedicineStrengths(styrkor_doser)` – Parses stored strengths (JSON or text)
- `formatStrengthOptions(...)` – Converts strength options into frontend-compatible lists

---

### [`questionHelpers.js`](./questionHelpers.js)
Core logic for dynamic question generation, evaluation, and validation.  
**Key Functions:**
- `calculateAnswer(formula, values)` – Evaluates the final numeric answer using variables
- `generateRandomQuestion(template)` – Randomizes variable values while respecting constraints
- `parseAnswerUnits()` – Validates and normalizes accepted unit inputs

---

### [`routeHelpers.js`](./routeHelpers.js)
Generic helpers used by API routes.  
**Key Functions:**
- `sendSuccess(res, data)` – Sends standardized success JSON
- `sendError(res, errorMessage)` – Sends standardized error JSON
- `wrapAsync(fn)` – Express middleware wrapper for async route handlers

---

### [`unitsHelpers.js`](./unitsHelpers.js)
Responsible for interpreting and validating units.  
**Key Functions:**
- `matchAcceptedUnits(userInput, accepted)` – Checks if user unit input matches accepted forms
- `standardizeUnitInput(input)` – Normalizes spacing, casing, and formatting for comparison

---

## ✅ Design Principles

- Functions are designed to be **pure** and **stateless** wherever possible.
- Code is **modularized by domain** (course, unit, question, etc.).
- Each helper is imported where needed to keep routes and models lean.

---

## 📌 Usage Example

```js
const { generateRandomQuestion, calculateAnswer } = require('./questionHelpers');

const question = generateRandomQuestion(template);
const correctAnswer = calculateAnswer(template.answer_formula, question.values);
