// questionRoutes.test.js
const request = require("../../../backend/node_modules/supertest");
const express = require("../../../backend/node_modules/express");

// Import the router to test
const router = require('../../../backend/routes/questionRoutes');

// ----------------------
// Mock dependencies
// ----------------------
jest.mock('../../../backend/models/questionModel', () => ({
  addRecord: jest.fn(),
  getTable: jest.fn(),
  getRecordWhere: jest.fn(),
  getRecordById: jest.fn()
}));

jest.mock('../../../backend/helpers/questionHelpers', () => ({
  generateValues: jest.fn(),
  formatQuestionText: jest.requireActual('../../../backend/helpers/questionHelpers').formatQuestionText,
  checkAnswer: jest.requireActual('../../../backend/helpers/questionHelpers').checkAnswer
}));

// We'll also override getRandomElement from routeHelpers to control randomness
const routeHelpers = require('../../../backend/helpers/routeHelpers');
routeHelpers.getRandomElement = jest.fn();

// Import the mocked models and helpers for use in tests
const questionModel = require('../../../backend/models/questionModel');
const questionHelpers = require('../../../backend/helpers/questionHelpers');

// Setup Express app for testing
const app = express();
app.use(express.json());
app.use('/question', router);

describe("Exhaustive tests for POST /question/add", () => {
  beforeEach(() => {
    jest.resetAllMocks();
  });

  test("should return 400 if missing required fields", async () => {
    const res = await request(app).post('/question/add').send({});
    expect(res.status).toBe(400);
    expect(res.body.success).toBe(false);
    expect(res.body.message).toMatch(/Missing required fields/);
  });

  test("should return 400 if answer_unit_id or question_type_id are invalid", async () => {
    const payload = {
      question: "Test question?",
      answer_unit_id: -1, // Invalid
      answer_formula: "1+1",
      variating_values: JSON.stringify([]),
      course_code: "test",
      question_type_id: 1
    };
    const res = await request(app).post('/question/add').send(payload);
    expect(res.status).toBe(400);
    expect(res.body.success).toBe(false);
    expect(res.body.message).toMatch(/Invalid answer_unit_id or question_type_id format/);
  });

  test("should return 400 if variating_values is not a valid JSON array", async () => {
    const payload = {
      question: "Test question?",
      answer_unit_id: 1,
      answer_formula: "1+1",
      // Although this is valid JSON, it is not an array.
      variating_values: JSON.stringify({ not: "an array" }),
      course_code: "test",
      question_type_id: 1
    };
    const res = await request(app).post('/question/add').send(payload);
    expect(res.status).toBe(400);
    expect(res.body.message).toMatch(/variating_values must be a valid JSON array/);
  });

  test("should convert course_code to uppercase before adding", async () => {
    questionModel.addRecord.mockResolvedValue({ id: 2 });
    const payload = {
      question: "Test question?",
      answer_unit_id: 1,
      answer_formula: "1+1",
      variating_values: JSON.stringify([]),
      course_code: "lowercase",
      question_type_id: 1
    };
    await request(app).post('/question/add').send(payload);
    // Verify that addRecord was called with an uppercase course_code.
    expect(questionModel.addRecord).toHaveBeenCalledWith(
      "question_data",
      ["question", "answer_unit_id", "answer_formula", "variating_values", "course_code", "question_type_id"],
      expect.arrayContaining(["Test question?", 1, "1+1", JSON.stringify([]), "LOWERCASE", 1])
    );
  });

  test("should handle FOREIGN KEY constraint failure error", async () => {
    questionModel.addRecord.mockRejectedValue({ message: "FOREIGN KEY constraint failed" });
    const payload = {
      question: "Test question?",
      answer_unit_id: 99, // Non-existing
      answer_formula: "1+1",
      variating_values: JSON.stringify([]),
      course_code: "TEST",
      question_type_id: 99
    };
    const res = await request(app).post('/question/add').send(payload);
    expect(res.status).toBe(400);
    expect(res.body.message).toMatch(/Invalid foreign key reference/);
  });

  test("should handle generic database errors during add", async () => {
    questionModel.addRecord.mockRejectedValue({ message: "Some DB error", status: 500 });
    const payload = {
      question: "Test question?",
      answer_unit_id: 1,
      answer_formula: "1+1",
      variating_values: JSON.stringify([]),
      course_code: "TEST",
      question_type_id: 1
    };
    const res = await request(app).post('/question/add').send(payload);
    expect(res.status).toBe(500);
    expect(res.body.message).toMatch(/Some DB error/);
  });

  test("should add a question successfully", async () => {
    questionModel.addRecord.mockResolvedValue({ id: 1 });
    const payload = {
      question: "Test question?",
      answer_unit_id: 1,
      answer_formula: "1+1",
      variating_values: JSON.stringify([]),
      course_code: "test",
      question_type_id: 1
    };
    const res = await request(app).post('/question/add').send(payload);
    expect(res.status).toBe(201);
    expect(res.body.success).toBe(true);
    expect(res.body.id).toBe(1);
  });

  test("should resolve placeholders correctly in question text", async () => {
    // Mock the database query
    questionModel.getTable.mockResolvedValue([
      { id: 1, namn: "Morfin", variating_values: '{"dosage": [15, 10], "available_dose": [10], "injektions_styrka": [10], "dos": [10]}', fass_link: "https://example.com/morfin" },
      { id: 2, namn: "Digoxin", variating_values: '{"tablet_dose": [0.25, 0.13], "dosage": [0.25], "available_dose": [0.25]}', fass_link: "https://example.com/digoxin" }
    ]);

    // Mock the generateValues function
    questionHelpers.generateValues.mockResolvedValue({
      age: 10,
      weight: 30,
      'medicine[0]': 'Morfin',
      'medicine[0].dosage': 15
    });

    const payload = {
      question: "Patienten är %%age%% år gammal och väger %%weight%% kg. Pasienten har fått recept på %%medicine[0]%% med dosering %%medicine[0].dosage%% mg.",
      variating_values: JSON.stringify({
        medicine: ["Morfin", "Digoxin"],
        age: [3, 14],
        weight: "formula(child_weight(age))"
      }),
      answer_unit_id: 1,
      answer_formula: "1+1",
      course_code: ["test"],
      question_type_id: 1
    };

    const res = await request(app).post('/question/add').send(payload);

    expect(res.status).toBe(200);
    expect(res.body.success).toBe(true);
    expect(questionHelpers.generateValues).toHaveBeenCalledWith(
      payload.question,
      JSON.parse(payload.variating_values),
      expect.any(Array)
    );
    expect(questionModel.addRecord).toHaveBeenCalledWith(
      expect.objectContaining({
        question: "Patienten är 10 år gammal och väger 30 kg. Pasienten har fått recept på Morfin med dosering 15 mg.",
        answer_unit_id: 1,
        answer_formula: "1+1",
        variating_values: payload.variating_values,
        course_codes: JSON.stringify(["test"]),
        question_type_id: 1
      })
    );
  });

  test("should handle complex medicine dosing calculations", async () => {
    // Mock the database query with real medicine data
    questionModel.getTable.mockResolvedValue([
      { id: 1, namn: "Morfin", variating_values: '{"dosage": [15, 10], "available_dose": [10], "injektions_styrka": [10], "dos": [10]}', fass_link: "https://www.fass.se/LIF/product?userType=0&nplId=20131019000027" },
      { id: 2, namn: "Dalacin", variating_values: '{"available_dose": [150], "ampul_mangd": [4, 2], "ordinerad_mangd_infusion": [600], "antal_dostillfallen": [3], "dosage": [20]}' }
    ]);

    // Mock the generateValues function
    questionHelpers.generateValues.mockResolvedValue({
      'medicine[0].namn': 'Dalacin',
      'medicine[0].dosage': 20,
      'medicine[0].available_dose': 150,
      'medicine[0].ampul_mangd': 4,
      'medicine[0].ordinerad_mangd_infusion': 600,
      'medicine[0].antal_dostillfallen': 3
    });

    const payload = {
      question: "Läkaren har ordinerat %%medicine[0].namn%% %%medicine[0].dosage%% mg x %%medicine[0].antal_dostillfallen%% subcutant. Tillgängligt: %%medicine[0].namn%% %%medicine[0].available_dose%% mg/ml. Hur många ml motsvarar en enkeldos?",
      variating_values: JSON.stringify({
        "medicine.namn": ["Dalacin"],
        "antal_dostillfallen": [3],
        "condition": "medicine[0].dosage >= medicine[0].available_dose"
      }),
      answer_unit_id: 3,
      answer_formula: "dosage / available_dose",
      course_code: ["KM1423"],
      question_type_id: 2
    };

    const res = await request(app).post('/question/add').send(payload);

    expect(res.status).toBe(200);
    expect(res.body.success).toBe(true);
    expect(questionHelpers.generateValues).toHaveBeenCalledWith(
      payload.question,
      JSON.parse(payload.variating_values),
      expect.any(Array)
    );
    expect(questionModel.addRecord).toHaveBeenCalledWith(
      expect.objectContaining({
        question: "Läkaren har ordinerat Dalacin 20 mg x 3 subcutant. Tillgängligt: Dalacin 150 mg/ml. Hur många ml motsvarar en enkeldos?",
        answer_unit_id: 3,
        answer_formula: "dosage / available_dose",
        variating_values: payload.variating_values,
        course_codes: JSON.stringify(["KM1423"]),
        question_type_id: 2
      })
    );
  });

  test("should handle insulin calculations with units", async () => {
    // Mock the database query with insulin data
    questionModel.getTable.mockResolvedValue([
      { id: 6, namn: "Novorapid", variating_values: '{"dosage": [0.04, 0.5, 0.01], "available_dose": [100]}' }
    ]);

    // Mock the generateValues function
    questionHelpers.generateValues.mockResolvedValue({
      'medicine[0].namn': 'Novorapid',
      'medicine[0].dosage': 0.5,
      'medicine[0].available_dose': 100
    });

    const payload = {
      question: "En patient är ordinerad %%medicine[0].namn%% %%medicine[0].dosage%% ml injektionsvätska insulin med styrkan %%medicine[0].available_dose%% E/ml. Hur många E motsvarar detta?",
      variating_values: JSON.stringify({
        "medicine.namn": ["Novorapid"],
        "dosage": [0.5],
        "available_dose": [100]
      }),
      answer_unit_id: 6,
      answer_formula: "dosage * available_dose",
      course_code: ["KM1424"],
      question_type_id: 2
    };

    const res = await request(app).post('/question/add').send(payload);

    expect(res.status).toBe(200);
    expect(res.body.success).toBe(true);
    expect(questionHelpers.generateValues).toHaveBeenCalledWith(
      payload.question,
      JSON.parse(payload.variating_values),
      expect.any(Array)
    );
    expect(questionModel.addRecord).toHaveBeenCalledWith(
      expect.objectContaining({
        question: "En patient är ordinerad Novorapid 0.5 ml injektionsvätska insulin med styrkan 100 E/ml. Hur många E motsvarar detta?",
        answer_unit_id: 6,
        answer_formula: "dosage * available_dose",
        variating_values: payload.variating_values,
        course_codes: JSON.stringify(["KM1424"]),
        question_type_id: 2
      })
    );
  });

  test("should handle weight calculations with formulas", async () => {
    // Mock the database query with formula data
    questionModel.getTable.mockResolvedValue([
      { id: 1, namn: "Morfin", variating_values: '{"dosage": [15, 10], "available_dose": [10], "injektions_styrka": [10], "dos": [10]}', fass_link: "https://www.fass.se/LIF/product?userType=0&nplId=20131019000027" }
    ]);

    // Mock the generateValues function
    questionHelpers.generateValues.mockResolvedValue({
      age: 5,
      weight: 20,
      'medicine[0].namn': 'Morfin',
      'medicine[0].dosage': 15,
      'medicine[0].available_dose': 10
    });

    const payload = {
      question: "En barnpatient är %%age%% år gammal och väger %%weight%% kg. Pasienten är ordinerad %%medicine[0].namn%% %%medicine[0].dosage%% mg x 3 subcutant. Tillgängligt: %%medicine[0].namn%% %%medicine[0].available_dose%% mg/ml. Hur många ml motsvarar en enkeldos?",
      variating_values: JSON.stringify({
        "age": [3, 7],
        "weight": "formula(child_weight(age))",
        "medicine.namn": ["Morfin"],
        "condition": "medicine[0].dosage >= medicine[0].available_dose"
      }),
      answer_unit_id: 3,
      answer_formula: "dosage / available_dose",
      course_code: ["KM1425"],
      question_type_id: 2
    };

    const res = await request(app).post('/question/add').send(payload);

    expect(res.status).toBe(200);
    expect(res.body.success).toBe(true);
    expect(questionHelpers.generateValues).toHaveBeenCalledWith(
      payload.question,
      JSON.parse(payload.variating_values),
      expect.any(Array)
    );
    expect(questionModel.addRecord).toHaveBeenCalledWith(
      expect.objectContaining({
        question: "En barnpatient är 5 år gammal och väger 20 kg. Pasienten är ordinerad Morfin 15 mg x 3 subcutant. Tillgängligt: Morfin 10 mg/ml. Hur många ml motsvarar en enkeldos?",
        answer_unit_id: 3,
        answer_formula: "dosage / available_dose",
        variating_values: payload.variating_values,
        course_codes: JSON.stringify(["KM1425"]),
        question_type_id: 2
      })
    );
  });
});

