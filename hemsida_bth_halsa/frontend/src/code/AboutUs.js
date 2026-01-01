import React from "react";
import "../css/AboutUs.css";

const TeamMemberCard = ({ member }) => (
  <div className="team-member-card">
    <div className="member-image-container">
      <div 
        className={`member-image member-image-${member.id}`}
        aria-label={`Photo of ${member.name}`}
      />
    </div>
    <div className="member-info">
      <h3>{member.name}</h3>
      <p className="member-role">{member.role}</p>
      <div className="member-contact">
        <p>
          <span className="contact-icon">✉️</span> 
          <a href={`mailto:${member.email}`} title={`Send email to ${member.name}`}>{member.email}</a>
        </p>
        <p>
          <span className="contact-icon">📞</span> 
          <a href={`tel:${member.phone.replace(/\s+/g, '')}`} title={`Call ${member.name}`}>{member.phone}</a>
        </p>
      </div>
    </div>
  </div>
);

const AboutUs = () => {
  const teamMembers = [
    {
      id: 1,
      name: "Abdalrahman Mohammed",
      role: "Projektledare/Frontend-utvecklare",
      email: "abmm22@student.bth.se",
      phone: "070-022 03 60"
    },
    {
      id: 2,
      name: "Oscar Wiktorin",
      role: "Backend-utvecklare/Systemarkitekt",
      email: "osai20@student.bth.se",
      phone: "070-770 09 54"
    },
    {
      id: 3,
      name: "Elias Karlsson",
      role: "Testare/Backend-utvecklare",
      email: "elka22@student.bth.se",
      phone: "070-589 64 81"
    },
    {
      id: 4,
      name: "Ali Reza Hussaini",
      role: "Inloggning & Hosting Ansvarig",
      email: "alhu22@student.bth.se",
      phone: "073-957 80 52"
    }
  ];

  return (
    <div className="about-us-container">
      <h1>Om Oss</h1>
      <p className="about-us-intro">
        Vi är ett team av engagerade personer som arbetar för att förbättra läkemedelsberäkningar 
        för sjuksköterskestudenter. Vårt mål är att skapa ett effektivt verktyg för inlärning.
      </p>
      
      <div className="team-grid">
        {teamMembers.map(member => (
          <TeamMemberCard key={member.id} member={member} />
        ))}
      </div>
    </div>
  );
};

export default AboutUs;
