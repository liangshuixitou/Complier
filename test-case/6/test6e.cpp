/* Synax error - Extra else statement on innermost if-then-else
*/
int main()
{

	int x;
	int y;

	x = 45;

	if (x >= 10)
	{
		if (x >= 20)
		{
			if (x >= 30)
			{
				if (x >= 40)
				{
					if (x >= 50)
					{
						y = -1;
					}
					else
					{
						y = 4;
					}
					else { x = 10; }
				}
				else
				{
					y = 3;
				}
			}
			else
			{
				y = 2;
			}
		}
		else
		{
			y = 1;
		}
	}
	else
	{
		y = 0;
	}

	// output(y);
	return 0;
}