# FakeOnline
Simple plugin for SA:MP 0.3.7 R2 server that allows you to adjust online player's count that displayed in the server list

# Example
```pawn
#include <FakeOnline>

public OnSendPlayerCount(count)
{
	return 996 + random(5);
}
```
