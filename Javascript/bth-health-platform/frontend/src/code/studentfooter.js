import React from "react";
import { useNavigate } from "react-router-dom";
import "../css/FooterMenuBar.css";

export default function FooterMenuBar() {
  const currentYear = new Date().getFullYear();
  const navigate = useNavigate();
  
  const handleAboutUsClick = (e) => {
    e.preventDefault();
    // Set localStorage directly (this is what MenuBar.js does)
    localStorage.setItem('activeTab', 'aboutus');
    
    // Dispatch a custom event that the parent component can listen for
    window.dispatchEvent(new CustomEvent('tabChange', { 
      detail: { tab: 'aboutus' } 
    }));
    
    // Refresh the current page or navigate to the student page
    if (window.location.pathname === '/student') {
      window.location.reload();
    } else {
      navigate('/student');
    }
  };

  const handleFeedbackClick = (e) => {
    e.preventDefault();
    // Set localStorage directly (this is what MenuBar.js does)
    localStorage.setItem('activeTab', 'feedback');
    
    // Dispatch a custom event that the parent component can listen for
    window.dispatchEvent(new CustomEvent('tabChange', { 
      detail: { tab: 'feedback' } 
    }));
    
    // Refresh the current page or navigate to the student page
    if (window.location.pathname === '/student') {
      window.location.reload();
    } else {
      navigate('/student');
    }
  };

  return (
    <footer className="footer-menu-bar" role="contentinfo">
      <div className="footer-content">
        <span className="footer-menu-text">
          © {currentYear} Läkemedelsberäkningar. Alla rättigheter förbehållna.
        </span>
        <div className="footer-links">
          <button 
            onClick={handleAboutUsClick} 
            className="footer-link" 
            aria-label="Om Oss"
          >
            Om Oss
          </button>
          <button 
            onClick={handleFeedbackClick} 
            className="footer-link" 
            aria-label="Lämna Feedback"
          >
            Lämna Feedback
          </button>
        </div>
      </div>
    </footer>
  );
}