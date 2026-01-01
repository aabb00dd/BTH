# Database

Thisfolder contains the database files and setup/update utilities for **Läkemedels Hemsida**.

---

## Files Overview

* [`initdatabase.js`](initdatabase.js) – *Returns connection to question\_data.db as db*
* [`dbCommands.js`](dbCommands.js) – *Runs the insert for the question\_data.db from the dict*
* [`insertDatabase.js`](insertDatabase.js)

---

### [`initdatabase.js`](./initdatabase.js)

Responsible for initializing the SQLite3 database and creating tables based on provided schema definitions.

#### Exports

* `db` – Database object connected to `question_data.db`

```js
initializeTable(tableName, columns)
```

#### Parameters

* `tableName` *(string)*: The name of the table to create.
* `columns` *(Array<{ name: string, type: string }>)*: List of column definitions.

  * `type`: The SQLite-compatible type declaration (e.g., `INTEGER PRIMARY KEY`, `TEXT NOT NULL`, etc.).

#### Error Handling

* If table creation fails (e.g. due to bad SQL syntax or DB lock), the function logs the error to the console using `console.error`.
* It does not throw or return structured errors — failures must be monitored via log output.

#### Example Console Output

```bash
Initialized table: course
Failed to initialize table: Syntax error near "question_data"
```

---

### [`dbCommands.js`](./dbCommands.js)

A command-line script that does a complete initialization of question_data.db using `initializeTable`, refreshing inconsistencies between the database and the dictionary.

#### What It Does
- Initializes the database connection
- Calls insert functions from `insertDatabase.js`:
  - `insertUnits()`
  - `insertQtypes()`
  - `insertCourses()`
  - `insertMedicines()`
  - `insertQuestions()`
- Clears the `question_data` table before reinserting
- Calls `updateNumQuestions()` from `questionModel.js`
- Closes the database connection when done


Run it with:

```bash
npm run db
```

---

### [`insertDatabase.js`](./insertDatabase.js)

This file populates the SQLite database `question_data.db` with default values using structured data from `database_dict.js`. It supports inserts and updates for all core entities (units, question types, courses, medicines, and questions).

---
**Functions**


* `initializeDatabase()`: Opens and Returns a connection to `question_data.db`
* `insertUnits()`: Asynchronously inserts/updates unit records using `tempDb.units` from `database_dict.js`




# **Formatering av frågor**

- Variabler omges av `%%variabel_namn%%`.
- Variabelnamn får endast innehålla:
  - **Bokstäver (A-Z, a-z)**
  - **Siffror (0-9)**
  - **Underscore (_)**
  - **Måste börja med en bokstav.**
  - *Inga mellanslag.*
- Specialhantering:
  - `%%name%%` och `%%namn%%` ersätts automatiskt med slumpmässiga namn från en lista.
  - `condition` används för att definiera mer avancerade regler för generering av variabler.

## **Formatering av varierande variabler**

### **Tal inom intervall**

- Anges med `[min, max]`, ex:

  ```json
  { "var1": [4, 16] }
  ```

  **Ger ett heltal mellan 4 och 16.**

### **Decimaltal eller heltal med steg**

- Format `[min, max, steg]`, ex:

  ```json
  { "var4": [1, 10, 0.1] }
  ```

  **Ger ett slumpmässigt värde mellan 1 och 10 med en decimal.**

- Exempel:

  ```json
  { "var5": [-10, 10, 0.5] }
  ```

  **Ger ett slumpmässigt värde mellan -10 och 10 med steg på 0.5.**

> **OBS!** `steg` måste vara mindre än `max - min`.

### **Välj från lista av siffror eller ord**

- Exempel:

  ```json
  { "var2": [20, 10] }
  ```

  **Ger antingen 20 eller 10.**
  
  > **OBS!** Om bara `två värden` anges måste det största anges först.
  
  > **OBS!** Om `tre värden` anges får det inte följa `[min, max, steg]`.

- Exempel med flera alternativ:

  ```json
  { "var3": [5, 10, 15, 20] }
  ```

  **Ger något av värdena 5, 10, 15 eller 20.**

- Exempel:

  ```json
  { "medicin": ["Medicin A", "Medicin B", "Medicin C"] }
  ```

  **Ger en av de specificerade medicinerna.**

## **Avancerade regler**

Vill du ha mer kontroll över hur variabler genereras kan du använda `condition`.

### **Villkor mellan variabler**

- Större än: `>`
- Mindre än: `<`
- Större eller lika med: `>=`
- Mindre eller lika med: `<=`

Exempel:

```json
{ "big_var": [10, 50], "small_var": [1, 10], "condition": "big_var > small_var" }
```

  > **OBS!** `big_var` måste alltid kunna vara större än `small_var`.


### Platshållare för databasuppslagning

För att dynamiskt hämta fält från andra tabeller i databasen stöds nu syntaxen:

```
%%table_name[index].field%%
```

- `table_name` är namnet på en tabell (t.ex. `medicine`).
- `index` är positionen i den lista av matchande rader (0-baserad).
- `field` är kolumnnamnet som ska hämtas.

### Exempel
Om variabla värden anger:
```json
"variating_values": "{\"medicine.namn\": [\"Morfin\"], \"antal\": [1,3]}"
```
kommer genereringssteget:
1. Hämta rad från `medicine` där `namn = 'Morfin'`.
2. Lagra hela raden som `medicine[0]`.
3. Ersätta `%%medicine[0].dosage%%`, `%%medicine[0].available_dose%%` etc.

### **Exempel JSON på en fullständig fråga med avancerade regler**

```json
    {
        "question": "Läkaren har ordinerat Morfin %%dosage%% mg x %%antal%% subcutant. Tillgängligt: Morfin %%available_dose%% mg/ml. Hur många ml motsvarar en enkeldos?",
        "answer_unit_id": 3,
        "answer_formula": "dosage / available_dose",
        "variating_values": "{'dosage' : [10,15], 'antal': [1,2,3], 'available_dose': [10], 'condition': 'dosage > 'avalible_dose'}",
        "course_code": "KM1424",
        "question_type_id": 2,
    }
```


## **Return from Randome Question **

| Column               | Typ  | Besktivning                                                                          |
|----------------------|------|--------------------------------------------------------------------------------------|
| **id** | int | UID |
| **question** | text | The string for the question. |
| **answer_unit_id** | int | The UID for the correct unit.|
| **answer_formula** | obj | A function for the fomula where the generated numbers can be used as ags to get the answer. |
| **variating_values** | text | A string in JSON format with the generated values in the form of a dictionary. |
| **question_type_id** | int | The UID for the question type. |
| **hint_id** | int | The UID for the hint. |

### **Exempel på en fråga i JSON format**

```json
{
    "question": "Omvandla %%var_name%%kg till %%var_name2%%g.",
    "answer_unit_id": "g",
    "answer_formula": "Func(a,b) -> a + b (var_name + var_name2)",
    "variating_values": "{ 'var_name': 55, 'var_name2': 15 }",
    "question_type_id": 1

}
```
