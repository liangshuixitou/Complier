/* Semantic error:  Variable I is used as i -- case not matched
*/
int main()
{
	int I;
	int j;
	int a;
	int b;
	int c;

	i = 3;
	j = 4;
	a = i + j + 1;
	b = i * j * 2;
	b = 0 - b;
	c = b / (0 - a);
	// output(c);
	return 0;
}