describe("Question Type Tests", () => {
  // Test case for infusion calculations
  test("should handle infusion calculations with weight", async () => {
    // Mock the database query with medicine data
    questionModel.getTable.mockResolvedValue([
      { id: 8, namn: "Paracetamol", variating_values: '{"infusions_styrka": [10], "dos": [10]}' }
    ]);

    // Mock the generateValues function
    questionHelpers.generateValues.mockResolvedValue({
      'medicine[0].namn': 'Paracetamol',
      'medicine[0].infusions_styrka': 10,
      'medicine[0].dos': 10,
      weight: 50
    });

    const payload = {
      question: "Patienten är ordinerad infusion %%medicine[0].namn%%. Tillgängligt finns %%medicine[0].namn%% infusionsvätska, lösning %%medicine[0].infusions_styrka%% mg/ml. Patientens vikt är %%weight%% kg och ska därför få %%medicine[0].dos%% mg/kg.",
      variating_values: JSON.stringify({
        "medicine.namn": ["Paracetamol"],
        "weight": [10, 50]
      }),
      answer_unit_id: 4,
      answer_formula: "medicine[0].dos * weight",
      course_code: ["KM1423"],
      question_type_id: 2
    };

    const res = await request(app).post('/question/add').send(payload);

    expect(res.status).toBe(200);
    expect(res.body.success).toBe(true);
    expect(questionHelpers.generateValues).toHaveBeenCalledWith(
      payload.question,
      JSON.parse(payload.variating_values),
      expect.any(Array)
    );
    expect(questionModel.addRecord).toHaveBeenCalledWith(
      expect.objectContaining({
        question: "Patienten är ordinerad infusion Paracetamol. Tillgängligt finns Paracetamol infusionsvätska, lösning 10 mg/ml. Patientens vikt är 50 kg och ska därför få 10 mg/kg.",
        answer_unit_id: 4,
        answer_formula: "medicine[0].dos * weight",
        variating_values: payload.variating_values,
        course_codes: JSON.stringify(["KM1423"]),
        question_type_id: 2
      })
    );
  });

  // Test case for nebulizer calculations
  test("should handle nebulizer dose calculations", async () => {
    // Mock the database query with medicine data
    questionModel.getTable.mockResolvedValue([
      { id: 11, namn: "Ventoline", variating_values: '{"styrka": [5, 2], "ordinerad_mangd_nebulisator": [0.5, 2, 0.5]}' }
    ]);

    // Mock the generateValues function
    questionHelpers.generateValues.mockResolvedValue({
      'medicine[0].namn': 'Ventoline',
      'medicine[0].styrka': 5,
      'medicine[0].ordinerad_mangd_nebulisator': 0.5
    });

    const payload = {
      question: "%%namn%% har kronisk bronkobstruktion och ordineras inhalation med %%medicine[0].namn%%. %%medicine[0].namn%% är ett bronkdilaterande medel vid astma och KOL som ges via nebulisator. Tillgängligt preparat: %%medicine[0].namn%%, lösning för nebulisator %%medicine[0].styrka%% mg/ml. Hur många mg erhåller %%namn%% om %%medicine[0].ordinerad_mangd_nebulisator%% ml %%medicine[0].namn%% ordineras?",
      variating_values: JSON.stringify({
        "medicine.namn": ["Ventoline"]
      }),
      answer_unit_id: 4,
      answer_formula: "medicine[0].styrka * medicine[0].ordinerad_mangd_nebulisator",
      course_code: ["KM1423"],
      question_type_id: 2
    };

    const res = await request(app).post('/question/add').send(payload);

    expect(res.status).toBe(200);
    expect(res.body.success).toBe(true);
    expect(questionHelpers.generateValues).toHaveBeenCalledWith(
      payload.question,
      JSON.parse(payload.variating_values),
      expect.any(Array)
    );
    expect(questionModel.addRecord).toHaveBeenCalledWith(
      expect.objectContaining({
        question: "Patienten har kronisk bronkobstruktion och ordineras inhalation med Ventoline. Ventoline är ett bronkdilaterande medel vid astma och KOL som ges via nebulisator. Tillgängligt preparat: Ventoline, lösning för nebulisator 5 mg/ml. Hur många mg erhåller patienten om 0.5 ml Ventoline ordineras?",
        answer_unit_id: 4,
        answer_formula: "medicine[0].styrka * medicine[0].ordinerad_mangd_nebulisator",
        variating_values: payload.variating_values,
        course_codes: JSON.stringify(["KM1423"]),
        question_type_id: 2
      })
    );
  });

  // Test case for intramuscular injection calculations
  test("should handle intramuscular injection calculations", async () => {
    // Mock the database query with medicine data
    questionModel.getTable.mockResolvedValue([
      { id: 1, namn: "Morfin", variating_values: '{"dosage": [15, 10], "available_dose": [10], "injektions_styrka": [10], "dos": [10]}' }
    ]);

    // Mock the generateValues function
    questionHelpers.generateValues.mockResolvedValue({
      'medicine[0].namn': 'Morfin',
      'medicine[0].injektions_styrka': 10,
      'medicine[0].dos': 10,
      antal: 3
    });

    const payload = {
      question: "Läkaren har ordinerat intramuskulär injektion %%medicine[0].namn%% %%medicine[0].injektions_styrka%% mg x %%antal%%. Tillgängligt finns %%medicine[0].namn%% injektionsvätska %%medicine[0].dos%% mg/ml.",
      variating_values: JSON.stringify({
        "medicine.namn": ["Morfin"],
        "antal": [1, 3]
      }),
      answer_unit_id: 3,
      answer_formula: "medicine[0].injektions_styrka / medicine[0].dos",
      course_code: ["KM1423"],
      question_type_id: 2
    };

    const res = await request(app).post('/question/add').send(payload);

    expect(res.status).toBe(200);
    expect(res.body.success).toBe(true);
    expect(questionHelpers.generateValues).toHaveBeenCalledWith(
      payload.question,
      JSON.parse(payload.variating_values),
      expect.any(Array)
    );
    expect(questionModel.addRecord).toHaveBeenCalledWith(
      expect.objectContaining({
        question: "Läkaren har ordinerat intramuskulär injektion Morfin 10 mg x 3. Tillgängligt finns Morfin injektionsvätska 10 mg/ml.",
        answer_unit_id: 3,
        answer_formula: "medicine[0].injektions_styrka / medicine[0].dos",
        variating_values: payload.variating_values,
        course_codes: JSON.stringify(["KM1423"]),
        question_type_id: 2
      })
    );
  });

  // Test case for postoperative infection treatment
  test("should handle postoperative infection treatment calculations", async () => {
    // Mock the database query with medicine data
    questionModel.getTable.mockResolvedValue([
      { id: 8, namn: "Dalacin", variating_values: '{"available_dose": [150], "ampul_mangd": [4, 2], "ordinerad_mangd_infusion": [600], "antal_dostillfallen": [3], "dosage": [20]}' }
    ]);

    // Mock the generateValues function
    questionHelpers.generateValues.mockResolvedValue({
      'medicine[0].namn': 'Dalacin',
      'medicine[0].ordinerad_mangd_infusion': 600,
      'medicine[0].available_dose': 150,
      'medicine[0].ampul_mangd': 4,
      'medicine[0].antal_dostillfallen': 3
    });

    const payload = {
      question: "En patient har en postoperativ sårinfektion. Pasienten ordineras infusion %%medicine[0].namn%% %%medicine[0].ordinerad_mangd_infusion%% mg x %%medicine[0].antal_dostillfallen%% . I medicinskåpet finner du injektionsvätska %%medicine[0].namn%% %%medicine[0].available_dose%% mg/ml med %%medicine[0].ampul_mangd%% ml i varje ampull.",
      variating_values: JSON.stringify({
        "medicine.namn": ["Dalacin"]
      }),
      answer_unit_id: 8,
      answer_formula: "ordinerad_mangd / (medicine[0].available_dose * medicine[0].ampul_mangd)",
      course_code: ["KM1423"],
      question_type_id: 2
    };

    const res = await request(app).post('/question/add').send(payload);

    expect(res.status).toBe(200);
    expect(res.body.success).toBe(true);
    expect(questionHelpers.generateValues).toHaveBeenCalledWith(
      payload.question,
      JSON.parse(payload.variating_values),
      expect.any(Array)
    );
    expect(questionModel.addRecord).toHaveBeenCalledWith(
      expect.objectContaining({
        question: "En patient har en postoperativ sårinfektion. Pasienten ordineras infusion Dalacin 600 mg x 3 . I medicinskåpet finner du injektionsvätska Dalacin 150 mg/ml med 4 ml i varje ampull.",
        answer_unit_id: 8,
        answer_formula: "ordinerad_mangd / (medicine[0].available_dose * medicine[0].ampul_mangd)",
        variating_values: payload.variating_values,
        course_codes: JSON.stringify(["KM1423"]),
        question_type_id: 2
      })
    );
  });
});

