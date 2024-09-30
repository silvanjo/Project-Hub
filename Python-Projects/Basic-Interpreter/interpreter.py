import re
import sys

class BasicInterpreter:
    def __init__(self):
        self.lines = {}
        self.variables = {}
        self.current_line = None
        self.sorted_line_numbers = []
        self.line_index = 0

    def load_program(self, program_lines):
        for line in program_lines:
            line = line.strip()
            if not line:
                continue
            match = re.match(r'^(\d+)\s+(.*)$', line)
            if match:
                line_number = int(match.group(1))
                code = match.group(2)
                self.lines[line_number] = code
            else:
                print(f"Syntax Error: '{line}' does not start with a line number.")
                sys.exit(1)
        self.sorted_line_numbers = sorted(self.lines.keys())

    def run(self):
        self.line_index = 0
        while self.line_index < len(self.sorted_line_numbers):
            self.current_line = self.sorted_line_numbers[self.line_index]
            code = self.lines[self.current_line]
            self.execute(code)
            self.line_index += 1

    def execute(self, code):
        tokens = self.tokenize(code)
        if not tokens:
            return
        command = tokens[0].upper()
        if command == 'LET':
            self.handle_let(tokens[1:])
        elif command == 'PRINT':
            self.handle_print(tokens[1:])
        elif command == 'INPUT':
            self.handle_input(tokens[1:])
        elif command == 'GOTO':
            self.handle_goto(tokens[1:])
        elif command == 'IF':
            self.handle_if(tokens[1:])
        elif command == 'END':
            sys.exit(0)
        else:
            print(f"Unknown command '{command}' at line {self.current_line}")
            sys.exit(1)

    def tokenize(self, code):
        # Simple tokenizer based on spaces and operators
        tokens = re.findall(r'[\w\.]+|==|<=|>=|<>|!=|[+\-*/()<>=]', code)
        return tokens

    def handle_let(self, tokens):
        if '=' in tokens:
            index = tokens.index('=')
        else:
            print(f"Syntax Error in LET statement at line {self.current_line}")
            sys.exit(1)
        var_name = tokens[0]
        expr = tokens[index + 1:]
        value = self.evaluate_expression(expr)
        self.variables[var_name] = value

    def handle_print(self, tokens):
        value = self.evaluate_expression(tokens)
        print(value)

    def handle_input(self, tokens):
        var_name = tokens[0]
        try:
            user_input = input(f"Enter value for {var_name}: ")
            self.variables[var_name] = float(user_input)
        except ValueError:
            print(f"Invalid input for variable '{var_name}' at line {self.current_line}")
            sys.exit(1)

    def handle_goto(self, tokens):
        if len(tokens) != 1:
            print(f"Syntax Error in GOTO statement at line {self.current_line}")
            sys.exit(1)
        line_number = int(tokens[0])
        if line_number in self.lines:
            self.line_index = self.sorted_line_numbers.index(line_number) - 1
        else:
            print(f"GOTO line number {line_number} does not exist.")
            sys.exit(1)

    def handle_if(self, tokens):
        # Find 'THEN' keyword
        if 'THEN' not in tokens:
            print(f"Syntax Error: IF statement without THEN at line {self.current_line}")
            sys.exit(1)
        then_index = tokens.index('THEN')
        condition_tokens = tokens[:then_index]
        then_tokens = tokens[then_index + 1:]

        condition_result = self.evaluate_condition(condition_tokens)
        if condition_result:
            self.execute(' '.join(then_tokens))

    def evaluate_expression(self, tokens):
        # Convert infix expression to postfix using Shunting Yard algorithm
        output_queue = []
        operator_stack = []

        precedence = {'+':1, '-':1, '*':2, '/':2}
        operators = set(['+', '-', '*', '/'])

        tokens = tokens.copy()
        while tokens:
            token = tokens.pop(0)
            if re.match(r'^[A-Za-z]\w*$', token):  # Variable
                if token in self.variables:
                    output_queue.append(str(self.variables[token]))
                else:
                    print(f"Undefined variable '{token}' at line {self.current_line}")
                    sys.exit(1)
            elif re.match(r'^\d+(\.\d+)?$', token):  # Number
                output_queue.append(token)
            elif token in operators:
                while (operator_stack and operator_stack[-1] in operators and
                       precedence[token] <= precedence[operator_stack[-1]]):
                    output_queue.append(operator_stack.pop())
                operator_stack.append(token)
            elif token == '(':
                operator_stack.append(token)
            elif token == ')':
                while operator_stack and operator_stack[-1] != '(':
                    output_queue.append(operator_stack.pop())
                if operator_stack and operator_stack[-1] == '(':
                    operator_stack.pop()
                else:
                    print(f"Syntax Error: Mismatched parentheses at line {self.current_line}")
                    sys.exit(1)
            else:
                print(f"Unknown token '{token}' at line {self.current_line}")
                sys.exit(1)

        while operator_stack:
            op = operator_stack.pop()
            if op in '()':
                print(f"Syntax Error: Mismatched parentheses at line {self.current_line}")
                sys.exit(1)
            output_queue.append(op)

        # Evaluate postfix expression
        stack = []
        for token in output_queue:
            if re.match(r'^\d+(\.\d+)?$', token):
                stack.append(float(token))
            elif token in operators:
                if len(stack) < 2:
                    print(f"Syntax Error in expression at line {self.current_line}")
                    sys.exit(1)
                b = stack.pop()
                a = stack.pop()
                if token == '+':
                    stack.append(a + b)
                elif token == '-':
                    stack.append(a - b)
                elif token == '*':
                    stack.append(a * b)
                elif token == '/':
                    if b == 0:
                        print(f"Division by zero at line {self.current_line}")
                        sys.exit(1)
                    stack.append(a / b)
            else:
                print(f"Unknown operator '{token}' at line {self.current_line}")
                sys.exit(1)

        if len(stack) != 1:
            print(f"Syntax Error in expression at line {self.current_line}")
            sys.exit(1)
        return stack[0]

    def evaluate_condition(self, tokens):
        # Simple condition evaluator for expressions like 'A > B', 'X <= 10'
        if len(tokens) < 3:
            print(f"Syntax Error in condition at line {self.current_line}")
            sys.exit(1)
        left_expr = []
        operator = None
        right_expr = []
        operators = ['==', '<=', '>=', '<>', '!=', '<', '>']
        # Find the operator
        for i, token in enumerate(tokens):
            if token in operators:
                operator = token
                left_expr = tokens[:i]
                right_expr = tokens[i+1:]
                break
        if not operator:
            print(f"Invalid operator in condition at line {self.current_line}")
            sys.exit(1)

        left_value = self.evaluate_expression(left_expr)
        right_value = self.evaluate_expression(right_expr)

        if operator == '==':
            return left_value == right_value
        elif operator in ('<>', '!='):
            return left_value != right_value
        elif operator == '<':
            return left_value < right_value
        elif operator == '>':
            return left_value > right_value
        elif operator == '<=':
            return left_value <= right_value
        elif operator == '>=':
            return left_value >= right_value
        else:
            print(f"Unknown operator '{operator}' at line {self.current_line}")
            sys.exit(1)

def main():
    print("Simple BASIC Interpreter")
    print("Enter your BASIC program line by line.")
    print("Type 'RUN' to execute the program.")
    print("Type 'EXIT' to quit.")
    program_lines = []
    while True:
        try:
            line = input('> ')
            if line.strip().upper() == 'RUN':
                break
            elif line.strip().upper() == 'EXIT':
                sys.exit(0)
            else:
                program_lines.append(line)
        except EOFError:
            break

    interpreter = BasicInterpreter()
    interpreter.load_program(program_lines)
    interpreter.run()

if __name__ == '__main__':
    main()

