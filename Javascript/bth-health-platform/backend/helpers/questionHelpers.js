// backend/helpers/questionHelpers.js

// Get the database connection from questionRoutes
const sqlite3 = require("sqlite3").verbose();
const { db } = require("../db/initdatabase");

async function generateValues(question_data) {
  
  // console.log("Generating values for question:", question_data);

  // Adding a random name to the question text
  const questionText = addRandomName(question_data.question);
    
  // Get the variating values
  let variatingValues = JSON.parse(question_data.variating_values);

  // If it's still a string, parse again
  if (typeof variatingValues === "string") {
    try {
      variatingValues = JSON.parse(variatingValues);
    } catch (e) {
      console.error(`❌ Invalid double-encoded JSON in question ${row.id}`);
      variatingValues = {};
    }
  }
  
  variatingValues = JSON.parse(JSON.stringify(variatingValues));

  const { tableData, variables, condition } = parseVariatingValues(variatingValues);
  const resolvedData = await fetchTableValues(tableData);
 
  // Extract the links for the medicines 
  const medicineInfo = {};
  if (resolvedData.medicine) {
      for (const idx in resolvedData.medicine) {
          const med = resolvedData.medicine[idx];
          // med.namn is the name, med.fass_link is the link
          medicineInfo[med.namn] = med.fass_link;
      }
  }
  // console.log("Medicine info:", medicineInfo);


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

          allVariables[varName] = value; // Generate the value here later
      } else {
          // simple variable
          const valList = variables[varName];
          allVariables[varName] = valList;
      }
  }

  // -------------- Extracted all the variables --------------

  for (const variable in allVariables) {
    const entryData = allVariables[variable];
    // Check if the entryData is an array and needs generating
    if (Array.isArray(entryData)){
        generatedValue = generateFromArray(entryData);
        // console.log("Generated value for ", variable," (", allVariables[variable] , ") : ", generatedValue);
        allVariables[variable] = generatedValue;
    }

  }

  // console.log("question_text: ", questionText);
  // console.log("generatedValues: ", allVariables);
  // console.log("medicineInfo: ", medicineInfo);

  return {
    question_text: questionText,
    generatedValues: allVariables,
    medicineInfo: medicineInfo
  };
}

// // Helper to safely evaluate math expressions with context
// function safeEval(expr, context) {
//   try {
//     const keys = Object.keys(context);
//     const values = Object.values(context);

//     const fn = new Function(...keys, `
//       function round(val, decimals = 0) {
//         const factor = 10 ** decimals;
//         return Math.round(val * factor) / factor;
//       }
//       return ${expr};
//     `);

//     return fn(...values);
//   } catch (err) {
//     console.error("Error evaluating formula:", expr, err);
//     return null;
//   }
// }


/**
 * Calculate the numeric result of a formula like
 *   "medicine[0].dosage / medicine[0].available_dose"
 * given a map of values:
 *   { 'medicine[0].dosage': 10, 'medicine[0].available_dose': 5, ... }
 */
function calculateAnswer(formula, values) {
  // console.log("Calculating answer for formula:", formula);
  // console.log("Using values:", values);

  // 1) Replace every placeholder key in the formula with its numeric value
  let expr = formula;
  for (const [key, val] of Object.entries(values)) {
    const safeKey = escapeForRegExp(key);
    // word‐boundary around the placeholder so we don't clobber partial names
    const re = new RegExp(`\\b${safeKey}\\b`, "g");
    expr = expr.replace(re, val);
  }
  // console.log("Evaluable expression:", expr);

  // 2) Evaluate it
  let result;
  try {
    // new Function is a bit safer/cleaner than eval
    result = new Function(`"use strict"; return (${expr});`)();
  } catch (e) {
    console.error("❌ Error evaluating formula:", e, "expr:", expr);
    return null;
  }
  // console.log("Result: ", result);
  return result;
}

// Evaluates a condition string using current values
function evaluateCondition(condition, values) {
  condition = condition.replace(/([a-zA-Z_]+)/g, (match) => values[match] ?? match);
  try {
    return eval(condition) ? true : false;
  } catch (error) {
    console.error("Error evaluating condition:", error);
    return null;
  }
}

/**
 * Safely escape any RegExp metacharacters in a placeholder name
 */
function escapeForRegExp(str) {
  return str.replace(/[.*+?^${}()|[\]\\]/g, "\\$&");
}

