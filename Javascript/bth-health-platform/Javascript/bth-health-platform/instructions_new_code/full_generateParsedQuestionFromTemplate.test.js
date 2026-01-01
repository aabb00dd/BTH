it('sets %%name%% value as the answer field', () => {
  const row = {
    question: "Vad heter patienten? %%name%%",
    variating_values: {},
    question_data: [{
      question: "Vad heter patienten? %%name%%"
    }]
  };

  const result = generateParsedQuestionFromTemplate(row);
  const values = result.question_data[0].generated_values;

  expect(values.name).toBeDefined();
  expect(values.answer).toBe(values.name);
});

const { generateParsedQuestionFromTemplate } = require("../helpers/questionHelpers");

it('attempts to call method-style expression: medicine[0].calcDose(weight)', () => {
  const row = {
    question: "Dos: %%medicine[0].calcDose(weight)%%",
    variating_values: {
      "medicine.namn": ["Morfin"],
      "weight": [60]
    },
    question_data: [{
      question: "Dos: %%medicine[0].calcDose(weight)%%"
    }]
  };

  const result = generateParsedQuestionFromTemplate(row);
  const values = result.question_data[0].generated_values;

  // At this stage the system is not expected to support method calls
  expect(values["medicine[0].calcDose(weight)"]).toMatch(/\[ERROR:/);
});

const { generateParsedQuestionFromTemplate } = require("../helpers/questionHelpers");

it('generates accepted_answers for Swedish character input', () => {
  const row = {
    question: "Vilken medicin? %%medicine[0]%%",
    variating_values: {
      "medicine.namn": ["Pär"]
    },
    question_data: [{
      question: "Vilken medicin? %%medicine[0]%%"
    }]
  };

  const result = generateParsedQuestionFromTemplate(row);
  const values = result.question_data[0].generated_values;

  expect(values.answer.toLowerCase()).toBe("pär");
  expect(values.accepted_answers).toContain("pär");
  expect(values.accepted_answers.length).toBeLessThanOrEqual(11);
  expect(values.accepted_answers.every(a => typeof a === "string")).toBe(true);
});

