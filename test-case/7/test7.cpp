int main()
{

	int x;
	int div;
	int rem;

	// x = input();
	x = 1;

	while (x > 0)
	{
		div = x / 10;
		rem = x - (div * 10);
		// output(rem);
		x = div;
	}
	return 0;
}