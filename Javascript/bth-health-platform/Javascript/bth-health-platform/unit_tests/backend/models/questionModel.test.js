const request = require("../../../backend/node_modules/supertest");
const express = require("../../../backend/node_modules/express");
const questionRoutes = require("questionsRoutes.js");
const { 
  addRecord, 
  getRecords, 
  getRecordById, 
  updateRecord, 
  deleteRecord 
} = require("../../../backend/models/questionModel");

const app = express();
app.use(express.json());
app.use("/api", questionRoutes);

jest.mock("../../../backend/models/questionModel");

describe("API Routes for Question Database", () => {
  const sampleRecords = {
    question_data: {
      question: "Sample Question %%x%% + %%y%%",
      answer_unit_id: 1,
      answer_formula: "x+y",
      variating_values: '{"x": [2,4], "y": [1,10]}',
      course_code: "CS101",
      question_type_id: 1,
      hint_id: 1
    },
    units: {
      ascii_name: "unit_name",
      accepted_answer: JSON.stringify(["valid answer"])
    },
    course: {
      course_code: "CS101",
      course_name: "Computer Science",
      question_types: "[1,2]"
    },
    medicine: {
      namn: "Aspirin",
      fass_link: "http://fass.se/aspirin",
      skyrkor_doser: '{"dose": 500}'
    },
    qtype: {
      name: "Multiple Choice",
      history_json: '{"attempts": 5}'
    }
  };

  Object.keys(sampleRecords).forEach((table) => {
    describe(`Testing /api/${table} routes`, () => {
      // -------------------------- Test Insertions -----------------------------
      it(`should add a new record to ${table} and return success message`, async () => {
        addRecord.mockResolvedValue({ id: 1 });
        const res = await request(app)
          .post(`/api/${table}/add`)
          .send(sampleRecords[table]);
        expect(res.status).toBe(201);
        expect(res.body.success).toBe(true);
        expect(res.body).toHaveProperty("message");
      });

      it(`should fail to add a record with missing fields to ${table} and return error message`, async () => {
        addRecord.mockRejectedValue({ status: 400, message: "Missing required fields" });
        const res = await request(app)
          .post(`/api/${table}/add`)
          .send({});
        expect(res.status).toBe(400);
        expect(res.body.success).toBe(false);
        expect(res.body).toHaveProperty("message");
      });

      it(`should fail to add duplicate record to ${table} and return conflict error`, async () => {
        addRecord.mockRejectedValue({ status: 409, message: "Duplicate entry. This record already exists." });
        const res = await request(app)
          .post(`/api/${table}/add`)
          .send(sampleRecords[table]);
        expect(res.status).toBe(409);
        expect(res.body.success).toBe(false);
        expect(res.body).toHaveProperty("message");
      });

      it(`should fail to add record with foreign key constraint error for ${table} and return error message`, async () => {
        addRecord.mockRejectedValue({ status: 400, message: "Foreign key constraint failed. Check that referenced data exists." });
        const res = await request(app)
          .post(`/api/${table}/add`)
          .send(sampleRecords[table]);
        expect(res.status).toBe(400);
        expect(res.body.success).toBe(false);
        expect(res.body).toHaveProperty("message");
      });

      // -------------------------- Test Fetching -----------------------------
      it(`should fetch all records from ${table} and return success`, async () => {
        getRecords.mockResolvedValue([sampleRecords[table]]);
        const res = await request(app)
          .get(`/api/${table}/all`);
        expect(res.status).toBe(200);
        expect(res.body.success).toBe(true);
        expect(res.body).toHaveProperty("records");
      });

      it(`should return 500 when fetching records from ${table} due to a database error`, async () => {
        getRecords.mockRejectedValue({ status: 500, message: "Database error" });
        const res = await request(app)
          .get(`/api/${table}/all`);
        expect(res.status).toBe(500);
        expect(res.body.success).toBe(false);
        expect(res.body).toHaveProperty("message");
      });

      it(`should fetch a single record by ID from ${table} and return success`, async () => {
        getRecordById.mockResolvedValue(sampleRecords[table]);
        const res = await request(app)
          .get(`/api/${table}/1`);
        expect(res.status).toBe(200);
        expect(res.body.success).toBe(true);
        expect(res.body).toHaveProperty("record");
      });

      it(`should return 404 for non-existent record in ${table} with error message`, async () => {
        getRecordById.mockRejectedValue({ status: 404, message: "Record not found" });
        const res = await request(app)
          .get(`/api/${table}/999`);
        expect(res.status).toBe(404);
        expect(res.body.success).toBe(false);
        expect(res.body).toHaveProperty("message");
      });

      it(`should return 500 when fetching a single record by ID from ${table} due to a database error`, async () => {
        getRecordById.mockRejectedValue({ status: 500, message: "Database error" });
        const res = await request(app)
          .get(`/api/${table}/1`);
        expect(res.status).toBe(500);
        expect(res.body.success).toBe(false);
        expect(res.body).toHaveProperty("message");
      });

      // -------------------------- Test Updating -----------------------------
      it(`should update an existing record in ${table} and return success`, async () => {
        updateRecord.mockResolvedValue({ message: "Record updated" });
        const res = await request(app)
          .put(`/api/${table}/update/1`)
          .send(sampleRecords[table]);
        expect(res.status).toBe(200);
        expect(res.body.success).toBe(true);
        expect(res.body).toHaveProperty("message");
      });

      it(`should return 404 when updating a non-existent record in ${table} with error message`, async () => {
        updateRecord.mockRejectedValue({ status: 404, message: "Record not found" });
        const res = await request(app)
          .put(`/api/${table}/update/999`)
          .send(sampleRecords[table]);
        expect(res.status).toBe(404);
        expect(res.body.success).toBe(false);
        expect(res.body).toHaveProperty("message");
      });

      it(`should return 500 when updating a record in ${table} due to a database error`, async () => {
        updateRecord.mockRejectedValue({ status: 500, message: "Database error" });
        const res = await request(app)
          .put(`/api/${table}/update/1`)
          .send(sampleRecords[table]);
        expect(res.status).toBe(500);
        expect(res.body.success).toBe(false);
        expect(res.body).toHaveProperty("message");
      });

      // -------------------------- Test Deleting -----------------------------
      it(`should delete a record from ${table} and return success message`, async () => {
        deleteRecord.mockResolvedValue({ message: "Record deleted" });
        const res = await request(app)
          .delete(`/api/${table}/delete/1`);
        expect(res.status).toBe(200);
        expect(res.body.success).toBe(true);
        expect(res.body).toHaveProperty("message");
      });

      it(`should return 404 when deleting a non-existent record from ${table} with error message`, async () => {
        deleteRecord.mockRejectedValue({ status: 404, message: "Record not found" });
        const res = await request(app)
          .delete(`/api/${table}/delete/999`);
        expect(res.status).toBe(404);
        expect(res.body.success).toBe(false);
        expect(res.body).toHaveProperty("message");
      });

      it(`should return 500 when deleting a record in ${table} due to a database error`, async () => {
        deleteRecord.mockRejectedValue({ status: 500, message: "Database error" });
        const res = await request(app)
          .delete(`/api/${table}/delete/1`);
        expect(res.status).toBe(500);
        expect(res.body.success).toBe(false);
        expect(res.body).toHaveProperty("message");
      });
    });
  });
});