// Replaces placeholders in a question with generated values
function formatQuestionText(question, generatedValues) {
    let q = question;
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

// Formats numbers in formulas
function formatFormula(formula) {
  return formula.replace(/\d+\.\d+/g, match => parseFloat(match).toFixed(2));
}

// Compares user input to correct value and unit
function checkAnswer(userAnswer, correctAnswer, correctUnits_obj, formula) {
  let correctUnits = correctUnits_obj?.accepted_answer;

  let  return_obj = {
    correct: false,
    formula: formatFormula(formula),
    message_type: null,
    message: null,
    additional_info: null
  };
  
  
  let precision = 2; // Default precision
  if (correctUnits_obj?.precision !== null){
    precision = correctUnits_obj?.precision;
  }


  if (typeof correctUnits === 'string') {
    try {
      correctUnits = JSON.parse(correctUnits);
    } catch (err) {
      console.error("Error parsing correctUnits:", err);
      
      return_obj["message_type"] = "Fel correctUnits format.";  
      return return_obj;
    }
  }

  if (!Array.isArray(correctUnits)) {
    correctUnits = correctUnits != null ? [correctUnits] : [];
  }

  const answerPattern = /^\s*([\d\s.,]+)\s*([a-zA-ZÅÄÖåäö.,\-+*()^_!?\d\s\/]*)?\s*$/;
  const match = userAnswer.match(answerPattern);
  let userValue = null;
  let userUnit = null;
  if (match) {
    userValue = match[1].replace(',', '.').replace(/\s+/g, '');
    userUnit = match[2] ? match[2].trim() : "";
    userValue = parseFloat(userValue);

    
    userUnit = userUnit.replace(/[^.,\s]/g, ''); // Remove whitespace, comma & dot
    userUnit = userUnit.toLowerCase(); // Convert to lowercase
  } 
  
  
  if (userValue === null || isNaN(userValue)) {
    return_obj["message_type"] = "Fel Format";
    return_obj["message"] = "Använd siffror följt av en enhet.";
    return_obj["additional_info"] = "Exempel: '5 kg', '5,4 kg', '5.4kg'";
    return return_obj;
  }

  // Calculate the correct rounded answer
  const factor = Math.pow(10, precision);
  const roundedCorrectAnswer = Math.round(correctAnswer * factor) / factor;

  // Check if the user value has the right precision
  if (userValue === roundedCorrectAnswer) {
    return_obj["correct"] = true;
    return_obj["message_type"] = "Rätt Svar";
    return_obj["message"] = "Rätt värde";
    return_obj["additional_info"] = `Ditt värde ${userValue}, är korrekt.`;
  } else {
    return_obj["message_type"] = "Fel Svar";
    return_obj["message"] =  "Fel värde";
    return_obj["additional_info"] = (precision === 0 && userValue % 1 !== 0) ? "Värdet är ett heltal." : `Ditt svar ${userValue}, är inte korrekt.`;
  }

  console.log("correctUnits: ", correctUnits);

  if (correctUnits.length > 0) {
    if (!correctUnits.includes(userUnit.trim())) {
      return_obj["message_type"] = "Fel Svar";
      if (return_obj["message_type"] === "Fel Svar") {
        return_obj["message"] += " & Fel Enhet";
        return_obj["additional_info"] += `\nGodkända enheter: ${correctUnits.join(', ')}`;
      } else if (return_obj["message_type"] === "Rätt Svar") {
        return_obj["correct"] = false;
        return_obj["message"] += "men Fel Enhet";
        return_obj["additional_info"] = `Godkända enheter: ${correctUnits.join(', ')}`;
      }
    } else {
      if (return_obj["message_type"] === "Fel Svar") {
        return_obj["message"] += "men Rätt Enhet";
      } else {
        return_obj["correct"] = true;
        return_obj["message"] = "Rätt Svar!";
        return_obj["additional_info"] = "Rätt värde & Rätt Enhet";
      }
    }
  }

  return return_obj;
}



// STEP 1: Helper to add name to question text
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

// STEP 2: Helper to parse variating_values
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


// STEP 3: Fetch values from DB for each entry in tableData
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


// STEP 4: Generate values from arrays
/**
 * Given an array, decide what kind of generation to do:
 *  - [min, max]                → random integer between min and max inclusive
 *  - [min, max, step]          → random number from min..max stepping by step
 *  - [a, b, c, …] or any mix   → pick one of the entries at random
 */
function generateFromArray(arr) {
  // guard
  if (!Array.isArray(arr) || arr.length === 0) return undefined;

  if (arr.length === 1) {
    // single value
    return arr[0];
  }

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

module.exports = {
  generateValues,
  formatQuestionText,
  checkAnswer,
  calculateAnswer
};
