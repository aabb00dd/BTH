// frontend/src/MenuBar.js
import React, { useState, useEffect, useRef } from "react";
import "../css/MenuBar.css";

export default function MenuBar({ setActiveTab, menuItems }) {
  const [isTablet, setIsTablet] = useState(window.innerWidth <= 1024);
  const [menuOpen, setMenuOpen] = useState(false);
  // Initialize activeTabState from localStorage or default to "home"
  const [activeTabState, setActiveTabState] = useState(() => {
    const savedTab = localStorage.getItem('activeTab');
    return savedTab || "home";
  });
  const menuRef = useRef(null);
  
  useEffect(() => {
    const handleResize = () => {
      setIsTablet(window.innerWidth <= 1024);
      if (window.innerWidth > 1024) {
        setMenuOpen(false);
      }
    };
    window.addEventListener("resize", handleResize);
    return () => window.removeEventListener("resize", handleResize);
  }, []);
  
  useEffect(() => {
    const handleClickOutside = (event) => {
      if (menuRef.current && !menuRef.current.contains(event.target) && menuOpen) {
        setMenuOpen(false);
      }
    };
    document.addEventListener("mousedown", handleClickOutside);
    return () => document.removeEventListener("mousedown", handleClickOutside);
  }, [menuOpen]);

  // Use useEffect to set active tab on component mount
  useEffect(() => {
    setActiveTab(activeTabState);
  }, [setActiveTab, activeTabState]);

  const handleTabClick = (tab) => {
    setActiveTab(tab);
    setActiveTabState(tab); // Update active tab state
    // Save to localStorage
    localStorage.setItem('activeTab', tab);
    if (isTablet) {
      setMenuOpen(false);
    }
  };

  const defaultItems = [
    { tab: "home", label: "Hem" },
    { tab: "upload", label: "Lägg Till Frågor" },
    { tab: "random", label: "Slumpmässig Fråga" },
    { tab: "data", label: "Dataöversikt" },
    { tab: "stat", label: "Statistik" }
  ];
  const items = menuItems || defaultItems;

  return (
    <div className="menu-bar" ref={menuRef}>
      <div className="menu-header">
        <span className="menu-title">Läkemedelsberäkningar</span>
        {isTablet && (
          <button 
            className={`menu-button ${menuOpen ? 'active' : ''}`}
            onClick={() => setMenuOpen(!menuOpen)}
            aria-label="Toggle menu"
            aria-expanded={menuOpen}
          >
            Meny
          </button>
        )}
      </div>
      <div 
        className={`menu-buttons ${isTablet ? 'mobile' : ''} ${menuOpen ? 'open' : ''}`}
        aria-hidden={isTablet && !menuOpen}
      >
        {items.map((item, index) => (
          <React.Fragment key={item.tab}>
            {!isTablet && <span className="menu-separator">|</span>}
            <button 
              className={`menu-item ${activeTabState === item.tab ? 'active' : ''}`}
              onClick={() => handleTabClick(item.tab)}
              style={{"--item-index": index}}
            >
              {item.label}
            </button>
          </React.Fragment>
        ))}
        {!isTablet && <span className="menu-separator">|</span>}
      </div>
    </div>
  );
}