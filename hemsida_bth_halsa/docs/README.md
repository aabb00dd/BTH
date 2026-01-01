from pathlib import Path

# Markdown content for GitHub Wiki
markdown_content = """
# Project Documentation

## `/backend/db`

> Folder for the database and updating/changing the SQLite DB

| File Name             | Description                                           | Imported By       | Notes                                                                 |
|----------------------|-------------------------------------------------------|-------------------|-----------------------------------------------------------------------|
| `question_temp.js`   | Frågedatabasen för läkemedelsberäkningar              |                   |                                                                       |
| `questions.json`     | Gammal frågedatabas                                   |                   | Kan tas bort?                                                         |
| `README.md`          | Beskriver hur man ska formatera frågorna              |                   | Behöver uppdateras. Lägg till hur man skapar/uppdaterar `.db`-filen. |
| `question_data.db`   | SQLite3-databas                                       |                   |                                                                       |
| `insertTempDatabase.js` | Skapar eller uppdaterar `.db`-filen för läkemedelsberäkningar |         | Koppla ihop skapandet av `.db` och `model.js`                         |

---

## `/backend/helpers`

> Hjälpfunktioner för routes  
> Naming convention: `%%name%%Helpers.js` → `../routes/%%name%%Routes.js`

| File Name             | Description             | Imported By        |
|----------------------|-------------------------|--------------------|
| `courseHelpers.js`   |                         | `courseRoutes.js`  |
| `medicineHelpers.js` |                         | `medicineRoutes.js`|
| `routeHelpers.js`    |                         | `questionRoutes.js`|
| `unitsHelpers.js`    |                         | `unitsRoutes.js`   |

---

## `/backend/models`

> Models for handling database interaction

| File Name           | Description                                   | Imported By          | Notes                   |
|--------------------|-----------------------------------------------|----------------------|-------------------------|
| `questionModel.js` | Kräver `../db/`-folder                         | Multiple route files |                         |
| `userModel.js`     | Temporär fil för användardata                 |                      |                         |

---

## `/backend/routes`

> Frontend-backend connection.  
> Naming convention: `%%name%%Routes.js` (where `name = table_name` in `question_data.db`)

| File Name           | Route Examples                                     | Description                            |
|--------------------|----------------------------------------------------|----------------------------------------|
| `commonRoutes.js`  | GET `/units/all`, `/course/all`, `/medicine/all`  |                                        |
| `courseRoutes.js`  | POST `/course/add`                                 |                                        |
| `medicineRoutes.js`| POST `/medicine/add`                               |                                        |
| `qtypeRoutes.js`   | POST `/qtype/add`                                  |                                        |
| `questionRoutes.js`| POST `/question/add`, `/question/check-answer`<br>GET `/question/random` |      |
| `unitsRoutes.js`   | POST `/units/add`                                  |                                        |
| `README.md`        | Första utkastet av API-dokumentation              | Behöver ses över och förbättras        |
"""

# Save to file
output_path = Path("/mnt/data/project-docs.md")
output_path.write_text(markdown_content.strip(), encoding='utf-8')
output_path
