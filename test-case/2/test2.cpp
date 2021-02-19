int main()
{
	int i;
	int j;
	i = 0;
	j = 1;
	while (i < 10)
	{
		i = i + 1;
		j = j + i;
	}
	output(i); // 10
	output(j); // 56
	return 0;
}