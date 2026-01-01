import axios from "axios";

const API_BASE_URL = "http://localhost:5000/api";

const fetchCourses = async () => {
    try {
        const { data } = await axios.get(`${API_BASE_URL}/course/all`);
        return data.records;
    } catch (err) {
        console.error("Error:", err);
    }
};

const fetchQtype = async () => {
    try {
        const { data } = await axios.get(`${API_BASE_URL}/qtype/all`);
        return data.records;
    } catch (err) {
        console.error("Error:", err);
    }
};

const fetchUnits = async () => {
    try {
        const { data } = await axios.get(`${API_BASE_URL}/units/all`);
        return data.records;
    } catch (err) {
        console.error("Error:", err);
    }
};

const fetchQuestion = async (qID) => {
      try {
        const res = await axios.get(`${API_BASE_URL}/questions/${qID}`);
        return res.data;
      } catch (err) {
        console.error("Error loading question", err);
      }
    };

export default {
    fetchCourses,
    fetchQtype,
    fetchQuestion,
    fetchUnits
};