describe("Exhaustive tests for GET /question/random", () => {
  beforeEach(() => {
    jest.resetAllMocks();
  });

  test("should return 400 if course_code is missing", async () => {
    const res = await request(app).get('/question/random');
    expect(res.status).toBe(400);
    expect(res.body.message).toMatch(/A course code must be provided/);
  });

  test("should return 404 if no questions found", async () => {
    questionModel.getRecordWhere.mockResolvedValue([]);
    const res = await request(app).get('/question/random').query({ course_code: "TEST" });
    expect(res.status).toBe(404);
    expect(res.body.message).toMatch(/No questions found/);
  });

  test("should return 500 if variating_values is invalid JSON", async () => {
    const fakeQuestion = {
      id: 1,
      question: "What is %%a%%?",
      variating_values: "invalid-json", // Will cause JSON.parse error
      answer_formula: "a",
      answer_unit_id: 1,
      course_code: "TEST",
      question_type_id: 1
    };
    questionModel.getRecordWhere.mockResolvedValue([fakeQuestion]);
    const res = await request(app).get('/question/random').query({ course_code: "TEST" });
    expect(res.status).toBe(500);
    expect(res.body.message).toMatch(/Invalid variating_values format/);
  });

  test("should return 500 if generated values are missing required keys", async () => {
    const fakeQuestion = {
      id: 1,
      question: "What is %%a%% + %%b%%?",
      variating_values: JSON.stringify({ a: [1, 1], b: [2, 2] }),
      answer_formula: "a+b",
      answer_unit_id: 1,
      course_code: "TEST",
      question_type_id: 1
    };
    questionModel.getRecordWhere.mockResolvedValue([fakeQuestion]);
    questionModel.getTable.mockResolvedValue([]); // Assume no medicine data
    routeHelpers.getRandomElement.mockReturnValue(fakeQuestion);
    // Simulate generatedValues missing the key 'b'
    questionHelpers.generateValues.mockReturnValue({ a: 1 });
    const res = await request(app).get('/question/random').query({ course_code: "TEST" });
    expect(res.status).toBe(500);
    expect(res.body.message).toMatch(/Missing required values for formula/);
  });

  test("should return 500 if answer_formula is missing required variables", async () => {
    const fakeQuestion = {
      id: 1,
      question: "What is %%a%% + %%b%%?",
      variating_values: JSON.stringify({ a: [1, 1], b: [2, 2] }),
      answer_formula: "a+b+c", // 'c' is not provided by generatedValues
      answer_unit_id: 1,
      course_code: "TEST",
      question_type_id: 1
    };
    questionModel.getRecordWhere.mockResolvedValue([fakeQuestion]);
    questionModel.getTable.mockResolvedValue([]);
    routeHelpers.getRandomElement.mockReturnValue(fakeQuestion);
    questionHelpers.generateValues.mockReturnValue({ a: 1, b: 2 });
    const res = await request(app).get('/question/random').query({ course_code: "TEST" });
    expect(res.status).toBe(500);
    expect(res.body.message).toMatch(/Missing required values for formula: c/);
  });

  test("should return 500 if formula substitution results in invalid format", async () => {
    const fakeQuestion = {
      id: 1,
      question: "What is %%a%% + %%b%%?",
      variating_values: JSON.stringify({ a: [1, 1], b: [2, 2] }),
      answer_formula: "a+b", // With non-numeric substitution, the result may be invalid
      answer_unit_id: 1,
      course_code: "TEST",
      question_type_id: 1
    };
    questionModel.getRecordWhere.mockResolvedValue([fakeQuestion]);
    questionModel.getTable.mockResolvedValue([]);
    routeHelpers.getRandomElement.mockReturnValue(fakeQuestion);
    // Simulate generatedValues with a non-numeric value for b
    questionHelpers.generateValues.mockReturnValue({ a: 1, b: "xyz" });
    const res = await request(app).get('/question/random').query({ course_code: "TEST" });
    expect(res.status).toBe(500);
    expect(res.body.message).toMatch(/Invalid formula format/);
  });

  test("should return 500 if error evaluating the formula", async () => {
    const fakeQuestion = {
      id: 1,
      question: "What is %%a%% + %%b%%?",
      variating_values: JSON.stringify({ a: [1, 1], b: [2, 2] }),
      answer_formula: "a++b", // Invalid syntax
      answer_unit_id: 1,
      course_code: "TEST",
      question_type_id: 1
    };
    questionModel.getRecordWhere.mockResolvedValue([fakeQuestion]);
    questionModel.getTable.mockResolvedValue([]);
    routeHelpers.getRandomElement.mockReturnValue(fakeQuestion);
    questionHelpers.generateValues.mockReturnValue({ a: 1, b: 2 });
    const res = await request(app).get('/question/random').query({ course_code: "TEST" });
    expect(res.status).toBe(500);
    expect(res.body.message).toMatch(/Error evaluating formula/);
  });

  test("should return 500 if getRecordById for units fails", async () => {
    const fakeQuestion = {
      id: 1,
      question: "What is %%a%%?",
      variating_values: JSON.stringify({ a: [1, 1] }),
      answer_formula: "a",
      answer_unit_id: 1,
      course_code: "TEST",
      question_type_id: 1
    };
    questionModel.getRecordWhere.mockResolvedValue([fakeQuestion]);
    questionModel.getTable.mockResolvedValue([]);
    routeHelpers.getRandomElement.mockReturnValue(fakeQuestion);
    questionHelpers.generateValues.mockReturnValue({ a: 1 });
    // Simulate failure in fetching the unit record.
    questionModel.getRecordById.mockRejectedValue({ message: "Unit not found", status: 404 });
    const res = await request(app).get('/question/random').query({ course_code: "TEST" });
    expect(res.status).toBe(500);
    expect(res.body.message).toMatch(/Error retrieving question/);
  });

  test("should return a processed question successfully", async () => {
    // Create a fake question record.
    const fakeQuestion = {
      id: 1,
      question: "What is %%a%% + %%b%%?",
      variating_values: JSON.stringify({ a: [1, 1], b: [2, 2] }),
      answer_formula: "a+b",
      answer_unit_id: 1,
      course_code: "TEST",
      question_type_id: 1
    };

    questionModel.getRecordWhere.mockResolvedValue([fakeQuestion]);
    questionModel.getTable.mockResolvedValue([]); // Assume no medicine data needed
    routeHelpers.getRandomElement.mockReturnValue(fakeQuestion);
    // Mock generateValues to return predictable values.
    questionHelpers.generateValues.mockReturnValue({ a: 1, b: 2 });
    // Mock getRecordById to return a fake unit record.
    questionModel.getRecordById.mockResolvedValue({
      id: 1,
      ascii_name: "kg",
      accepted_answer: JSON.stringify(["kg"])
    });

    const res = await request(app).get('/question/random').query({ course_code: "TEST" });
    expect(res.status).toBe(200);
    expect(res.body.success).toBe(true);
    expect(res.body.data).toBeDefined();
    expect(res.body.data.computed_answer).toBe(3);
    expect(res.body.data.question).toBe("What is 1 + 2?");
  });
});

