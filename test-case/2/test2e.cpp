/* Semantic error:  The integer variable i is used as a boolean
*/
int main()
{
      int i;
      int j;

      i = 1;
      j = 1;
      while (i)
      {
            i = i + 1;
            j = j + i;
      }
      output(i);
      output(j);
      return 0;
}