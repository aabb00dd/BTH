// frontend/src/RandomQuestion.js
import React, { useState, useEffect, useRef, useMemo } from "react";
import axios from "axios";
import "../css/RandomQuestion.css";

// Custom hook for detecting iOS device
const useIsIOS = () =>
  /iPad|iPhone|iPod/.test(navigator.userAgent) ||
  (navigator.platform === "MacIntel" && navigator.maxTouchPoints > 1);

// Ultra-compact CustomSelect component
const CustomSelect = ({ value, onChange, options, placeholder }) => {
  const [isOpen, setIsOpen] = useState(false);
  const selectRef = useRef(null);
  const isIOS = useIsIOS();

  useEffect(() => {
    const handleClick = (e) =>
      selectRef.current &&
      !selectRef.current.contains(e.target) &&
      setIsOpen(false);
    document.addEventListener("mousedown", handleClick);
    return () => document.removeEventListener("mousedown", handleClick);
  }, []);

  if (!isIOS)
    return (
      <select value={value} onChange={onChange}>
        <option value="">{placeholder}</option>
        {options.map(({ value: v, label }) => (
          <option key={v} value={v}>
            {label}
          </option>
        ))}
      </select>
    );

  return (
    <div className="custom-select" ref={selectRef}>
      <div className="custom-select__trigger" onClick={() => setIsOpen(!isOpen)}>
        {options.find((o) => o.value === value)?.label || placeholder}
      </div>
      {isOpen && (
        <div className="custom-select__options">
          <div
            className="custom-select__option"
            onClick={() => {
              onChange({ target: { value: "" } });
              setIsOpen(false);
            }}
          >
            {placeholder}
          </div>
          {options.map(({ value: v, label }) => (
            <div
              key={v}
              onClick={() => {
                onChange({ target: { value: v } });
                setIsOpen(false);
              }}
              className={`custom-select__option ${value === v ? "selected" : ""}`}
            >
              {label}
            </div>
          ))}
        </div>
      )}
    </div>
  );
};

// HintModal component for displaying image hints
const HintModal = ({
  isOpen,
  onClose,
  currentIndex,
  setCurrentIndex,
  fallbackImage,
}) =>
  !isOpen ? null : (
    <div className="hint-modal-overlay" onClick={onClose}>
      <div className="hint-modal" onClick={(e) => e.stopPropagation()}>
        <span className="hint-close" onClick={onClose}>
          &times;
        </span>
        <h3>Tips</h3>
        <div className={`hint-image-container hint-image-${currentIndex + 1}`}>
          <img
            src={fallbackImage}
            alt={`Hint ${currentIndex + 1}`}
            className="hint-image"
            onError={(e) => {
              e.target.style.display = "block";
            }}
          />
          <div className="hint-pagination">
            {[1, 2, 3].map((_, idx) => (
              <span
                key={idx}
                onClick={(e) => {
                  e.stopPropagation();
                  setCurrentIndex(idx);
                }}
                className={`hint-dot ${currentIndex === idx ? "active" : ""}`}
              />
            ))}
          </div>
          <button
            className="hint-nav prev"
            onClick={(e) => {
              e.stopPropagation();
              setCurrentIndex((currentIndex - 1 + 3) % 3);
            }}
          >
            &#10094;
          </button>
          <button
            className="hint-nav next"
            onClick={(e) => {
              e.stopPropagation();
              setCurrentIndex((currentIndex + 1) % 3);
            }}
          >
            &#10095;
          </button>
        </div>
        <p className="hint-counter">{currentIndex + 1} / 3</p>
      </div>
    </div>
  );

// Improved function to extract medicine names from FASS URLs
const extractMedicineName = (url) => {
  try {
    // First, try to extract from a standard URL format
    if (!url) return "FASS";
    
    // Extract the last part of the URL path
    const urlParts = url.split('/');
    let filename = urlParts.pop() || urlParts.pop(); // Handle trailing slash
    
    // Remove file extension if present
    filename = filename.replace('.html', '').replace('.aspx', '');
    
    // Replace underscores and hyphens with spaces
    let medicineName = filename.replace(/[_-]/g, ' ');
    
    // Decode URI components (for URLs with encoded characters)
    medicineName = decodeURIComponent(medicineName);
    
    // Capitalize first letter of each word
    medicineName = medicineName.split(' ')
      .map(word => word.charAt(0).toUpperCase() + word.slice(1).toLowerCase())
      .join(' ');
    
    return medicineName || "FASS";
  } catch (e) {
    // Return a fallback value if any error occurs
    return "FASS";
  }
};

