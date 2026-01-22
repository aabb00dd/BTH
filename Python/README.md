# Adversarial Attacks on Facial Recognition

This project investigates the robustness of deep learning–based facial recognition systems against adversarial attacks, with a focus on **physically realizable adversarial patches**. The work evaluates how vulnerable modern CNN-based models are to both digital and real-world attacks, and assesses the effectiveness of different defense strategies.

The project was conducted as part of coursework at Blekinge Institute of Technology (BTH).

---

## Overview

Facial recognition systems are widely used in security and surveillance applications, yet their reliability can be compromised by adversarial inputs. This project explores:

- Digital adversarial attacks (FGSM, PGD, DeepFool)
- Localized adversarial patch attacks
- The transferability of attacks from digital to physical settings
- Defense mechanisms to improve robustness

---

## Methodology

### Models
- ResNet-18 (lightweight, edge-oriented model)
- ResNet-101 (high-capacity, server-side model)
- Transfer learning from ImageNet
- Fine-tuned on a custom facial recognition dataset (183 identities)

### Attacks
- **FGSM** (Fast Gradient Sign Method)
- **PGD / AutoPGD** (iterative gradient-based attack)
- **DeepFool**
- **Adversarial Patches** (targeted and untargeted, digital and physical)

### Defenses
- **Adversarial Training**
- **Feature Squeezing** (reduced color bit depth)
- Combined defenses for comparison

### Physical Evaluation
Adversarial patches were printed and physically applied to a subject’s face to evaluate real-world effectiveness under non-ideal conditions such as lighting, rotation, and print quality.

---

## Key Findings

- Higher clean accuracy does **not** imply robustness to adversarial attacks  
- Iterative attacks (PGD) achieved near-100% success digitally  
- Adversarial patches pose a realistic threat but are less reliable in physical settings  
- **Adversarial training** provided the strongest defense and even improved clean accuracy  
- Combining defenses did not outperform adversarial training alone  
