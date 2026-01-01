// frontend/src/App.js
import React, { useState, useEffect } from "react";
import { useLocation } from "react-router-dom";
import "./css/App.css";

import AddEditQuestion from "./code/AddEditQuestion";
import RandomQuestion from "./code/RandomQuestion";
import AboutUs from "./code/AboutUs";
import DataOverview from "./code/DataOverview";
import DataVis from "./code/DataVis";
import MenuBar from "./code/MenuBar";
import FooterMenuBar from "./code/FooterMenuBar";
import Feedback from "./code/Feedback";

function App() {
  const location = useLocation();
  const [activeTab, setActiveTab] = useState(() => {
    // Only use localStorage for state persistence
    return localStorage.getItem("activeTab") || "home";
  });

  useEffect(() => {
    // Save that we're on the admin app
    localStorage.setItem("lastPath", "/admin");
    
    // Handle state from navigation (useLocation)
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
      case "upload":
        return <AddEditQuestion />;
      case "random":
        return <RandomQuestion />;
      case "aboutus":
        return <AboutUs />;
      case "data":
        return <DataOverview />;
      case "stat":
        return <DataVis />;
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
                // Set localStorage directly (using exact same code as in FooterMenuBar.js)
                localStorage.setItem('activeTab', 'random');
                
                // Dispatch a custom event that the parent component can listen for
                window.dispatchEvent(new CustomEvent('tabChange', { 
                  detail: { tab: 'random' } 
                }));
                
                // Force a page reload - this seems to be necessary based on footer implementation
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
      <MenuBar setActiveTab={handleTabChange} activeTab={activeTab} />
      <div className="content">
        <div className="content-transition-wrapper">
          {renderContent()}
        </div>
      </div>
      <FooterMenuBar />
    </div>
  );
}

export default App;