# 🧠 **Läkemedelsberäkningar** – Project Overview

## 📌 tl;dr
En webbapplikation för sjuksköterskestudenter vid BTH för att öva på olika läkemedelsberäkningar – med stöd för dynamiska frågor, automatiska svarskontroller och statistikhantering.

---

## 📁 Folder Structure Overview

- **📦 [`backend`](./backend/README.md)**  
  Node.js server handling API endpoints, database operations, and dynamic question generation.

- **📦 [`frontend`](./frontend/README.md)**  
  React-based user interface for displaying questions, submitting answers, and providing feedback.

- **🚧 [`docs`](./docs/README.md)**  
  Automated documentation (WIP).

- **📦 [`unit_tests/backend`](./unit_tests/backend)**  
  Unit and integration tests for backend logic, including edge cases and validation.

- **📦 [`instructions_new_code`](./instructions_new_code)**  
  Technical documentation and setup instructions for new backend contributions, including FASS integration.

- **📦 [`notes`](./notes)**  
  Design sketches, planning materials, and internal developer notes.

- **📦 [`resuser`](./resuser/README.md)**  
  External resources and files.

---

## 📌 Known Issues / Limitations

Highlight current known rough edges:


## 🧭 Git Workflow

- `live`: The currently deployed version
- `deployment`: This should be code that is tested ready and approved to be deployed when possible
- `main`: Intended to reflect a clean, deployable version (Before deployment but only completed tested and aproved changed)
- `name`: Local dev base for developer *name* to work with

Always branch off from `main` unless you want a certain version.

Pull requests should be made into the `main` branch for new features that are up for review.

For detailed instructions on using Git and GitHub in this project, see the [Git Commands Guide](./notes/github/README.md).

---

## 🔬 Testing
Vi använder **Jest** för enhetstester och integrationstester av backend-komponenter.

To run tests:
```bash
npm test
```
