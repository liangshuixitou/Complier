int Fibonacci(int x)
{
	int val;

	if (x == 1)
	{
		val = 1;
	}
	if (x == 2)
	{
		val = 1;
	}
	if (x > 2)
	{
		val = Fibonacci(x - 2) + Fibonacci(x - 1);
	}
	return val;
}

int main()
{
	int x;
	int y;
	int result;

	x = 5;
	y = 2;
	result = Fibonacci(x * y);
	output(result);
	return 0;
}

// 1 1 2 3 5 8 13 21 34 55