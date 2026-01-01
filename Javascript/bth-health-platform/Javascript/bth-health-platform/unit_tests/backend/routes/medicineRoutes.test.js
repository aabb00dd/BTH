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
const router = require("../../../backend/routes/medicineRoutes"); 

// Mock the model functions and axios
const { addRecord } = require("../../../backend/models/questionModel");
jest.mock("../../../backend/models/questionModel");

// PROBLEM WITH AXIOS IMPORT
const axios = require("axios");
jest.mock("axios");
// PROBELM WITH AXIOS IMPORT

// Create an Express app with your router
const app = express();
app.use(express.json());
app.use("/course", router);

describe("POST /course/add", () => {
  beforeEach(() => {
    jest.clearAllMocks();
  });

  it("should return 400 if required fields are missing", async () => {
    const response = await request(app)
      .post("/course/add")
      .send({ /* no name, fass_link, styrkor_doser here */ });

    expect(response.status).toBe(400);
    expect(response.body).toMatchObject({
      success: false,
      message: expect.stringContaining("Missing required fields"),
    });
  });

  it("should return 400 if name or fass_link are not strings", async () => {
    const response = await request(app)
      .post("/course/add")
      .send({
        name: 12345,          // invalid type
        fass_link: true,      // invalid type
        styrkor_doser: "{}",  // valid JSON string
      });

    expect(response.status).toBe(400);
    expect(response.body).toMatchObject({
      success: false,
      message: expect.stringContaining("Invalid data type"),
    });
  });

  it("should return 400 if fass_link is invalid URL format", async () => {
    // Here we ensure the URL constructor in the route fails
    const response = await request(app)
      .post("/course/add")
      .send({
        name: "TestMedicine",
        fass_link: "randomBadUrl",
        styrkor_doser: "{}",
      });

    expect(response.status).toBe(400);
    expect(response.body).toMatchObject({
      success: false,
      message: expect.stringContaining("fass_link is not a valid or reachable website"),
    });
  });

  it("should return 400 if fass_link is unreachable", async () => {
    // Mock axios to reject -> simulating unreachable link
    axios.head.mockRejectedValue(new Error("Network Error"));

    const response = await request(app)
      .post("/course/add")
      .send({
        name: "TestMedicine",
        fass_link: "http://www.fakefass.se?userType=0",
        styrkor_doser: "[]",
      });

    expect(response.status).toBe(400);
    expect(response.body).toMatchObject({
      success: false,
      message: expect.stringContaining("fass_link is not a valid or reachable website"),
    });
  });

  it("should return 400 if styrkor_doser is not valid JSON", async () => {
    // Mock axios to succeed
    axios.head.mockResolvedValue({ status: 200 });

    const response = await request(app)
      .post("/course/add")
      .send({
        name: "TestMedicine",
        fass_link: "https://www.fass.se?userType=0",
        styrkor_doser: "thisIsNotJson", // invalid JSON
      });

    expect(response.status).toBe(400);
    expect(response.body).toMatchObject({
      success: false,
      message: expect.stringContaining("must be a valid JSON string"),
    });
  });

  it("should return 201 and call addRecord if everything is valid", async () => {
    // Mock axios to succeed
    axios.head.mockResolvedValue({ status: 200 });

    // Mock addRecord to return a successful ID
    addRecord.mockResolvedValue({ id: 123 });

    const validData = {
      name: "TestMedicine",
      fass_link: "https://someDomain.se?userType=0",
      styrkor_doser: '{"dose": "value"}',
    };

    const response = await request(app).post("/course/add").send(validData);

    expect(response.status).toBe(201);
    expect(response.body).toMatchObject({
      success: true,
      message: "Record successfully added",
      id: 123,
    });

    // Ensure addRecord was called with correct args
    expect(addRecord).toHaveBeenCalledWith(
      "medicine",
      ["name", "fass_link", "styrkor_doser"],
      [
        "TestMedicine",
        expect.stringContaining("https://www.fass.se/?userType=0"),
        '{"dose": "value"}'
      ]
    );
  });

  it("should return 409 if UNIQUE constraint fails", async () => {
    // Mock axios to succeed
    axios.head.mockResolvedValue({ status: 200 });

    // Force addRecord to reject with an error message containing "UNIQUE constraint failed"
    addRecord.mockRejectedValue({
      message: "UNIQUE constraint failed: medicine.name",
    });

    const response = await request(app)
      .post("/course/add")
      .send({
        name: "DuplicateMedicine",
        fass_link: "https://www.fass.se?userType=0",
        styrkor_doser: "{}",
      });

    expect(response.status).toBe(409);
    expect(response.body).toMatchObject({
      success: false,
      message: expect.stringContaining("already exists"),
    });
  });

  it("should return 500 on an unexpected error", async () => {
    // Mock axios to succeed
    axios.head.mockResolvedValue({ status: 200 });

    // Force addRecord to throw a generic error
    addRecord.mockRejectedValue(new Error("Some unexpected error"));

    const response = await request(app)
      .post("/course/add")
      .send({
        name: "TestMedicine",
        fass_link: "https://www.fass.se?userType=0",
        styrkor_doser: "{}",
      });

    expect(response.status).toBe(500);
    expect(response.body).toMatchObject({
      success: false,
      message: "Internal server error",
    });
  });
});

const { closeDB } = require("../../../backend/models/questionModel");

const sqlite3 = require("sqlite3");
sqlite3.Database.prototype.close = function(callback) {
  if (callback) callback();
};
