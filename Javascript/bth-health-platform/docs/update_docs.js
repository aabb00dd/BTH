// generateTree.js
const fs = require("fs");
const path = require("path");

const OUTPUT_FILE = "docs/README.md";
const ROOT_DIR = path.join(__dirname, "../backend");
const MAX_DEPTH = 2;

function generateTree(dirPath, depth = 0, folder = {}) {
    if (depth >= MAX_DEPTH) return folder;
  
    const entries = fs.readdirSync(dirPath, { withFileTypes: true }).sort((a, b) =>
      a.name.localeCompare(b.name)
    );
    let relativeFolder = '/' + path.relative(ROOT_DIR, dirPath);
  
    folder[relativeFolder] = folder[relativeFolder] || [];
  
    for (const entry of entries) {
      if (["node_modules", "__pycache__"].includes(entry.name)) continue;
  
      const fullPath = path.join(dirPath, entry.name);

      if (entry.isDirectory()) {
        // Add directory name to current path
        folder[relativeFolder].push({ name: entry.name, type: "dir" });
  
        // Recurse into it
        generateTree(fullPath, depth + 1, folder);
      } else {
        folder[relativeFolder].push({  name: entry.name, type: "file" });
      }
    }
  
    return folder;
  }
  

// Generate and save
const output = ["# 📁 Backend Folder Structure", "", generateTree(ROOT_DIR)];
console.log(generateTree(ROOT_DIR))