// New component for displaying FASS links with improved naming
const FassLinksModal = ({ isOpen, onClose, links = [] }) => {
  if (!isOpen || links.length === 0) return null;
  return (
    <div className="fass-modal-overlay" onClick={onClose}>
      <div className="fass-modal" onClick={(e) => e.stopPropagation()}>
        <span className="fass-close" onClick={onClose}>
          &times;
        </span>
        <h3>FASS-länkar</h3>
        <div className="fass-links-list">
          {links.map(({ name, link }, index) => (
            <a
              key={index}
              href={link}
              target="_blank"
              rel="noopener noreferrer"
              className="fass-link-item"
              onClick={onClose}
            >
              {name}
            </a>
          ))}
        </div>
      </div>
    </div>
  );
};

const RandomQuestion = () => {
  const [state, setState] = useState({
    questions: [],
    currentQuestionIndex: 0,
    questionsRight: 0,
    questionData: null,
    userAnswer: "",
    feedback: { message: "", type: "" },
    attempts: 0,
    loading: false,
    courses: [],
    selectedCourse: "",
    showHints: false,
    currentHintIndex: 0,
    showAnswer: false,
    revealedQuestionHintCount: 0,
    numQuestions: 1, // Add new state variable for number of questions
    maxQuestions: 10, // Default maximum questions limit
    courseQuestionCounts: {}, // Add state to track question counts for each course
  });

  const {
    questionData,
    userAnswer,
    feedback,
    attempts,
    loading,
    courses,
    selectedCourse,
    showHints,
    currentHintIndex,
    showAnswer,
    revealedQuestionHintCount,
    numQuestions, // Chosen number of questions
    maxQuestions, // Maximum number of questions
    courseQuestionCounts, // Extract the new state variable
  } = state;

  // Helper to update partial state
  const update = (patch) =>
    setState((prev) => ({
      ...prev,
      ...patch,
    }));

  const API_BASE = "http://localhost:5000/api";
  const fallbackImage = "https://placehold.co/600x400?text=Hint+Image";

  // Prepare course options for the dropdown with question counts
  const courseOptions = useMemo(
    () =>
      courses.map(({ course_code, course_name }) => ({
        value: course_code,
        label: `${course_code}: ${course_name} (${courseQuestionCounts[course_code] || '?'} frågor)`,
      })),
    [courses, courseQuestionCounts]
  );


  const fetchCourses = async () => {
    try {
      const { data } = await axios.get(`${API_BASE}/course/all`);
      const courseQuestionCounts = {};
      data.records.forEach(course => {
        courseQuestionCounts[course.course_code] = course.num_questions || 0;
      });
      update({ 
        courses: data.records,
        courseQuestionCounts
      });
    } catch (err) {
      console.error("Error:", err);
    }
  };


  // Fetch courses on component mount
  useEffect(() => {
    fetchCourses();
  }, [API_BASE])


  const fetchQuestionBlock = async (courseCode) => {
        update({
          loading: true,
          attempts: 0,
          questionData: null,
          questions: [],
          questionsRight: 0,
          currentQuestionIndex: 0,
          userAnswer: "",
          feedback: { message: "", type: "" },
          revealedQuestionHintCount: 0,
          showAnswer: false,
        });
        try {
          const count = Math.min(numQuestions, maxQuestions); // Ensure we don't request more than available
          const { data } = await axios.get(
            `${API_BASE}/question/random?course_code=${courseCode}&count=${count}`
          );
          update({
            questions: data.success ? data.data : null,
            questionData: data.data[0],
            loading: false,
          });
        } catch (error) {
          update({
            questionData: null,
            loading: false,
            feedback: {
              message:
                error.response?.status === 404
                  ? "Ingen fråga hittades för den valda kursen."
                  : "Ett fel uppstod vid hämtning av fråga.",
              type: "incorrect",
            },
          });
        }
      }


  // API functions to fetch courses, questions, and check answers
  const api = useMemo(
    () => ({
      checkAnswer: async (answer, question, attemptCount) => {
        try {
          const { data } = await axios.post(
            `${API_BASE}/question/check-answer`,
            {
              question_id: question.id,
              answer,
              correctAnswer: question.computed_answer,
              correctUnit: question.answer_units,
              formula: question.answer_formula,
              course_code: selectedCourse, // NEW: Pass the selected course code
            }
          );
          update({
            feedback: {
              message: data.message,
              additional_info: data.additional_info,
              type: data.correct ? "correct" : "incorrect",
            },
            attempts: data.correct ? 0 : attemptCount + 1,
          });
        } catch (error) {
          update({
            feedback: {
              message: "Ett fel uppstod vid svarskontrollen.",
              type: "incorrect",
            },
          });
        }
      },
    }),
    [API_BASE, selectedCourse, numQuestions, maxQuestions, courseQuestionCounts] // Add numQuestions, maxQuestions, and courseQuestionCounts to dependencies
  );



  // Event handlers
  // Update course change handler to fetch question count when course changes
  const handleCourseChange = (e) => {
    const courseCode = e.target.value;
    update({
      selectedCourse: courseCode,
      maxQuestions: courseQuestionCounts[courseCode] || 10
    });
  };


  const handleAnswerChange = (e) => update({ userAnswer: e.target.value });
  const toggleHints = () =>
    update({
      showHints: !showHints,
      currentHintIndex: !showHints ? 0 : currentHintIndex,
    });

  const handleFetchQuestion = () => {
    if (!selectedCourse) {
      return update({
        feedback: { message: "Välj en kurs först.", type: "incorrect" },
      });
    }
    fetchQuestionBlock(selectedCourse);
  };

  const handleSubmit = (e) => {
    e.preventDefault();
    if (questionData) api.checkAnswer(userAnswer, questionData, attempts);
  };

  const toggleAnswer = () => update({ showAnswer: !showAnswer });

  const handleNextQuestion = () => {
    let nextQuestionsRight = state.questionsRight;
    // If the last answer was correct, increment before showing summary
    if (state.feedback.type === "correct") {
      nextQuestionsRight += 1;
    }

    if (state.currentQuestionIndex + 1 < state.questions.length) {
      update({
        currentQuestionIndex: state.currentQuestionIndex + 1,
        questionData: state.questions[state.currentQuestionIndex + 1],
        userAnswer: "",
        feedback: { message: "", type: "" },
        revealedQuestionHintCount: 0,
        showAnswer: false,
        questionsRight: nextQuestionsRight,
      });
    } else {
      // When there are no more questions left
      let message = "";
      const correctCount = nextQuestionsRight;
      const totalCount = state.questions.length;
      if (correctCount === totalCount) {
        message += "Fantastiskt!\nDu har fått rätt på alla frågor!\n";
      } else {
        message += "Bra jobbat!\n";
        message += `Du har svarat rätt på ${correctCount} av ${state.currentQuestionIndex + 1} frågor.\n`;
      }

      message += "Vill du börja om med en ny uppsättning frågor?\n";
      message += "(Du kan änra antalet frågor innan du trycker fortsätt öva om du vill.)\n";
      message += "(SORRY MEN DU MÅSTE TRYCKA PÅ REFRESH FÖR ATT BÖRJA OM.)\n";
      questionData.question = message;

      update({
        userAnswer: state.numQuestions,
        feedback: { message: "Klar!", type: "done" },
        revealedQuestionHintCount: 0,
        showAnswer: false,
        questionsRight: 0,
      });
    }
  };

  const handleShowNextQuestionHint = () => {
    if (questionData && questionData.hints) {
      const totalHints = questionData.hints.length;
      if (revealedQuestionHintCount < totalHints) {
        update({ revealedQuestionHintCount: revealedQuestionHintCount + 1 });
      }
    }
  };

  // Update handler for changing number of questions with max limit
  const handleNumQuestionsChange = (e) => {
    const value = e.target.value;
    // Allow empty string so user can clear the field
    if (value === "") {
      update({ numQuestions: "" });
      return;
    }

    const num = parseInt(value, 10);
    update({ numQuestions: Math.max(1, Math.min(maxQuestions, num)) }); // Limit to maxQuestions
  };

  // Add state for FASS links modal
  const [showFassLinks, setShowFassLinks] = useState(false);
  
  // Function to extract medicine links from question data
  const getMedicineLinks = () => {
    if (!questionData || !questionData.medicine_link) return [];
    if (Array.isArray(questionData.medicine_link)) {
      // Not expected, but fallback
      return questionData.medicine_link.map(link => ({ name: extractMedicineName(link), link }));
    } else if (typeof questionData.medicine_link === 'object') {
      // Dictionary: { name: link }
      return Object.entries(questionData.medicine_link).map(([name, link]) => ({
        name,
        link: link || "https://www.fass.se"
      }));
    } else if (typeof questionData.medicine_link === 'string') {
      return [{ name: extractMedicineName(questionData.medicine_link), link: questionData.medicine_link }];
    } else {
      return [{ name: "FASS", link: "https://www.fass.se" }];
    }
  };

  // Resource buttons for additional tools.
  const resourceButtons = [
    {
      type: "link",
      href: "https://www.desmos.com/fourfunction?lang=sv-SE",
      icon: "🧮",
      text: "Miniräknare",
      className: "calculator-btn",
    },
    {
      type: "button", // Explicitly mark as button type
      onClick: () => setShowFassLinks(true),
      icon: "💊",
      text: "FASS",
      className: "fass-btn",
    },
    {
      type: "button", // Explicitly mark as button type
      onClick: toggleHints,
      icon: "💡",
      text: "Tips",
      className: "hint-resource-btn",
    },
  ];

  return (
    <div className="random-question-container">
      <h2>Slumpmässig Fråga</h2>

      <div className="course-selector">
        <CustomSelect
          value={selectedCourse}
          onChange={handleCourseChange}
          placeholder="-- Välj en kurs --"
          options={courseOptions}
        />
        
        {/* Update number of questions input to show max limit */}
        <div className="question-count-selector">
          <label>Antal frågor</label>
          <input
            type="number"
            min="1"
            max={maxQuestions}
            value={numQuestions}
            onChange={handleNumQuestionsChange}
            className="question-count-input"
          />
        </div>
      </div>

      {!questionData && (
        <>
          <button className="btn-primary start-button" onClick={handleFetchQuestion}>
            {loading ? "Laddar..." : "Kom Igång"}
          </button>
          
          {/* Display error message under the button when there's no question data */}
          {feedback.message && (
            <div className={`answer-display ${feedback.type} start-button-error`}>
              <p>{feedback.message}</p>
            </div>
          )}
        </>
      )}

      {loading && <div className="loading-spinner" />}

      {questionData && (
        <div className="question-display">
          {/* Question content wrapper */}
          <div className="question-content-wrapper">
            <p>
              {questionData.question.split('\n').map((line, idx) => (
                <React.Fragment key={idx}>
                  {line}
                  {idx < questionData.question.split('\n').length - 1 && <br />}
                </React.Fragment>
              ))}
            </p>
            
            <form className="answer-form" onSubmit={handleSubmit}>
              <input
                type="text"
                value={userAnswer}
                onChange={handleAnswerChange}
                placeholder="Ange ditt svar"
                required
              />
              <div className="button-group">
                <button
                  type="button"
                  onClick={toggleAnswer}
                  className="next-button"
                >
                  Visa Svar
                </button>
                <button
                  // If correct, act as a normal button; otherwise as a submit
                  type={state.feedback.type === 'correct' ? 'button' : 'submit'}
                  className={'next-button' + (state.feedback.type === 'correct' ? ' next-complete' : '')}
                  onClick={ (e) => {
                    if (state.feedback.type === 'correct'){
                      e.preventDefault(); 
                      handleNextQuestion();
                    }
                  }}>

                  {state.feedback.type === 'correct' ? 'Nästa Fråga' : 'Rätta Mitt Svar'}
                </button>
              </div>
              <button type="button" className="skip-button" onClick={handleNextQuestion}>
                  Skippa Fråga
              </button>
            </form>

            {/* Separated feedback and answer displays */}
            {feedback.message && (
              <div className={`answer-display ${feedback.type}`}>
                <p>{feedback.message}</p>
                <p>{feedback.additional_info}</p>
              </div>
            )}

            {showAnswer && state.feedback.type !== 'correct' && (
              <div className="answer-display">
                <p>
                  {(() => {
                    const raw = questionData.computed_answer;
                    if (raw == null) {
                      return "Rätt svar: -";
                    }
                    const num = raw;
                    const precision = Number.isInteger(questionData.answer_units.precision) ? questionData.answer_units.precision : 2;
                    const formattedNum = parseFloat(num.toFixed(precision));
                    let unit = "";
                    if ( questionData.answer_units && questionData.answer_units.accepted_answer) {
                      try {
                        const parsedUnits = JSON.parse( questionData.answer_units.accepted_answer);
                        unit = Array.isArray(parsedUnits) ? parsedUnits[0] : parsedUnits;
                      } catch (e) {
                        unit = questionData.answer_units.accepted_answer;
                      }
                    }
                    return `Rätt svar: ${formattedNum} ${unit}`;
                    })()}
                  </p>
                  </div>
                )}
                </div>
                
                {/* Hints container with resource buttons */}
                <div className="question-hints-container">
                  <h4>Frågehints</h4>

                  {(!questionData.hints || questionData.hints.length === 0) ? (
                    <p className="hint-progress">Inga hints tillgängliga.</p>
                  ) : (
                    <>
                      {questionData.hints
                        .slice(0, revealedQuestionHintCount)
                        .map((hint, idx) => (
                          <div key={idx} className="question-hint">
                            {hint}
                          </div>
                        ))}

                      {revealedQuestionHintCount < questionData.hints.length ? (
                        <>
                          <button
                            onClick={handleShowNextQuestionHint}
                            className="show-question-hint-btn"
                          >
                            Visa nästa hint ({revealedQuestionHintCount + 1}/
                            {questionData.hints.length})
                          </button>
                          <div className="question-hint-progress-bar">
                            <div
                              className="question-hint-progress-bar-filled"
                              style={{
                                width: `${
                                  (revealedQuestionHintCount / questionData.hints.length) *
                                  100
                                }%`,
                              }}
                            />
                          </div>
                        </>
                      ) : (
                        <p className="hint-progress">Alla hints är visade.</p>
                      )}
                    </>
                  )}

                  {/* Resource buttons (always shown) */}
                  <div className="resource-buttons hints-resource-buttons">
                    {resourceButtons.map((resource, i) =>
                      resource.type === "link" ? (
                        <a
                          key={i}
                          href={resource.href}
                          target="_blank"
                          rel="noopener noreferrer"
                          className={`resource-btn ${resource.className}`}
                        >
                          <span className="resource-icon">{resource.icon}</span>
                          <span className="resource-text">{resource.text}</span>
                        </a>
                      ) : (
                        <button
                          key={i}
                          type="button"
                          onClick={resource.onClick}
                          className={`resource-btn ${resource.className}`}
                        >
                          <span className="resource-icon">{resource.icon}</span>
                          <span className="resource-text">{resource.text}</span>
                        </button>
                      )
                    )}
                  </div>
                </div>
              </div>
            )}

              {state.questions.length > 0 && (
              <div className="counters-container">
                {/* Always show which question you're on */}
          <div className="question-counter">
            Fråga {state.currentQuestionIndex + 1} av {state.questions.length}
          </div>

          {/* Show correct‐counter as soon as questionsRight ≥ 1 */}
          {state.questionsRight > 0 && (
            <div className="correct-counter">
              Antal Rätt {state.questionsRight} av{" "}
              {Math.max(state.currentQuestionIndex, 1)}
            </div>
          )}
        </div>
      )}


      <FassLinksModal 
        isOpen={showFassLinks}
        onClose={() => setShowFassLinks(false)}
        links={getMedicineLinks()}
      />

      <HintModal
        isOpen={showHints}
        onClose={toggleHints}
        currentIndex={currentHintIndex}
        setCurrentIndex={(idx) => update({ currentHintIndex: idx })}
        fallbackImage={fallbackImage}
      />
    </div>
  );
};

export default RandomQuestion;
