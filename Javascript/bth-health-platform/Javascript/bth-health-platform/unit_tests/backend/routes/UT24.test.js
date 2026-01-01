const { generateValues } = jest.requireActual("../../../backend/helpers/questionHelpers");

describe("UT 24: Comprehensive tests for generateValues", () => {

  // A dummy empty medicine list for tests that don't involve medicine.
  const emptyMedicineList = [];

  test("Range-based value for 'x' within [1, 3]", () => {
    const variatingValues = { x: [1, 3] };
    const questionText = "Value for x: %%x%%";
    const values = generateValues(questionText, variatingValues, emptyMedicineList);
    expect(values).toHaveProperty("x");
    expect(values.x).toBeGreaterThanOrEqual(1);
    expect(values.x).toBeLessThanOrEqual(3);
  });

  test("Step-based value for 'y' with [2, 10, 2]", () => {
    const variatingValues = { y: [2, 10, 2] };
    const questionText = "Value for y: %%y%%";
    const values = generateValues(questionText, variatingValues, emptyMedicineList);
    const validValues = [2, 4, 6, 8, 10];
    expect(validValues).toContain(values.y);
  });

  test("List selection for 'color'", () => {
    const colors = ["red", "blue", "green"];
    const variatingValues = { color: colors };
    const questionText = "Color: %%color%%";
    const values = generateValues(questionText, variatingValues, emptyMedicineList);
    expect(colors).toContain(values.color);
  });

  test("Automatic name generation for 'name'", () => {
    const namesList = [
      "Alice", "Bob", "Charlie", "Diana", "Elias", "Frida",
      "Gustav", "Hanna", "Isak", "Jasmine", "Kasper", "Lina",
      "Mikael", "Nora", "Oskar", "Petra", "Quentin", "Rebecca",
      "Simon", "Tove", "Ulf", "Vera", "Wilhelm", "Xenia",
      "Yasmine", "Zack"
    ];
    // No variatingValues for 'name', so the function should pick a random name.
    const variatingValues = {};
    const questionText = "Name: %%name%%";
    const values = generateValues(questionText, variatingValues, emptyMedicineList);
    expect(namesList).toContain(values.name);
  });

  test("Condition evaluation: 'x' must be even", () => {
    const variatingValues = { x: [1, 3], condition: "x % 2 === 0" };
    const questionText = "Value for x: %%x%%";
    const values = generateValues(questionText, variatingValues, emptyMedicineList);
    // Only the even number in [1, 3] is 2.
    expect(values.x).toBe(2);
  });

  test("Undefined variable: should not assign a value if variable not found", () => {
    const variatingValues = { x: [1, 3] };  // 'z' is not defined
    const questionText = "Undefined: %%z%%";
    const spyWarn = jest.spyOn(console, "warn").mockImplementation(() => {});
    const values = generateValues(questionText, variatingValues, emptyMedicineList);
    expect(values).not.toHaveProperty("z");
    expect(spyWarn).toHaveBeenCalled();
    spyWarn.mockRestore();
  });

  test("Multiple variables: 'x', 'color', and 'name'", () => {
    const namesList = [
      "Alice", "Bob", "Charlie", "Diana", "Elias", "Frida",
      "Gustav", "Hanna", "Isak", "Jasmine", "Kasper", "Lina",
      "Mikael", "Nora", "Oskar", "Petra", "Quentin", "Rebecca",
      "Simon", "Tove", "Ulf", "Vera", "Wilhelm", "Xenia",
      "Yasmine", "Zack"
    ];
    const colors = ["red", "green", "blue"];
    const variatingValues = { 
      x: [1, 3],
      color: colors
    };
    const questionText = "x: %%x%%, color: %%color%%, name: %%name%%";
    const values = generateValues(questionText, variatingValues, emptyMedicineList);
    expect(values.x).toBeGreaterThanOrEqual(1);
    expect(values.x).toBeLessThanOrEqual(3);
    expect(colors).toContain(values.color);
    expect(namesList).toContain(values.name);
  });

  test("Medicine selection: use 'dose' from medicine data", () => {
    const variatingValues = { medicine: ["Aspirin"] };
    const questionText = "Dose: %%dose%%";
    const medicineList = [
      { namn: "Aspirin", styrkor_doser: '{"dose": [10, 20]}' }
    ];
    const values = generateValues(questionText, variatingValues, medicineList);
    expect(values).toHaveProperty("dose");
    expect(values.dose).toBeGreaterThanOrEqual(10);
    expect(values.dose).toBeLessThanOrEqual(20);
  });

  test("Impossible condition: returns value after max attempts", () => {
    const variatingValues = { x: [1, 3], condition: "x > 3" }; // condition cannot be met
    const questionText = "Value for x: %%x%%";
    const values = generateValues(questionText, variatingValues, emptyMedicineList);
    // Even though the condition is never met, x is still generated within the range.
    expect(values.x).toBeGreaterThanOrEqual(1);
    expect(values.x).toBeLessThanOrEqual(3);
  });

});
