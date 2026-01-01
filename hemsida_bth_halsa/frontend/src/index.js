import React from "react";
import ReactDOM from "react-dom/client";
import { BrowserRouter as Router, Routes, Route, Navigate } from "react-router-dom";

import "./css/index.css";

import App from "./App";
import MiniApp from "./code/student";
// import Login from "./Login";
import Log from "./code/Login";
import msalInstance from "./msalInstance";

const ProtectedRoute = ({ children}) => {
  const account = msalInstance.getActiveAccount();

  if (!account) {
    return <Navigate to="/" replace />;
  }

  const email = account.username || account.userName || "";

  const isStudent = email.endsWith("@student.bth.se");
  const admin = email.startsWith("alhu22") || email.startsWith("abmm22") || email.startsWith("osai20");

  if (isStudent && window.location.pathname === "/admin" && !admin) {
    return <Navigate to="/" replace />;
  }

  return children;
};


const root = ReactDOM.createRoot(document.getElementById("root"));
root.render(
  <React.StrictMode>
    <Router>
      <Routes>
        <Route path="/" element={<Log />} />
        <Route path="/admin" element={
            <ProtectedRoute>
              <App />
            </ProtectedRoute>
          }
        />
        <Route path="/student" element={
            <ProtectedRoute>
              <MiniApp />
            </ProtectedRoute>
          }
        />
        <Route path="*" element={<Navigate to="/" replace />} />
      </Routes>
    </Router>
  </React.StrictMode>
);