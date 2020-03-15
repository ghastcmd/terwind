float d_sqrt(float number)
{
	int i;
	float x, y;
	x = number * 0.5;
	y = number;
	i = *(int*) &y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*) &i;
	y = y * (1.5 - (x * y * y));
	y = y * (1.5 - (x * y * y));
	return number * y;
}


unsigned int f_randi(unsigned int index)
{
	index = (index << 13) ^ index;
	return ((index * (index * index * 15731 + 789221) + 1376312589) & 0x7fffffff);
}


GetAsyncKeyState(VK_LBUTTON);