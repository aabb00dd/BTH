# Medical Dosage Calculations – Project Overview

## tl;dr
This is a **school project** developed as part of coursework at **Blekinge Institute of Technology (BTH)**.  
It is a web application designed for **nursing students** to practice different types of medical dosage calculations, with support for dynamic questions, automatic answer validation, and basic statistics handling.

---

## Project Background

This project was created for educational purposes within a university course.  
The goal is to support learning and training in medical dosage calculations by providing an interactive, practice-oriented tool.

The application is **not intended for clinical use** and should only be used in an academic or learning context.

---

## Folder Structure Overview

- **[`backend`](./backend/README.md)**  
  Node.js server responsible for API endpoints, database operations, and dynamic question generation.

- **[`frontend`](./frontend/README.md)**  
  React-based user interface for presenting questions, submitting answers, and giving feedback.

- **[`docs`](./docs/README.md)**  
  Automatically generated documentation (work in progress).

- **[`unit_tests/backend`](./unit_tests/backend)**  
  Unit and integration tests for backend logic, including validation and edge cases.

- **[`instructions_new_code`](./instructions_new_code)**  
  Technical documentation and setup instructions for new backend contributions, including FASS integration.

- **[`notes`](./notes)**  
  Design sketches, planning documents, and internal developer notes.

- **[`resuser`](./resuser/README.md)**  
  External resources and supporting files.

---

## Known Issues / Limitations

This project is still under development.  
Some features may be incomplete, experimental, or subject to change as part of the learning process.

---

## Git Workflow

- `live`: Currently deployed version  
- `deployment`: Tested and approved code ready for deployment  
- `main`: Clean, stable version intended to be deployable  
- `name`: Local development branch for an individual developer  

Always branch from `main` unless a specific version is required.

Pull requests for new features or changes should be made into the `main` branch for review.

For detailed instructions on Git and GitHub usage in this project, see the  
[Git Commands Guide](./notes/github/README.md).

---

## Testing

The project uses **Jest** for unit and integration testing of backend components.

To run tests:

```bash
npm test
