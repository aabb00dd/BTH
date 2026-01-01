const sqlite3 = require("sqlite3").verbose();
const path = require("path");

const dbPath = path.resolve(__dirname, "question_data.db");
const db = new sqlite3.Database(dbPath);

// STEP 1: Helper to parse variating_values
function parseVariatingValues(variatingValues) {
  const tableData = {};
  const variables = {};
  let condition = null;

  for (const key in variatingValues) {
    if (key === "condition") {
      condition = variatingValues[key];
      continue;
    }

    const valueArray = variatingValues[key];
    const dotParts = key.split(".");
    if (dotParts.length === 2) {
      const [table, field] = dotParts;
      if (!tableData[table]) tableData[table] = {};

      valueArray.forEach((val, idx) => {
        if (!tableData[table][idx]) tableData[table][idx] = {};
        tableData[table][idx][field] = val;
    });
       
    } else {
      variables[key] = valueArray;
    }
  }

  return { tableData, variables, condition };
}

function addRandomName(questionText) {
  const namesList = [
    "Alice", "Bob", "Charlie", "Diana", "Elias", "Frida",
    "Gustav", "Hanna", "Isak", "Jasmine", "Kasper", "Lina",
    "Mikael", "Nora", "Oskar", "Petra", "Quentin", "Rebecca",
    "Simon", "Tove", "Ulf", "Vera", "Wilhelm", "Xenia",
    "Yasmine", "Zack"
  ];

  const randomName = namesList[Math.floor(Math.random() * namesList.length)];
  return questionText.replace(/%%(.*?)%%/g, (fullMatch, key) => {
    if (key === "name" || key === "namn") {
      return randomName;
    }
    return fullMatch;
  });
}

// STEP 2: Fetch values from DB for each entry in tableData
async function fetchTableValues(tableData) {
  const filledTableData = {};

  for (const table in tableData) {
    filledTableData[table] = {};

    for (const index in tableData[table]) {
      const queryFields = tableData[table][index];
      const whereClause = Object.entries(queryFields)
        .map(([key, val]) => `${key} = ?`)
        .join(" AND ");
      const values = Object.values(queryFields);
      const sql = `SELECT * FROM ${table} WHERE ${whereClause} LIMIT 1`;

      const row = await new Promise((resolve) => {
        db.get(sql, values, (err, result) => {
          if (err) {
            console.error(`❌ Error querying ${table}[${index}]`, err.message);
            return resolve(null);
          }
          resolve(result);
        });
      });

      if (row) {
        filledTableData[table][index] = row;
        if(row.variating_values) {
            let var_values = row.variating_values;
            if (typeof var_values === "string") {
                try { var_values = JSON.parse(var_values); }
                catch (e) { console.error("⚠️ Failed parsing variating_values:", e); }
            }
            
            for (const [k, v] of Object.entries(var_values || {})) {
                row[k] = v;
            }
            delete row.variating_values;
        }
        

        filledTableData[table][index] = row;
        //console.log("Row:  ", row);
      } else {
        console.error(`⚠️ No match for ${table}[${index}]`, queryFields);
        filledTableData[table][index] = { __error__: true };
      }
    }
  }

  return filledTableData;
}

