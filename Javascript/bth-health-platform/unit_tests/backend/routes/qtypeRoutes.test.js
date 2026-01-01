// qtypeRoute.test.js
const request = require("../../../backend/node_modules/supertest");
const express = require("../../../backend/node_modules/express");

// 1) Mock the functions from questionModel
//    This will replace the real addRecord, etc., with jest mock functions
jest.mock("../../../backend/models/questionModel", () => ({
  addRecord: jest.fn(),
  updateRecord: jest.fn(),
  getRecordById: jest.fn(),
}));

// 2) Import the router after mocking
const router = require("../../../backend/routes/qtypeRoutes"); // e.g. "./qtypeRoute"

// 3) Create an express app to mount the router
//    This allows supertest to call the endpoints.
const app = express();
app.use(express.json());
app.use("/qtype", router);

// 4) Extract the mocked functions to use in tests
const { addRecord } = require("../../../backend/models/questionModel");

describe("POST /qtype/add", () => {
  afterEach(() => {
    // Clear all mocks after each test to avoid test pollution
    jest.clearAllMocks();
  });

  test("Should respond with 400 if 'name' is missing", async () => {
    const response = await request(app)
      .post("/qtype/add")
      .send({}); // no 'name'

    expect(response.status).toBe(400);
    expect(response.body).toEqual({
      success: false,
      message: "Invalid or missing 'name' (must be a string)",
    });

    // addRecord should NOT have been called
    expect(addRecord).not.toHaveBeenCalled();
  });

  test("Should respond with 400 if 'name' is not a string", async () => {
    const response = await request(app)
      .post("/qtype/add")
      .send({ name: 123 });

    expect(response.status).toBe(400);
    expect(response.body).toEqual({
      success: false,
      message: "Invalid or missing 'name' (must be a string)",
    });

    // addRecord should NOT have been called
    expect(addRecord).not.toHaveBeenCalled();
  });

  test("Should respond with 201 when successfully added", async () => {
    // 1) Mock addRecord to resolve with an object containing an id
    addRecord.mockResolvedValueOnce({ id: 42 });

    const response = await request(app)
      .post("/qtype/add")
      .send({ name: "myQType" });

    expect(response.status).toBe(201);
    expect(response.body).toEqual({
      success: true,
      message: "QType successfully added",
      id: 42,
    });

    // addRecord should have been called once with the right args
    expect(addRecord).toHaveBeenCalledTimes(1);
    expect(addRecord).toHaveBeenCalledWith(
      "qtype",
      ["name", "history_json"],
      ["myQType", "{}"] // history_json default "{}"
    );
  });

  test("Should respond with 409 on unique constraint errors", async () => {
    // 1) Mock addRecord to throw an error that includes “UNIQUE constraint failed”
    addRecord.mockRejectedValueOnce(new Error("UNIQUE constraint failed: qtype.name"));

    const response = await request(app)
      .post("/qtype/add")
      .send({ name: "duplicateName" });

    expect(response.status).toBe(409);
    expect(response.body).toEqual({
      success: false,
      message:
        "QType name must be unique. The provided value already exists.",
    });

    // addRecord should have been called once
    expect(addRecord).toHaveBeenCalledTimes(1);
  });

  test("Should respond with 500 on server errors", async () => {
    // 1) Mock addRecord to throw some other error
    addRecord.mockRejectedValueOnce(new Error("Some random DB error"));

    const response = await request(app)
      .post("/qtype/add")
      .send({ name: "myQType" });

    expect(response.status).toBe(500);
    expect(response.body).toEqual({
      success: false,
      message: "Internal server error",
    });

    // addRecord should have been called once
    expect(addRecord).toHaveBeenCalledTimes(1);
  });
});