describe("Exhaustive tests for POST /question/check-answer", () => {
  test("should return 400 if required fields are missing", async () => {
    const res = await request(app).post('/question/check-answer').send({});
    expect(res.status).toBe(400);
    expect(res.body.correct).toBe(false);
    expect(res.body.message).toMatch(/Missing required fields/);
  });

  test("should return 500 if correctUnit JSON is invalid", async () => {
    const payload = {
      answer: "5.5 kg",
      correctAnswer: 5.5,
      correctUnit: "not-a-json", // Invalid JSON
      formula: "5.5"
    };
    const res = await request(app).post('/question/check-answer').send(payload);
    expect(res.status).toBe(500);
    expect(res.body.message).toMatch(/Invalid accepted_answers format/);
  });

  test("should return correct result for a valid answer", async () => {
    const payload = {
      answer: "5.5 kg",
      correctAnswer: 5.5,
      correctUnit: JSON.stringify(["kg"]),
      formula: "5.5"
    };
    const res = await request(app).post('/question/check-answer').send(payload);
    expect(res.status).toBe(200);
    expect(res.body.correct).toBe(true);
    expect(res.body.message).toBe("Correct!");
  });

  test("should return incorrect result if numeric value does not match after rounding", async () => {
    const payload = {
      answer: "5.556 kg", // Rounds to 5.56
      correctAnswer: 5.55,
      correctUnit: JSON.stringify(["kg"]),
      formula: "5.555"
    };
    const res = await request(app).post('/question/check-answer').send(payload);
    expect(res.status).toBe(200);
    expect(res.body.correct).toBe(false);
    expect(res.body.message).toMatch(/Incorrect answer/);
  });

  test("should return incorrect result if the unit is missing", async () => {
    const payload = {
      answer: "5.5", // missing unit
      correctAnswer: 5.5,
      correctUnit: JSON.stringify(["kg"]),
      formula: "5.5"
    };
    const res = await request(app).post('/question/check-answer').send(payload);
    // Expect a failure because unit is missing
    expect(res.status).toBe(200);
    expect(res.body.correct).toBe(false);
    expect(res.body.message).toMatch(/unit/); // Adjust depending on your error message
  });
  
  test("should return incorrect result if the wrong unit is provided", async () => {
    const payload = {
      answer: "5.5 lb", // wrong unit
      correctAnswer: 5.5,
      correctUnit: JSON.stringify(["kg"]),
      formula: "5.5"
    };
    const res = await request(app).post('/question/check-answer').send(payload);
    // Expect a failure because the unit doesn't match the expected "kg"
    expect(res.status).toBe(200);
    expect(res.body.correct).toBe(false);
    expect(res.body.message).toMatch(/unit/); // Adjust based on the actual error text you'll implement
  });
});