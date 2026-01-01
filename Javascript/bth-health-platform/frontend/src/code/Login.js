// src/components/Log.js
import React from "react";
import "../css/login.css";
import msalInstance from "../msalInstance";

  

async function switchMode(role) {
  
  let authority;
  if (role === "student") {
    authority = "https://login.microsoftonline.com/229ecd8f-0027-4293-bb33-19cab7e6b83d/";
  } else if (role === "employee") {
    authority = "https://login.microsoftonline.com/885b11db-fb41-4d7a-85b0-39967c789c5a/";
  } else {
    authority = "https://login.microsoftonline.com/common/";
  }

  
  await msalInstance.initialize();
  const lastLogin = sessionStorage.getItem("lastLoginTime");
  const now = Date.now();
  const oneMinute = 60 * 60000;

  let promptType = "none"; // Try silent first

  if (!lastLogin || now - parseInt(lastLogin) > oneMinute) {
    // ⏰ More than 1 minute has passed – force login
    promptType = "login";
  }

  const request = {
    scopes: ['user.read'],
    prompt: promptType,
    authority: authority, // ✅ override here safely

  };

  let response;
  try {
    response = await msalInstance.loginPopup(request);
    sessionStorage.setItem("lastLoginTime", now.toString());
  } catch (error) {
    console.error("Login failed:", error);
    alert("Login failed. Please try again.");
    return;
  }

  const account = response.account;
  msalInstance.setActiveAccount(account);
  
  const userEmail = account.username || account.userName;
  
  // if useremail ends with student.bth.se
  if (userEmail.endsWith("@bth.se") || userEmail.startsWith("alhu22")) {
    console.log("User is an employee");
    window.location.href = "/admin";
  } else if (userEmail.endsWith("@student.bth.se")) {
    console.log("User is a student");
    window.location.href = "/student";
  } else {
    alert("Du är inte en student eller anställd vid BTH. Vänligen kontakta supporten.");
  }

}

const Log = () => {
  const currentYear = new Date().getFullYear();
  
  return (
    <div className="log-container">
      <div className="log-header">
        <h1 className="log-title">Läkemedelsberäkningar</h1>
      </div>
      <div className="log-buttons">
        <button type="button" className="log-button student" onClick={() => switchMode("student")}>
          <span className="button-icon"></span>
          <span className="button-text">Student vid BTH / Student at BTH</span>
        </button>
        <button type="button" className="log-button employee" onClick={() => switchMode("employee")}>
          <span className="button-icon"></span>
          <span className="button-text">Anställd vid BTH / Employee at BTH</span>
        </button>
        <button type="button" className="log-button other" onClick={() => switchMode("other")}>
          <span className="button-icon"></span>
          <span className="button-text">Övriga användare / Other users</span>
        </button>
      </div>
      <footer className="log-footer">
        <p>© {currentYear} Läkemedelsberäkningar. Alla rättigheter förbehållna.</p>
      </footer>
    </div>
  );
};

// export Log and ProtectedRoute
export { Log };
export default Log;