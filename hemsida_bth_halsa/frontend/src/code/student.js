// frontend/src/App.js
import React, { useState, useEffect } from "react";
import { useLocation } from "react-router-dom";
import "../css/App.css";

import RandomQuestion from "./RandomQuestion";
import AboutUs from "./AboutUs";
import MenuBar from "./MenuBar";
import StudentFooter from "./studentfooter";
import Feedback from "./Feedback";

function App() {
  const location = useLocation();
  const [activeTab, setActiveTab] = useState(() => {
    // Only use localStorage for state persistence
    return localStorage.getItem("activeTab") || "home";
  });

  useEffect(() => {
    // Save that we're on the student app
    localStorage.setItem("lastPath", "/student");
    
    if (location.state && location.state.activeTab) {
      setActiveTab(location.state.activeTab);
      localStorage.setItem("activeTab", location.state.activeTab);
    }
  }, [location.state]);

  const handleTabChange = (tab) => {
    setActiveTab(tab);
    localStorage.setItem("activeTab", tab);
  };

  const renderContent = () => {
    switch(activeTab) {
      case "random":
        return <RandomQuestion />;
      case "aboutus":
        return <AboutUs />;
      case "feedback":
        return <Feedback />;
      case "home":
      default:
        return (
          <div className="welcome-container">
            <div className="home-content">
              <h1>Välkommen till Läkemedelsberäkningar</h1>
              <p className="welcome-description">En plattform för att öva och förbättra dina kunskaper inom läkemedelsberäkning.</p>
              <div className="feature-grid">
                <div className="feature-item">
                  <div className="feature-icon">📝</div>
                  <h3>Träna</h3>
                  <p>Öva på olika typer av läkemedelsberäkningar</p>
                </div>
                <div className="feature-item">
                  <div className="feature-icon">🔍</div>
                  <h3>Utvärdera</h3>
                  <p>Testa dina kunskaper och se din utveckling</p>
                </div>
                <div className="feature-item">
                  <div className="feature-icon">📚</div>
                  <h3>Lär</h3>
                  <p>Fördjupa dina kunskaper inom området</p>
                </div>
              </div>
              <button className="start-button" onClick={(e) => {
                e.preventDefault();
                // Set localStorage directly
                localStorage.setItem('activeTab', 'random');
                
                // Dispatch a custom event that the parent component can listen for
                window.dispatchEvent(new CustomEvent('tabChange', { 
                  detail: { tab: 'random' } 
                }));
                
                // Force a refresh to ensure everything updates
                window.location.reload();
              }}>
                Börja Nu
              </button>
            </div>
          </div>
        );
    }
  };

  return (
    <div className="App">
      <MenuBar 
        setActiveTab={handleTabChange} 
        activeTab={activeTab} 
        menuItems={[
          { tab: "home", label: "Hem" },
          { tab: "random", label: "Slumpmässig Fråga" }
        ]}
      />
      <div className="content">
        <div className="content-transition-wrapper">
          {renderContent()}
        </div>
      </div>
      <StudentFooter />
    </div>
  );
}

export default App;