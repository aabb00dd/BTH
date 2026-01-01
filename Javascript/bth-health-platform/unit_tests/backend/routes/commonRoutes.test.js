const request = require("../../../backend/node_modules/supertest");
const express = require("../../../backend/node_modules/express");
const router = require("../../../backend/routes/commonRoutes");

// Mock the model so it doesn't perform real DB operations
jest.mock("../../../backend/models/questionModel", () => ({
  getTable: jest.fn(),
  getRecordById: jest.fn(),
}));

const { getTable } = require("../../../backend/models/questionModel");

// Helper function to set up an Express app using our router
function createTestApp() {
  const app = express();
  app.use(express.json());
  // Mount the router at /api
  app.use('/api', router);
  return app;
}

describe('Test the /:table/all route', () => {
  let app;

  beforeAll(() => {
    app = createTestApp();
  });

  afterEach(() => {
    jest.clearAllMocks();
  });

  it('should return 200 and the records for a valid table', async () => {
    // Mock getTable to return some data
    const mockData = [{ id: 1, name: 'Sample' }];
    getTable.mockResolvedValue(mockData);

    const validTable = 'units'; // One of the allowed tables in your code
    const res = await request(app).get(`/api/${validTable}/all`);

    expect(res.status).toBe(200);
    expect(res.body).toEqual({
      success: true,
      records: mockData,
    });
    // Ensure our mock was called with the correct table name
    expect(getTable).toHaveBeenCalledWith(validTable);
  });

  it('should return 400 for an invalid table name', async () => {
    const invalidTable = 'notARealTable';
    const res = await request(app).get(`/api/${invalidTable}/all`);

    expect(res.status).toBe(400);
    expect(res.body).toEqual({
      success: false,
      message: 'Invalid table name',
    });
    // The model function should NOT be called in this case
    expect(getTable).not.toHaveBeenCalled();
  });

  it('should return 500 if getTable throws an error', async () => {
    // Force getTable to reject (simulating a DB error)
    getTable.mockRejectedValue(new Error('DB failed'));

    const validTable = 'units';
    const res = await request(app).get(`/api/${validTable}/all`);

    expect(res.status).toBe(500);
    expect(res.body).toEqual({
      success: false,
      message: 'Error fetching records',
    });
    expect(getTable).toHaveBeenCalledWith(validTable);
  });
});
