# Backend – Läkemedels Hemsida

This is the backend for **Läkemedels Hemsida**, a web application aimed at generating and managing medical and course-related questions. It supports dynamic variable-based question generation, robust API endpoints for CRUD operations, and an extensible SQLite-based data layer.

---

## Folder Structure Overview

- **[`database`](./db/README.md)** – SQLite3 database setup and utilities
- **[`models`](./models/README.md)** – The database schema and definitions
- **[`routes`](./routes/README.md)** – The API calls and documentation
- **[`helpers`](./helpers/README.md)** – Helper modules for question generation, validation, and randomization
- **`/components`** – Server-side components like `RandomQuestion` responsible for generating randomized question instances

---

## Starting the Backend

Install dependencies:
```bash
npm install
```

Start the server:
```bash
npm start
```

---

## Key Features

### Dynamic Question Generation
- Variables in questions use the `%%variable%%` format
- JSON-defined `variating_values` enable randomization of numeric and text inputs
- Advanced rules via `condition` allow constraints between variables

### SQLite-Based Persistence
- Includes schemas for:
  - `units` (e.g., mg, ml)
  - `course` (e.g., DV1683 – AI in Python)
  - `medicine` (with strengths/doses in JSON)
  - `qtype` (question types with performance tracking)
  - `question_data` (main question bank)

### RESTful API Endpoints
- Fully documented routes for managing:
  - Courses
  - Medicines
  - Units
  - Question Types
  - Questions

---

## API Overview

You can explore the full [API Documentation](./routes/README.md), but here are some highlights:

- `GET /api/questions` – Fetch all questions
- `POST /api/questions` – Create a new question
- `GET /api/common/qtypes` – Fetch all question types
- `GET /api/common/units` – Fetch all unit types
- `POST /api/medicines` – Add a medicine
- `POST /api/courses` – Add a course

All endpoints return JSON responses.

---

## Example Question Object

```json
{
  "question": "Läkaren har ordinerat Morfin %%dosage%% mg x %%antal%% subcutant...",
  "answer_unit_id": 3,
  "answer_formula": "dosage / available_dose",
  "variating_values": {
    "dosage": [10, 15],
    "antal": [1, 2, 3],
    "available_dose": [10],
    "condition": "dosage > available_dose"
  },
  "course_code": "KM1424",
  "question_type_id": 2
}
```


## Known Issues / Limitations

Highlight current known rough edges:




