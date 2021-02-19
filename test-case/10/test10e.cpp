int div(int x)
{
      int factor;

      factor = 10;
      return (x / factor);
}

int rem(int x, int y)
{
      int factor;

      factor = 10;
      factor = (y - (factor * x));
}

int digitize(int x)
{
      int dx;
      int rx;

      while (x > 0)
      {
            dx = div(x);
            rx = rem(dx, x);
            x = dx;
      }
      return 0;
}

int Fibonacci(int xx)
{
      int val;

      if (xx == 1)
      {
            val = 1;
      }
      if (xx == 2)
      {
            val = 1;
      }
      if (xx > 2)
      {
            val = Fibonacci(xx - 2) + Fibonacci(xx - 1);
      }
      digitize(val);
      return val;
}

int main()
{
      int x;
      int result;

      result = Fibonacci(x);
      return 0;
}