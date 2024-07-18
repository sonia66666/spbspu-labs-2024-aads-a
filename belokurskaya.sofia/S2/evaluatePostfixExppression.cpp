#include "evaluatePostfixExpression.hpp"

#include <cctype>
#include <limits>
#include <stdexcept>

#include "stack.hpp"

bool belokurskaya::isOperator(char c)
{
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}

void belokurskaya::safeStrCopy(char* dest, const char* src, size_t max_len)
{
  for (size_t i = 0; i < max_len - 1 && src[i] != '\0'; ++i)
  {
    dest[i] = src[i];
    dest[i + 1] = '\0';
  }
}

bool belokurskaya::safeAdd(long long a, long long b, long long& result)
{
  if ((b > 0 && a > std::numeric_limits< long long >::max() - b) ||
    (b < 0 && a < std::numeric_limits< long long >::min() - b))
  {
    std::cerr << "Addition Overflow\n";
    return false;
  }
  result = a + b;
  return true;
}

bool belokurskaya::willMultiplyOverflowOrUnderflow(long long a, long long b, char* errorType)
{
  if (a > 0)
  {
    if (b > 0)
    {
      if (a > (std::numeric_limits< long long >::max() / b))
      {
        safeStrCopy(errorType, "Overflow", 30);
        return true;
      }
    }
    else
    {
      if (b < (std::numeric_limits< long long >::min() / a))
      {
        safeStrCopy(errorType, "Underflow", 30);
        return true;
      }
    }
  }
  else
  {
    if (b > 0)
    {
      if (a < (std::numeric_limits< long long >::min() / b))
      {
        safeStrCopy(errorType, "Underflow", 30);
        return true;
      }
    }
    else
    {
      if (a != 0 && b < (std::numeric_limits< long long >::max() / a))
      {
        safeStrCopy(errorType, "Overflow", 30);
        return true;
      }
    }
  }
  return false;
}

bool belokurskaya::willSubtractUnderflow(long long a, long long b)
{
  if ((b > 0 && a < std::numeric_limits< long long >::min() + b) ||
    (b < 0 && a > std::numeric_limits< long long >::max() + b))
  {
    return true;
  }
  return false;
}

long long belokurskaya::evaluatePostfixExpression(std::string expression)
{
  Stack< long long > st;
  long long num = 0;
  bool flag = false;

  for (char& c : expression)
  {
    if (std::isdigit(c))
    {
      num = num * 10 + (c - '0');
      flag = true;
    }
    else if (std::isspace(c))
    {
      if (flag)
      {
        st.push(num);
        flag = false;
        num = 0;
      }
    }
    else if (isOperator(c))
    {
      long long operand2 = st.pop();
      long long operand1 = st.pop();
      if (c == '+')
      {
        long long result;
        if (!safeAdd(operand1, operand2, result))
        {
          exit(1);
        }
        st.push(result);
      }
      else if (c == '-')
      {
        char errorType[30];
        if (willSubtractUnderflow(operand1, operand2))
        {
          safeStrCopy(errorType, "Subtraction Underflow", 30);
          std::cerr << errorType << "\n";
          exit(1);
        }
        else
        {
          st.push(operand1 - operand2);
        }
      }
      else if (c == '*')
      {
        char errorType[10];
        if (willMultiplyOverflowOrUnderflow(operand1, operand2, errorType))
        {
          std::cerr << "Multiplication" << errorType << "\n";
          exit(1);
        }
        else
        {
          st.push(operand1 * operand2);
        }
      }
      else if (c == '/')
      {
        if (operand2 == 0)
        {
          std::cerr << "Division by zero\n";
          exit(1);
        }
        if (operand1 == std::numeric_limits< long long >::min() && operand2 == -1)
        {
          std::cerr << "Division Overflow\n";
          exit(1);
        }
        st.push(operand1 / operand2);
      }
      else if (c == '%')
      {
        st.push(operand1 % operand2);
      }
    }
    else
    {
      throw std::invalid_argument("Unknown symbol in the argument");
    }
  }
  return st.top();
}