# Database Utilities for Läkemedels Hemsida

This folder contains the core database models and utility functions for the [databases](./../db).

---

# Databases
| **Model** | **Database File**| **Info** |
| ----- | ---- | ----- | 
|[`questionModel.js`](./questionModel.js) | [`question_data.db`](./../db/question_data.db)| [Question Database](#-question-database) |
|[`feedbackModel.js`](./feedbackModel.js) | [`question_data.db`](./../db/question_data.db)| [Feedback Database](#-feedback-database) |
|[`userModel.js`](./userModel.js) | [`missing`](#-databases)| 🚧[WIP](#-databases) |

# Question Database

### [`questionModel.js`](./questionModel.js) -> [`question_data.db`](./../db/question_data.db)
---
## 🛠 Functions: 
```js
addRecord(table, columns, values)
getTable(table)
getRecordById(table, id)
getRecordWhere(table, column, value)
updateRecord(table, id, updates)
deleteRecord(table, column, value)
```
---
## Database Schema


| Column            | Type    | Notes                                                      |
| ----------------- | ------- | ---------------------------------------------------------- |
| ***Table***         |       |  **units**                                             |
| `id`              | INTEGER | Primary key                                                |
| `ascii_name`      | TEXT    | Must be unique                                             |
| `accepted_answer` | TEXT    | JSON-encoded array, validated via `CHECK(json_valid(...))` |
|                                                                                          |
| ***Table***| |**course**        |        
| `course_code`    | TEXT | Primary key                                                    |
| `question_types` | TEXT | JSON-encoded list of allowed question type IDs                 |
|                                                                                          |
| ***Table***| |**medicine**     |         
| `id`            | INTEGER | Primary key                                |
| `namn`          | TEXT    | Unique medication name                     |
| `styrkor_doser` | TEXT    | JSON-encoded list of dose/strength options |
|                                                                                          |
| ***Table***| |**qtype**     |       
| `id`            | INTEGER | Primary key                            |
| `description`   | TEXT    | Human-readable type label              |
| `right_answers` | INTEGER | Tracks number of correct answers       |
| `wrong_answers` | INTEGER | Tracks number of incorrect answers     |
| `history_json`  | TEXT    | Optional performance history (as JSON) |
|                                                                                        |
| ***Table***| |**question_data**     |         | ***New Table***                                            
| `id`               | INTEGER | Primary key                              |
| `question`         | TEXT    | Question text with variable placeholders |
| `answer_unit_id`   | INTEGER | Foreign key to `units`                   |
| `answer_formula`   | TEXT    | Formula to calculate correct answer      |
| `variating_values` | TEXT    | JSON with ranges, constraints, etc.      |
| `course_code`      | TEXT    | Foreign key to `course`                  |
| `question_type_id` | INTEGER | Foreign key to `qtype`                   |
| `hints`            | TEXT    | JSON list of hints (optional)            |

---

## Special Logic

* *updateNumQuestions()* keeps course.num_questions synced with actual data.
* *handleInsertError()* provides readable responses for foreign key and unique constraint violations.
* JSON columns are validated in table schema to prevent corrupt entries.

The [`questionModel.js`](./questionModel.js) acts as the foundation for all backend data transactions regarding the [`quetion_data.db`](./../db/question_data.db).

---

# Feedback Database
