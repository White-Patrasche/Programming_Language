from collections import deque
ERROR = False

def error():
    print("Syntax error!!")
    exit()

def DIGIT(i_str, index, lexeme):
    temp = ""
    i = index
    cnt = 0
    while i < len(i_str):
        #if i_str[i] is char type of number
        if i_str[i].isnumeric():
            temp += i_str[i]
        elif i_str[i] == ' ':
            pass
        #not blank or number
        else :
            break
        cnt+=1
        i+=1
    lexeme.append(temp)
    return cnt-1

def tokenParse(i_str, lexeme):
    i = 0
    while i < len(i_str):
        if i_str[i] == ' ': 
            i+=1
            continue
        elif i_str[i] >= '0' and i_str[i] <= '9':
            i+=DIGIT(i_str, i, lexeme)
        elif i_str[i] == '*' or i_str[i] == '/' or i_str[i] == '+' or i_str[i] == '-' or i_str[i] == '(' or i_str[i] == ')':
            lexeme.append(i_str[i])
        else:
            error()
            return
        i+=1

def expr(lexeme):
    data = term(lexeme)

    while nextValue(lexeme) == "+" or nextValue(lexeme) == "-":
        oper = lex(lexeme)

        if oper == "+": data += term(lexeme)
        elif oper == "-": data -= term(lexeme)
        else : error()
    return data

def term(lexeme) :
    data = factor(lexeme)
    while nextValue(lexeme) == "*" or nextValue(lexeme) == "/":
        oper = lex(lexeme)
        if oper == "*" : data *= factor(lexeme)
        elif oper == "/" : data /= factor(lexeme)
        else : exit()
    return data

def factor(lexeme):
    oper = "+"
    if nextValue(lexeme) == "-":
        oper = lex(lexeme)
    
    if nextValue(lexeme).isnumeric():
        data = number(lexeme)
    else :
        if nextValue(lexeme) == "(":
            lex(lexeme)
            data = expr(lexeme)
            if nextValue(lexeme) == ")":
                lex(lexeme)
            else : error()
        else : error()
    
    if oper == "-": return -data
    else : return data

def number(lexeme) :
    num = lex(lexeme)
    return int(num)

def lex(lexeme):
    if lexeme:
        return lexeme.popleft()
    else :
        return ""

def nextValue(lexeme):
    if lexeme: 
        returnData = lexeme.popleft()
        lexeme.appendleft(returnData)
        return returnData
    else : 
        return ""

while True:
    inputStr = input('>> ')
    lexeme = deque()
    tokenParse(inputStr, lexeme)
    ans = expr(lexeme)

    #if lexeme have data even calculation is done
    if lexeme :
        error()
    print(ans)
