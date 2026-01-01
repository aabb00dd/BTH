
# Answer Validation API Design

## Endpoint

**POST** `/question/validate`

---

## Purpose

Validate a user's submitted answer against the generated values from a question template.

---

## Request JSON

```json
{
  "question_id": 42,
  "user_answer": "morfin",
  "question_data_index": 0
}
```

- `question_id`: ID of the question that was answered
- `user_answer`: Answer input from the user
- `question_data_index`: If there are multiple subquestions (e.g., part a/b/c)

---

## Response JSON

```json
{
  "correct": true,
  "message": "Correct answer!",
  "expected": "morfin",
  "accepted_answers": ["morfin", "morfinn", "morfn", "..."]
}
```

- `correct`: Boolean indicating if the answer was accepted
- `message`: Feedback string
- `expected`: Canonical correct answer (from `generated_values.answer`)
- `accepted_answers`: Array of valid accepted strings (e.g., typo variants, alternate spellings)

---

## Validation Logic

- Convert `user_answer` to lowercase
- Match against `accepted_answers`
- For numeric answers, consider:
  - Rounded equivalence (e.g., 2.0 == 2)
  - Unit normalization (e.g., g == 1000 mg)
- Feedback categories:
  - ✅ Correct
  - ⚠️ Close match (e.g., wrong unit, off by factor of 10)
  - ❌ Incorrect

---

## State Management

- Option 1: Store `generated_values` in session and retrieve using `question_id`
- Option 2: Accept `generated_values` in the validation request body (stateless validation)
- Option 3: Re-generate using deterministic seed

---

## TODO

- [ ] Implement `/question/validate` route in `routes/question.js`
- [ ] Add support for session storage or deterministic re-generation
- [ ] Write tests for:
  - Exact match
  - Case-insensitive match
  - Misspellings
  - Numeric equivalence
  - Unit errors


---

## Additional Metadata

The system should also track the following per validation:

### Request Additions

```json
{
  "question_id": 42,
  "user_answer": "morfin",
  "question_data_index": 0,
  "user_uid": "abc123",
  "timestamp": "2025-05-12T08:37:00Z"
}
```

- `user_uid`: A unique identifier for the user (from authentication session)
- `timestamp`: ISO 8601 UTC timestamp of submission

### Purpose

- Enables tracking user performance over time
- Useful for audit logs, retry history, or saving results to DB
- Supports replay/debugging of session-based answer flow

---

## Updated TODOs

- [ ] Save `user_uid`, `timestamp`, and `question_id` with result
- [ ] Extend validation model or database schema (if persistent)
- [ ] Confirm timestamp is generated server-side if not passed
