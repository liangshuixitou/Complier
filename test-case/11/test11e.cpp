/* Syntax error - Variable declaration (k) occurs after statements begin
*/

int main()
{
   int i;
   int j;
   int k;

   i = 5;
   j = i + j;
   k = i + j + k;

   // output(i);
   // output(j);
   // output(k);

   if (i == i)
   {
      int i;
      int j;

      i = 10;
      j = i + j;

      int k;

      k = i + j + k;
      // output(i);
      // output(j);
      // output(k);

      if (j < k)
      {
         int j;
         int k;

         j = i * 5;
         k = i * 10;

         // output(i);
         // output(j);d
         // output(k);
      }

      // output(i);
      // output(j);
      // output(k);
   }

   // output(i);
   // output(j);
   // output(k);
   return 0;
}