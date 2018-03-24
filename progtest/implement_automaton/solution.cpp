int data[22][4] = {
	{1, 2, 3, -1}, 
	{4, 5, -1, -1}, 
	{6, -1, -1, 7}, 
	{8, -1, -1, -1}, 
	{9, 10, -1, -1}, 
	{-1, -1, 11, -1}, 
	{-1, -1, -1, 12}, 
	{-1, -1, -1, 7}, 
	{-1, 13, 14, -1}, 
	{-1, 15, -1, -1}, 
	{16, -1, 11, -1}, 
	{17, -1, -1, -1}, 
	{-1, -1, -1, 18}, 
	{-1, -1, -1, 19}, 
	{-1, 20, -1, -1}, 
	{16, -1, -1, -1}, 
	{-1, 15, -1, -1}, 
	{-1, 5, -1, -1}, 
	{-1, -1, -1, 7}, 
	{-1, -1, -1, 21}, 
	{21, -1, -1, -1}, 
	{-1, -1, -1, -1}
};

bool finalState(int state)
{
	switch(state)
	{
		case 7:
		case 16:
		case 17:
		case 21:
			return true;
	}
	return false;
}

bool hasState(int curState, char cur)
{
	if(cur != 'a' && cur != 'b' && cur != 'c' && cur != 'd')
		return false;

	int index = cur - 'a';
	if(data[curState][index] == -1)
		return false;
	return true;
}

void nextState(int& curstate, char cur)
{
	int index = cur - 'a';
	int next = data[curstate][index];
	curstate = next;
}

bool checkString(const char * inputString)
{
	int state = 0;
	char cur;
	while((cur = *inputString++))
	{
		if(!hasState(state, cur))
			return false;
		nextState(state, cur);
	}

	if(finalState(state))
		return true;
	return false;
}
