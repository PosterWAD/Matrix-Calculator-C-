# Graph Report - Matrix Calculator C++  (2026-11-13)

## Corpus Check
- 15 files · ~4,131 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 1 file(s) not represented in the graph (top: (none) 1)

## Summary
- 128 nodes · 333 edges · 11 communities (8 shown, 3 thin omitted)
- Extraction: 82% EXTRACTED · 18% INFERRED · 0% AMBIGUOUS · INFERRED: 59 edges (avg confidence: 0.85)
- Token cost: 72,320 input · 0 output

## Graph Freshness
- Built from commit: `fc5cb5b0`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- Tokenizer i błędy
- Tokeny (Token, TokenKind)
- Klasa Matrix
- Operations.cpp
- main i interfejs Matrix
- Instrukcje graphify
- inv
- Parser.hpp
- Parser.cpp
- Token
- main

## God Nodes (most connected - your core abstractions)
1. `Matrix` - 40 edges
2. `Parser` - 18 edges
3. `TokenKind` - 17 edges
4. `Value` - 15 edges
5. `rref()` - 15 edges
6. `inv()` - 13 edges
7. `Token` - 12 edges
8. `main()` - 12 edges
9. `applyFunction()` - 11 edges
10. `solve()` - 10 edges

## Surprising Connections (you probably didn't know these)
- `graphify skill (/graphify trigger)` --conceptually_related_to--> `graphify usage rules (query before grep, update after edits)`  [INFERRED]
  .claude/CLAUDE.md → CLAUDE.md
- `powerValues()` --calls--> `power()`  [INFERRED]
  src/parser/Parser.cpp → src/core/Operations.cpp
- `applyFunction()` --calls--> `det()`  [INFERRED]
  src/parser/Parser.cpp → src/core/Operations.cpp
- `applyFunction()` --calls--> `inv()`  [INFERRED]
  src/parser/Parser.cpp → src/core/Operations.cpp
- `applyFunction()` --calls--> `rank()`  [INFERRED]
  src/parser/Parser.cpp → src/core/Operations.cpp

## Import Cycles
- None detected.

## Communities (11 total, 3 thin omitted)

### Community 0 - "Tokenizer i błędy"
Cohesion: 0.47
Nodes (5): addToken(), isDigitAt(), isFunctionName(), isLetterAt(), tokenize()

### Community 1 - "Tokeny (Token, TokenKind)"
Cohesion: 0.17
Nodes (12): TokenKind, Comma, Function, LeftParen, MatrixName, Minus, Number, Plus (+4 more)

### Community 2 - "Klasa Matrix"
Cohesion: 0.19
Nodes (6): Matrix, columns, rows, values, det(), trace()

### Community 3 - "Operations.cpp"
Cohesion: 0.40
Nodes (11): divideRow(), findPivotRow(), isRowZero(), isZero(), joinSideBySide(), leftPartIsIdentity(), rank(), rref() (+3 more)

### Community 4 - "main i interfejs Matrix"
Cohesion: 0.12
Nodes (5): MatrixError, printValue(), testExpression(), ParseError, position

### Community 6 - "inv"
Cohesion: 0.70
Nodes (3): copyColumns(), inv(), power()

### Community 8 - "Parser.cpp"
Cohesion: 0.24
Nodes (13): addValues(), applyFunction(), isNumber(), multiplyValues(), negateValue(), Parser, a, b (+5 more)

### Community 9 - "Token"
Cohesion: 0.29
Nodes (6): Parser::Parser(), Token, kind, position, text, value

## Knowledge Gaps
- **24 isolated node(s):** `tokens`, `a`, `b`, `index`, `Number` (+19 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 38 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **3 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `Matrix` connect `Klasa Matrix` to `Operations.cpp`, `main i interfejs Matrix`, `inv`, `Parser.cpp`, `Token`, `main`?**
  _High betweenness centrality (0.350) - this node is a cross-community bridge._
- **Are the 5 inferred relationships involving `rref()` (e.g. with `.get()` and `.getColumns()`) actually correct?**
  _`rref()` has 5 INFERRED edges - model-reasoned connections that need verification._
- **What connects `tokens`, `a`, `b` to the rest of the system?**
  _24 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `main i interfejs Matrix` be split into smaller, more focused modules?**
  _Cohesion score 0.11594202898550725 - nodes in this community are weakly interconnected._
- **Why does `Parser` connect `Parser.cpp` to `Token`, `Klasa Matrix`, `Parser.hpp`?**
  _High betweenness centrality (0.199) - this node is a cross-community bridge._
- **Why does `TokenKind` connect `Tokeny (Token, TokenKind)` to `Parser.cpp`, `Token`, `Tokenizer i błędy`, `Parser.hpp`?**
  _High betweenness centrality (0.177) - this node is a cross-community bridge._