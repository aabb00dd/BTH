# Backend Overhaul Summary: Dynamic Question Resolution

This document outlines the full set of backend changes implemented to support dynamic placeholder substitution, formula-based value generation, and structured response formatting.

---

## Core Functional Changes

### 1. Generalized Placeholder Resolution
- Supports dynamic placeholders of the form:
  ```
  %%table_name[index].field%%
  ```
- Example:
  ```
  %%medicine[0].dosage%%
  ```

---

### 2. `variating_values` Structure
- Replaced `styrkor_doser` with a unified JSON format.
- Example:
  ```json
  {
    "medicine.namn": ["Morfin"],
    "age": [3, 14],
    "weight": "formula(child_weight(age))"
  }
  ```

---

### 3. Dynamic DB Lookups
- For placeholders like `%%medicine[0].dosage%%`:
  - Use the name from `"medicine.namn": [...]`
  - Query the medicine table:
    ```sql
    SELECT variating_values FROM medicine WHERE namn = 'Morfin'
    ```
  - Extract the needed field from the JSON

---

### 4. Formula Evaluation
- Parse and evaluate formulas such as:
  ```json
  "weight": "formula(child_weight(age))"
  ```
- Evaluates using:
  - Formula definitions from `database_dict.js`
  - Resolved values from `variating_values`
- Supports expressions like:
  ```
  round(age * 2.5 + 8)
  ```

---

### 5. Index Out-of-Range Handling
- If the index in a placeholder exceeds available values:
  - Log a warning
  - Replace with:
    ```
    [ERROR:medicine[3].dosage]
    ```

---

## Route & Helper Refactor

### `/random` Route
- Accepts `course_code` and `count`
- Returns either:
  - A single parsed question object (if `count === 1`)
  - A list of parsed question objects
- Uses:
  ```js
  const parsed = await generateParsedQuestionFromTemplate(row);
  ```

### `generateParsedQuestionFromTemplate(row)`
- Accepts full DB row
- Parses:
  - `row.question` and/or `row.preamble`
  - `row.variating_values`
- Generates all values:
  - From database lookups
  - From static/ranged values
  - From formulas
- Adds `generated_values` to each `question_data` entry

---

## Implemented Checklist (from handwritten plan)

- [x] 4D – Create question list
- [x] 4D.1 – Send index 0 if frontend can’t handle a list
- [x] 4D.2 – Do value generation
- [x] 4C – Fix value generation
  - [x] 4C.1 – Handle formula
  - [x] 4C.2 – Handle `table.column`
  - [x] 4C.3 – Handle `%%table[index]%%`
  - [x] 4C.4 – Handle `%%table[index].column%%`
- [x] 4E – Send medicine name and fass link
- [x] 4B – Shuffle order
- [x] 4A – Validate question count

---

This implementation lays the foundation for dynamically resolving structured medical question templates with precise database integration and formula support.
