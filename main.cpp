
#include "Chat.h"


void main(int argc, char* argv[]) {
	Chat chat("127.0.0.1", 27015);
	chat.init();//initializes server
	chat.run();//runs listeners
	system("pause");


}
