// import React, { useState, useEffect, useCallback } from "react";
// import axios from "axios";
// import "../css/AddQuestion.css";

// import { fetchCourses, fetchQtype, fetchUnits,  fetchQuestion } from "./code/helpers/fetchHelpers";
// import { units } from "../../../backend/db/database_dict";

// const API_BASE_URL = "http://localhost:5000/api";

// const AddEditQuestion = () => { 

//   const [state, setState] = useState({
//     questionID : "",
//     courses: {},
//     qtypes: {},
//     units: {},
//     question_text: "",
//     answer_formula: "",
//     answer_unit_id: "",
//     variating_values: "",
//     course_codes: "",
//     question_type_id: "",
//     hints: "",

//   });

//   const update = (patch) =>
//   setState((prev) => ({
//     ...prev,
//     ...patch,
//   }));



//   // Fetch courses on component mount
//   useEffect(() => {
//     update(
//       { courses: fetchCourses(),
//         qtypes: fetchQtype(),
//         units: fetchUnits()
//       }
//     )
//   }, [API_BASE_URL])
  



//   // Load-knappens händelse
//   const handleLoadClick = () => {
//     const question_data = fetchQuestion(state.questionID);
//     update({
//       question_text: question_data.question,
//       answer_formula: question_data.answer_formula,
//       answer_unit_id: question_data.answer_unit_id,
//       variating_values: question_data.variating_values,
//       course_codes: question_data.course_codes,
//       question_type_id: question_data.question_type_id,
//       hints: question_data.hints
//     }
//     )

//   };

//   handleFormChange = (field, value) => {
//     update({ [field]: value });
//   }

  

//   return (
//     <div className="add-question">
//       <h2>{editingId ? "Ändra Fråga" : "Lägg Till En Ny Fråga"}</h2>

//       {/* ID-input och load-knapp istället för dropdown */}
//       <div className="load-section">
//         <label htmlFor="loadId">Fråge-ID:</label>
//         <input
//           id="loadId"
//           type="text"
//           value={loadId}
//           onChange={e => update({questionID: e.target.value})}
//           placeholder="Ange fråga-id"
//         />
//         <button type="button" onClick={handleLoadClick}>
//           Ladda Fråga
//         </button>
//       </div>

//       <form onSubmit={handleSubmit}>
//         <fieldset>
//           <legend>Fråga Detaljer</legend>
//           <div>
//             <label>Fråga</label>
//             <input
//               type="text"
//               value={state.question_text}

//             />
//           </div>
//           <div>
//             <label>Svars Formel</label>
//             <input
//               type="text"
//               value={state.answer_formula}
//             />
//             <label>Enhet (Visa mer)</label>
//             <input
//               type="text"
//               value={state.answer_unit_id}
//             />
//           </div>
//           </fieldset>
//           <div>
//             <label>Kommentar (variating_values)</label>
//             <textarea
//               value={formData.variatingValues}
//               onChange={e => handleFormChange("variatingValues", e.target.value)}
//             />
//           </div>
//           <div>
//             <label>Enhets-ID (answer_unit_id)</label>
//             <input
//               type="number"
//               value={formData.answerUnitId}
//               onChange={e => handleFormChange("answerUnitId", e.target.value)}
//             />
//           </div>
//           <div>
//             <label>Hints (en rad per hint)</label>    
//             <textarea
//               value={formData.hints}
//               onChange={e => handleFormChange("hints", e.target.value)}
//             />
//           </div>
//         </fieldset>
//         <fieldset>
//           <legend>Kurs &amp; Frågetyp</legend>
//           <div>
//             <label>Kurs</label>
//             <select
//               multiple
//               value={formData.courses}
//               onChange={e => handleFormChange(
//                 "courses",
//                 Array.from(e.target.selectedOptions, o => o.value)
//               )}
//             >
//               {courses.map(c => (
//                 <option key={c.course_code} value={c.course_code}>
//                   {c.course_code}
//                 </option>
//               ))}
//             </select>
//           </div>
//           <div>
//             <label>Frågetyp</label>
//             <select
//               value={formData.questionType}
//               onChange={e => handleFormChange("questionType", e.target.value)}
//             >
//               <option value="">Välj En Frågetyp</option>
//               {qtypes.map(qt => (
//                 <option key={qt.id} value={qt.id}>
//                   {qt.name}
//                 </option>
//               ))}
//             </select>
//           </div>
//         </fieldset> 
//         <button type="submit">
//           {editingId ? "Spara Ändringar" : "Lägg Till Fråga"}
//         </button>
//       </form>
//     </div>
//   );
// };

// export default AddEditQuestion;