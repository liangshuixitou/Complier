int main()
{
	int i;
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
	output(c); // 3
	return 0;
}