// STEP 3: Run test across all questions
db.all(`SELECT id, question, variating_values FROM question_data`, async (err, rows) => {
  if (err) {
    console.error("Failed to load questions:", err.message);
    return;
  }

  for (const [index, row] of rows.entries()) {
    console.log(`\n===== Question ${index + 1} (ID ${row.id}) =====`);
    try {
        // Print the question text
        // const questionTextNoName = row.question;
        // console.log("Question Text:", questionTextNoName);

        // Choose and repalce a random name
        questionText = addRandomName(row.question);
        console.log("Question Text:", questionText);


        // Get the variating values
        let variatingValues = JSON.parse(row.variating_values);

        // If it's still a string, parse again
        if (typeof variatingValues === "string") {
            try {
                variatingValues = JSON.parse(variatingValues);
            } catch (e) {
                console.error(`❌ Invalid double-encoded JSON in question ${row.id}`);
                continue;
            }
        }

        const { tableData, variables, condition } = parseVariatingValues(variatingValues);
        //console.log("Parsed Variables:", variables);

        // Fetch values that need to be called from other tables
        const resolvedData = await fetchTableValues(tableData);
        // console.log("Parsed tableData:", resolvedData);


        // Extract the links for the medicines 
        const medicineInfo = {};
        if (resolvedData.medicine) {
            for (const idx in resolvedData.medicine) {
                const med = resolvedData.medicine[idx];
                // med.namn is the name, med.fass_link is the link
                medicineInfo[med.namn] = med.fass_link;
            }
        }
        console.log("Medicine info:", medicineInfo);

        // Extract variable names from the question text
        const variableMatches = questionText.match(/%%(.*?)%%/g) || [];
        const uniqueMatches = Array.from(new Set(variableMatches));
        const variableNames = uniqueMatches.map(m => m.slice(2, -2));

        let allVariables = {};

        // Sort the variable names and tables
        for (const varName of variableNames) {
        const tableRefRe = /^(\w+)\[(\d+)\]\.(\w+)$/;
        const varMatch = varName.match(tableRefRe);
        if (varMatch) {
                const [, table, idxStr, field] = varMatch;
                const idx = parseInt(idxStr, 10);

                // grab the resolved value (or mark error)
                const value =
                resolvedData?.[table]?.[idx]?.[field] ?? `[ERROR:${table}[${idx}].${field}]`;

                allVariables[varName] = value;
            } else {
                // simple variable
                const valList = variables[varName];
                allVariables[varName] = valList;
            }
        }

      
        if (condition) console.log("Condition:", condition);

        for (const variable in allVariables) {
            const entryData = allVariables[variable];

            // Check if the entryData is an array and needs generating
            if (Array.isArray(entryData)){
                generatedValue = generateFromArray(entryData);
                console.log("Generated value for ", variable," (", allVariables[variable] , ") : ", generatedValue);
                allVariables[variable] = generatedValue;
            }
        
        }

        console.log("All variables:", allVariables);

        console.log("Final question text:", formatQuestionText(questionText, allVariables));

    } catch (parseError) {
      console.error(`❌ Error parsing or resolving question ${row.id}:`, parseError.message);
    }
  }

  db.close();
});



function formatQuestionText(question, generatedValues) {
    let q = question;
    function escapeForRegExp(str) {
      return str.replace(/[.*+?^${}()|[\]\\]/g, "\\$&");
    }
    for (const [key, rawValue] of Object.entries(generatedValues)) {
        // skip any internals
        if (key.startsWith("__")) continue;

        // escape the key so that ".", "[", "]" etc. match literally
        const safeKey = escapeForRegExp(key);
        const regex   = new RegExp(`%%${safeKey}%%`, "g");
        q = q.replace(regex, rawValue);
    }
    return q;
}


/**
 * Given an array, decide what kind of generation to do:
 *  - [min, max]                → random integer between min and max inclusive
 *  - [min, max, step]          → random number from min..max stepping by step
 *  - [a, b, c, …] or any mix   → pick one of the entries at random
 */
function generateFromArray(arr) {
  // guard
  if (!Array.isArray(arr) || arr.length === 0) return undefined;

  // all numbers?
  const allNum = arr.every(x => typeof x === "number");

  if (allNum && arr.length === 2 && arr[0] < arr[1]) {
    // integer range
    const [min, max] = arr;
    const low = Math.ceil(min), high = Math.floor(max);
    return Math.floor(Math.random() * (high - low + 1)) + low;
  }

  if (allNum && arr.length === 3 && arr[0] < arr[1] && (arr[1] - arr[0]) > arr[2]) {
    // range with step
    const [min, max, step] = arr;
    const count = Math.floor((max - min) / step) + 1;
    const idx = Math.floor(Math.random() * count);
    const val = min + idx * step;
    // to avoid floating-point quirks, round to the same decimals as step:
    const decimals = (step.toString().split(".")[1] || "").length;
    return +(val.toFixed(decimals));
  }

  // otherwise: pick one element (could be numbers or strings)
  return arr[Math.floor(Math.random() * arr.length)];
}