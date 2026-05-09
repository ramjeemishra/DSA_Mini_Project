# Expression Evaluator Using Stack

> **DSA Mini Project** — Using Custom Array-Based Stacks

A menu-driven console calculator backend built in C++ that demonstrates core Data Structure concepts through expression conversion and evaluation.

---

## Features

| # | Feature | DSA Concept |
|---|---------|-------------|
| 1 | Check Balanced Parentheses | Stack Push/Pop + Bracket Matching |
| 2 | Convert Infix to Postfix | Shunting-Yard Algorithm (Stack) |
| 3 | Evaluate Postfix Expression | Stack-Based Arithmetic Evaluation |
| 4 | Convert Infix to Prefix | Reverse + Modified Shunting-Yard |
| 5 | View Expression History | Array-Based Linear Storage |
| 6 | Undo Last Operation | Stack Pop + History Removal |

---

## Data Structures Used

### 1. CharStack (Array-Based)
- Stores characters for parentheses checking and expression conversion
- Operations: `push()`, `pop()`, `peek()`, `isEmpty()`, `isFull()`, `display()`
- All operations run in **O(1)** time

### 2. DoubleStack (Array-Based)
- Stores numeric values for postfix evaluation
- Same interface as CharStack but holds `double` values
- Handles arithmetic with division-by-zero detection

### 3. HistoryManager
- Stores all evaluated expressions in a chronological array
- Supports formatted table display and entry removal

### 4. UndoManager
- Array-based stack of undone operations
- Enables reversal of the last action

---

## How to Compile & Run

```bash
# Compile
clang++ expression_evaluator.cpp -o expression_evaluator

# Run
./expression_evaluator
```

---

## Team Contribution

| Member | Role | Modules |
|--------|------|---------|
| **Ramjee** | Stack Developer | CharStack, DoubleStack, Push/Pop/Peek |
| **Dev** | Conversion Core | Infix→Postfix, Infix→Prefix, Precedence handling |
| **Aryan** | Evaluator & Validator | Postfix Evaluation, Balanced Parentheses, Error handling |
| **Ismail** | Features & UI | History Manager, Undo Manager, Menu Interface |

---

## Complexity Analysis

| Operation | Time | Space |
|-----------|------|-------|
| Push / Pop / Peek | O(1) | O(1) |
| Balanced Parentheses | O(n) | O(n) |
| Infix → Postfix | O(n) | O(n) |
| Postfix Evaluation | O(n) | O(n) |
| Infix → Prefix | O(n) | O(n) |
| Display History | O(k) | O(1) |
| Undo | O(1) | O(1) |

Where **n** = expression length, **k** = number of history entries.# DSA_Mini_Project
