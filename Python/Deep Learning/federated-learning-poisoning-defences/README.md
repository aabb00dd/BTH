# Federated Learning: Poisoning and Defences

This project studies the robustness of federated learning systems under **model poisoning attacks**, comparing FedAvg and FedProx under **IID and non-IID data** and evaluating different defence mechanisms.

The work was conducted as part of coursework in deep learning and federated learning.

---

## Overview

Federated learning is vulnerable to malicious client updates. This project evaluates how performance degrades as the number of malicious clients increases and how different defences affect robustness.

---

## Methods

- **Aggregation:** FedAvg, FedProx  
- **Data:** IID and non-IID partitions  
- **Attacks:** 0, 1, and 2 malicious clients  
- **Defences:**  
  - Gradient clipping  
  - Coordinate-wise median aggregation  

Performance is measured using accuracy, weighted F1, Cohen’s kappa, ROC-AUC, and loss.

---

## Key Findings

- Non-IID data increases vulnerability to poisoning attacks  
- FedProx provides limited robustness compared to FedAvg  
- Gradient clipping is neutral under weak attacks but harmful under strong attacks  
- Median aggregation offers the best robustness–accuracy trade-off  

---

## Notes

This project is intended for academic and educational purposes.
