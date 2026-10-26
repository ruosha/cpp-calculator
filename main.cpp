#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <unordered_map>
using namespace std;

// storage operations with precedence
unordered_map<string, int> ops = {{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}};

int prec(const string &op)
{
    if (ops.find(op) == ops.end())
        return 0;
    return ops[op];
}

bool isOp(const string &t)
{
    if (ops.find(t) == ops.end())
        return false;
    return true;
}

int main()
{
    string input;
    getline(cin, input);
    // remove all whitespace characters
    auto transform = [&](string text) -> string
    {
        string res;
        for (char ch : text)
            if (!isspace((unsigned char)ch))
                res += ch;
        return res;
    };
    // split the string into tokens
    auto tokenize = [&](const string &s) -> vector<string>
    {
        vector<string> tok;
        for (size_t i = 0; i < s.size();)
        {
            if (isdigit((unsigned char)s[i]))
            {
                string num;
                while (i < s.size() && isdigit((unsigned char)s[i]))
                    num += s[i++];
                tok.push_back(num);
            }
            else
            {
                tok.push_back(string(1, s[i++]));
            }
        }
        return tok;
    };
    // convert infix string into prefix expression
    auto toPrefix = [&](const vector<string> &tok) -> vector<string>
    {
        vector<string> out;
        stack<string> st;
        for (int i = (int)tok.size() - 1; i >= 0; --i)
        {
            const string &t = tok[i];

            if (isOp(t))
            {
                while (!st.empty() && st.top() != ")" && prec(st.top()) > prec(t))
                {
                    out.push_back(st.top());
                    st.pop();
                }
                st.push(t);
            }
            else if (t == ")")
            {
                st.push(t);
            }
            else if (t == "(")
            {
                while (!st.empty() && st.top() != ")")
                {
                    out.push_back(st.top());
                    st.pop();
                }
                if (!st.empty())
                    st.pop();
            }
            else
            {
                out.push_back(t);
            }
        }
        while (!st.empty())
        {
            out.push_back(st.top());
            st.pop();
        }
        reverse(out.begin(), out.end());
        return out;
    };
    // calculate
    auto calc = [&](const vector<string> &pre) -> int
    {
        stack<int> st;
        for (int i = (int)pre.size() - 1; i >= 0; --i)
        {
            const string &t = pre[i];
            if (!isOp(t))
            {
                st.push(stoi(t));
                continue;
            }
            int x = st.top();
            st.pop();
            int y = st.top();
            st.pop();
            if (t == "+")
                st.push(x + y);
            else if (t == "-")
                st.push(x - y);
            else if (t == "*")
                st.push(x * y);
            else
                st.push(x / y);
        }
        return st.top();
    };
    vector<string> tok = tokenize(transform(input));
    vector<string> pre = toPrefix(tok);
    for (const string &t : pre)
        cout << t << ' ';
    cout << '\n'
         << calc(pre) << '\n';
    return 0;
}