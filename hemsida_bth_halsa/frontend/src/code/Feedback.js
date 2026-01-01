import React, { useState, useEffect } from "react";
import "../css/Feedback.css";

function Feedback() {
  const [feedbackText, setFeedbackText] = useState("");
  const [statusMessage, setStatusMessage] = useState("");
  const [isSuccess, setIsSuccess] = useState(false);
  const [isLoading, setIsLoading] = useState(false);
  const [isVisible, setIsVisible] = useState(false);
  
  const MAX_CHARS = 500;

  useEffect(() => {
    // Add animation when component mounts
    setIsVisible(true);
  }, []);

  const handleSubmit = async (e) => {
    e.preventDefault();
    setStatusMessage("");
    setIsLoading(true);

    try {
      const response = await fetch("/api/feedback", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ feedback_text: feedbackText }),
      });
      const data = await response.json();

      if (data.success) {
        setFeedbackText("");
        setIsSuccess(true);
        setStatusMessage("Tack för din feedback!");
      } else {
        setIsSuccess(false);
        setStatusMessage(data.message || "Något gick fel.");
      }
    } catch (error) {
      setIsSuccess(false);
      setStatusMessage("Fel vid anslutning till servern.");
    } finally {
      setIsLoading(false);
    }
  };

  return (
    <div className={`feedback-container ${isVisible ? 'visible' : ''}`}>
      <div className="feedback-header">
        <h2>Lämna Feedback</h2>
        <p className="feedback-subtitle">Vi värdesätter dina åsikter och förslag</p>
      </div>
      
      <form onSubmit={handleSubmit} className="feedback-form">
        <div className="textarea-wrapper">
          <textarea
            value={feedbackText}
            onChange={(e) => setFeedbackText(e.target.value)}
            placeholder="Skriv din feedback här..."
            maxLength={MAX_CHARS}
            disabled={isLoading}
            style={{ boxSizing: 'border-box' }}
          />
          <div className="character-counter">
            <span className={feedbackText.length > MAX_CHARS * 0.8 ? "warning" : ""}>
              {feedbackText.length}
            </span>/{MAX_CHARS}
          </div>
        </div>
        
        <button 
          type="submit" 
          className={`submit-button ${isLoading ? "loading" : ""}`}
          disabled={isLoading || feedbackText.trim().length === 0}
        >
          {isLoading ? (
            <span className="button-content">
              <span className="spinner"></span>
              <span>SKICKAR...</span>
            </span>
          ) : (
            <span className="button-content">SKICKA</span>
          )}
        </button>
      </form>
      
      {statusMessage && (
        <div
          className={`feedback-message ${isSuccess ? "success" : "error"}`}
        >
          {isSuccess ? (
            <span className="icon">✓</span>
          ) : (
            <span className="icon">!</span>
          )}
          <span>{statusMessage}</span>
        </div>
      )}
    </div>
  );
}

export default Feedback;