# Graph Report - Matrix Calculator C++  (2026-11-13)

## Corpus Check
- 15 files · ~4,427 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 1 file(s) not represented in the graph (top: (none) 1)

## Summary
- 125 nodes · 312 edges · 10 communities (8 shown, 2 thin omitted)
- Extraction: 82% EXTRACTED · 18% INFERRED · 0% AMBIGUOUS · INFERRED: 57 edges (avg confidence: 0.85)
- Token cost: 72,320 input · 0 output

## Graph Freshness
- Built from commit: `fc5cb5b0`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- Tokenizer i błędy
- Tokeny (Token, TokenKind)
- Eliminacja Gaussa (Operations)
- main i interfejs Matrix
- Instrukcje graphify
- Parser.cpp
- Parser
- applyToken
- Token

## God Nodes (most connected - your core abstractions)
1. `Matrix` - 40 edges
2. `TokenKind` - 19 edges
3. `Token` - 16 edges
4. `applyToken()` - 15 edges
5. `rref()` - 15 edges
6. `inv()` - 13 edges
7. `main()` - 12 edges
8. `Value` - 10 edges
9. `Parser` - 10 edges
10. `solve()` - 10 edges

## Surprising Connections (you probably didn't know these)
- `graphify skill (/graphify trigger)` --conceptually_related_to--> `graphify usage rules (query before grep, update after edits)`  [INFERRED]
  .claude/CLAUDE.md → CLAUDE.md
- `powerValues()` --calls--> `power()`  [INFERRED]
  src/parser/Parser.cpp → src/core/Operations.cpp
- `applyToken()` --calls--> `det()`  [INFERRED]
  src/parser/Parser.cpp → src/core/Operations.cpp
- `applyToken()` --calls--> `inv()`  [INFERRED]
  src/parser/Parser.cpp → src/core/Operations.cpp
- `applyToken()` --calls--> `rank()`  [INFERRED]
  src/parser/Parser.cpp → src/core/Operations.cpp

## Import Cycles
- None detected.

## Communities (10 total, 2 thin omitted)

### Community 0 - "Tokenizer i błędy"
Cohesion: 0.23
Nodes (6): MatrixError, addToken(), isDigitAt(), isFunctionName(), isLetterAt(), tokenize()

### Community 1 - "Tokeny (Token, TokenKind)"
Cohesion: 0.13
Nodes (14): kindName(), TokenKind, Comma, Function, LeftParen, MatrixName, Minus, Number (+6 more)

### Community 3 - "Eliminacja Gaussa (Operations)"
Cohesion: 0.23
Nodes (21): Matrix, columns, rows, values, copyColumns(), det(), divideRow(), findPivotRow() (+13 more)

### Community 4 - "main i interfejs Matrix"
Cohesion: 0.14
Nodes (4): printValue(), testExpression(), ParseError, position

### Community 6 - "Parser.cpp"
Cohesion: 0.36
Nodes (5): endsValue(), moveUntilLeftParen(), Parser::Parser(), startsValue(), weight()

### Community 7 - "Parser"
Cohesion: 0.29
Nodes (4): Parser, a, b, tokens

### Community 8 - "applyToken"
Cohesion: 0.61
Nodes (8): addValues(), applyToken(), isNumber(), multiplyValues(), negateValue(), powerValues(), toMatrix(), Value

### Community 9 - "Token"
Cohesion: 0.33
Nodes (6): argumentCount(), Token, kind, position, text, value

## Knowledge Gaps
- **24 isolated node(s):** `tokens`, `a`, `b`, `Number`, `MatrixName` (+19 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 37 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **2 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `Matrix` connect `Eliminacja Gaussa (Operations)` to `Klasa Matrix`, `main i interfejs Matrix`, `Parser.cpp`, `Parser`, `applyToken`?**
  _High betweenness centrality (0.329) - this node is a cross-community bridge._
- **Are the 5 inferred relationships involving `applyToken()` (e.g. with `det()` and `inv()`) actually correct?**
  _`applyToken()` has 5 INFERRED edges - model-reasoned connections that need verification._
- **What connects `tokens`, `a`, `b` to the rest of the system?**
  _24 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `Tokeny (Token, TokenKind)` be split into smaller, more focused modules?**
  _Cohesion score 0.13333333333333333 - nodes in this community are weakly interconnected._
- **Why does `Token` connect `Token` to `Tokenizer i błędy`, `Tokeny (Token, TokenKind)`, `Parser.cpp`, `Parser`, `applyToken`?**
  _High betweenness centrality (0.207) - this node is a cross-community bridge._
- **Are the 5 inferred relationships involving `rref()` (e.g. with `.get()` and `.getColumns()`) actually correct?**
  _`rref()` has 5 INFERRED edges - model-reasoned connections that need verification._
- **Should `main i interfejs Matrix` be split into smaller, more focused modules?**
  _Cohesion score 0.13970588235294118 - nodes in this community are weakly interconnected._