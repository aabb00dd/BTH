// backend/routes/feedbackRoutes.js
const express = require("express");
const router = express.Router();
const { addFeedback, getAllFeedback, deleteFeedback } = require("../models/feedbackModel");

/**
 * Create new feedback
 */
router.post("/", async (req, res) => {
  const { feedback_text } = req.body;

  if (!feedback_text) {
    return res
      .status(400)
      .json({ success: false, message: "Feedback text is required." });
  }

  try {
    const { id } = await addFeedback(feedback_text);
    return res.status(201).json({
      success: true,
      message: "Feedback mottaget!",
      feedbackId: id
    });
  } catch (error) {
    console.error("Error adding feedback:", error);
    return res
      .status(500)
      .json({ success: false, message: "Database error. Could not add feedback." });
  }
});

/**
 * Retrieve all feedback (newest first).
 */
router.get("/all", async (req, res) => {
  try {
    const allFeedback = await getAllFeedback();
    return res.json({ success: true, records: allFeedback });
  } catch (error) {
    console.error("Error retrieving feedback:", error);
    return res
      .status(500)
      .json({ success: false, message: "Failed to retrieve feedback." });
  }
});

/**
 * Delete feedback by ID
 */
router.delete("/delete/:id", async (req, res) => {
  try {
    const feedbackId = parseInt(req.params.id, 10);
    if (!feedbackId) {
      return res.status(400).json({ success: false, message: "Invalid feedback ID." });
    }
    const changes = await deleteFeedback(feedbackId);
    if (changes === 0) {
      return res.status(404).json({ success: false, message: "Feedback not found." });
    }
    return res.json({ success: true, message: "Feedback raderad." });
  } catch (error) {
    console.error("Error deleting feedback:", error);
    return res
      .status(500)
      .json({ success: false, message: "Database error. Could not delete feedback." });
  }
});

module.exports = router;