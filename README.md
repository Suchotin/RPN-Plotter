# RPN-Plotter
Console-based C program that builds and visualizes in ASCII mathematical function graphs computed using Reverse Polish Notation (RPN).

🔧 Features
- Parses user input with support for parentheses, operators, and functions (`sin`, `cos`, `tan`, `ctg`, `ln`, `sqrt`).
- Converts infix expressions to postfix (RPN) using **Dijkstra’s Shunting Yard algorithm**:
  - uses tree stacks:
    1. **Input stack** - tokens from parser.
    2. **Operator stack** - temporary stack for operators and parentheses.
    3. **Output stack (RPN)** - which is passed to perform the calculation.
- Evaluates RPN expressions via a **custom stack implementation**:
  - `typedef struct stack_node {void *data; int type; struct stack_node *next;} stack_node_t;`
  - Each node stores a pointer (generic, can be number, operator, function name, etc.), integer flag defining what kind of token it holds (e.g., `TYPE_NUMBER`, `TYPE_OPERATOR`, `TYPE_FUNCTION`, etc.), pointer to the next node in the stack (LIFO).
  - Core operations are manually implemented - no `stdlib` stack API used.
- Renders an ASCII graph in the terminal:
  - The graph is 80 characters wide and 25 rows tall;
  - `x` spans from 0 to 4π, `y` spans from -1 to +1, oriented downwards in a positive direction.
- Includes detailed error handling for invalid expressions (such as incorrect input, calculation error, out of boundaries of the screen).


🧠 Example

<img width="645" height="422" alt="image" src="https://github.com/user-attachments/assets/cf997f88-81bc-48f0-9744-6a745ed72601" />
