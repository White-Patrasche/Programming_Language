import java.util.*;

public class task2 {
    static Queue<String> Q = new LinkedList<>();
    static Scanner sc = new Scanner(System.in); 

    static void error() {
        System.out.print("syntax error!!\n");
        sc.close();
        System.exit(0);
    }

    static int DIGIT(char[]str, int index) {
        String temp = "";
        int cnt = 0;
        for(int i=index; i<str.length; i++, cnt++) {
            if(str[i] >= '0' && str[i] <= '9') temp += str[i];
            else if(str[i] == ' ') continue;
            else break;
        }
        Q.add(temp);
        return cnt-1;
    }

    static void tokenParse(String stringData) {
        String[] strArr = stringData.split("");
        char[] str = new char[stringData.length()];
        for(int i=0; i<strArr.length; i++) {
            str[i] = strArr[i].charAt(0);
        }
        
        for(int i=0; i<str.length; i++) {
            if(str[i] == ' ') continue;
            else if(str[i] >= '0' && str[i] <= '9') {
                i += DIGIT(str, i);
            }
            else if(str[i] == '*' || str[i] == '/' || str[i] == '+' || str[i] == '-' || str[i] == '(' || str[i] == ')') {
                String temp = "";
                temp += str[i];
                Q.add(temp);
            }
            else {
                error();
                return;
            }
        }
    }

    static double expr() {
        double data = 0;
        data = term();
        while(nextValue().equals("+") || nextValue().equals("-")) {
            char oper = lex().charAt(0);
            if(oper == '+') data += term();
            else if(oper == '-') data -= term();
            if(nextValue() == null) break;
        }
        return data;
    }

    static double term() {
        double data = 0;
        data = factor();
        while(nextValue().equals("*") || nextValue().equals("/")) {
            char oper = lex().charAt(0);
            if(oper == '*') data *= factor();
            else if(oper == '/') data /= factor();
            if(nextValue() == null) break;
        }
        return data;
    }

    static double factor() {
        double data = 0;
        char oper = '+';
        if(nextValue().equals("-")) {
            oper = lex().charAt(0);
        }

        if(nextValue() == "") error(); //if input is only "-", can generate OutOfBoundException
        else if(nextValue().charAt(0) >= '0' && nextValue().charAt(0) <= '9') {
            data = number();
        }
        else {
            if(nextValue().equals("(")) {
                lex();
                data = expr();
                if(nextValue().equals(")")) {
                    lex();
                }
                else error();
            }
            else error();
        }
        if(oper == '-') return -data;
        else return data;
    }

    static int number() {
        String number = lex();
        return Integer.parseInt(number);
    }

    static String lex() {
        if(Q.peek() == null) return "";
        else return Q.poll();
    }

    static String nextValue() {
        if(Q.peek()==null) return "";
        else return Q.peek();
    }

    static void queueClear() {
        Q.clear();
    }

    public static void main(String[] args) {
        
        while(true) {
            double ans = 0;
            String str;
            queueClear();
            System.out.print(">> ");
            str = sc.nextLine();
            tokenParse(str);
            ans = expr();
            if(!Q.isEmpty()) {
                error();
            }
            if(Math.ceil(ans) - ans != 0) //if data is floating point
                System.out.println(ans);
            else //if data is integer
                System.out.println((int)ans);
        }
    }
}
