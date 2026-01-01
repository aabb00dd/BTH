# Fuzzer for JSON Serialization Libraries

This project explores fuzz testing for JSON serialization libraries to identify edge cases and inconsistencies in handling various data structures and values.

---

## Features

- Generates random strings, numbers, and nested data structures to test serialization.
- Compares outputs from `json`, `orjson`, and `msgspec` libraries.
- Identifies exceptions and mismatches in serialization behavior.

## What I Learned

- The importance of edge case testing in serialization libraries.
- Differences in how libraries handle large integers and scientific notation.
- Techniques for generating diverse and complex test data.