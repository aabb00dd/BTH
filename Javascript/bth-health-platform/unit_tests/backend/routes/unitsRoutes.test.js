// === Patch for mime ===
const mime = require("mime");
if (typeof mime.getType !== "function") {
  mime.getType = mime.lookup;
}

// === Suppress SQLite connection log after tests are done ===
const originalConsoleLog = console.log;
console.log = (...args) => {
  if (typeof args[0] === "string" && args[0].includes("Connected to the SQLite database.")) {
    // Ignore this log to prevent async logging errors.
    return;
  }
  originalConsoleLog(...args);
};

const request = require("../../../backend/node_modules/supertest");
const express = require("../../../backend/node_modules/express");

// 1) Mock the questionModel so we don't actually call the DB
jest.mock("../../../backend/models/questionModel", () => ({
  addRecord: jest.fn(),
}));

// 2) Import the router AFTER the mock
const questionRouter = require("../../../backend/routes/unitsRoutes");

// 3) Setup a minimal Express app with the tested router
const app = express();
app.use(express.json());
app.use("/units", questionRouter);

// 4) Grab the mocked addRecord function so we can control/test its behavior
const { addRecord } = require("../../../backend/models/questionModel");

describe("POST /units/add", () => {
  afterEach(() => {
    jest.clearAllMocks(); // Reset mock counts between tests
  });

  test("should return 400 if ascii_name or accepted_answer is missing", async () => {
    const res = await request(app)
      .post("/units/add")
      .send({}) // No ascii_name or accepted_answer
      .expect(400);

    expect(res.body).toEqual({
      success: false,
      message: "Missing required fields",
    });
  });

  test("should return 400 if accepted_answer is not a valid JSON array of strings", async () => {
    const res = await request(app)
      .post("/units/add")
      .send({
        ascii_name: "testUnit",
        accepted_answer: "invalid json",
      })
      .expect(400);

    expect(res.body).toEqual({
      success: false,
      message: "Invalid accepted_answer format. Must be a JSON array of strings.",
    });
  });

  test("should return 409 if ascii_name already exists", async () => {
    // Mock addRecord to throw an error that includes UNIQUE constraint
    addRecord.mockRejectedValueOnce({
      message: "UNIQUE constraint failed: ascii_name",
    });

    const res = await request(app)
      .post("/units/add")
      .send({
        ascii_name: "duplicateUnit",
        accepted_answer: '["valid","JSON","array"]',
      })
      .expect(409);

    expect(res.body).toEqual({
      success: false,
      message: "ascii_name must be unique. The provided value already exists.",
    });
  });

  test("should return 201 on success and include new record ID", async () => {
    // Mock a successful insert => returns { id: someNumber }
    addRecord.mockResolvedValueOnce({ id: 123 });

    const res = await request(app)
      .post("/units/add")
      .send({
        ascii_name: "uniqueUnit",
        accepted_answer: '["some","answers"]',
      })
      .expect(201);

    expect(res.body).toEqual({
      success: true,
      message: "Record successfully added",
      id: 123,
    });
  });
});
