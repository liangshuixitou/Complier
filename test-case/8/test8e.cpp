/* Semantic error - rem function called with wrong number of args
*/

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
     return (y - (factor * x));
}

int main()
{

     int x;
     int dx;
     int rx;

     // x = input();
     x = 1;

     while (x > 0)
     {
          dx = div(x);
          rx = rem(dx);
          // output(rx);
          x = dx;
     }
     return 0;
}