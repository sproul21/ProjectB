void printString(char*);
void printChar(char);
void readString(char*);
void readSector();
void handleInterrupt21();

void main() {

//      interrupt(0x10, 0xe*256+'Q', 0, 0, 0);
//	printString("HELLO WORLD");
	char buffer[512];
	char* line[80];
/*	printString("Enter a line: ");
	readString(line);
	printString(line);
*/
	makeInterrupt21();
	interrupt(0x21, 0, "Enter a line: ", 0, 0);
	interrupt(0x21, 1, line, 0, 0);
	interrupt(0x21, 0, line, 0, 0);
	interrupt(0x21, 2, buffer, 30, 0);
	interrupt(0x21, 0, buffer, 0, 0);
	interrupt(0x21, 3, 0, 0, 0);
        while(1);
}

void printString(char* chars){

	int c=0;
	while(chars[c] != 0x0){
		printChar(chars[c]);
		c++;
	}
}

void printChar(char c){

	c= interrupt(0x10, 0xe*256+c, 0, 0, 0);

}

void readString(char* line){

	char c;
	int n = 0;
	while(1){
		c = interrupt(0x16, 0, 0, 0);
		if(c == 0xd){
			break;
		}
		if(c == 0x8){
			line[n--];
			printChar(0x8);
			printChar(' ');
			printChar(0x8);
		}
		else{
			line[n++] = c;
			printChar(c);
		}
	}
	line[n++] = "0xd";
	line[n++] = "0xa";
	line[n++] = "0x0";
	printChar("0xd");
	printChar("0xa");

}


void readSector(char* buffer, int sector){
	int AX = 2*256+1;
	int BX = buffer;
	int CX = 0*256+(sector+1);
	int DX = 0*256+(0x80);
	interrupt(0x13, AX, BX, CX, DX);
}

void handleInterrupt21(int ax, int bx, int cx, int dx){

	if(ax == 0){
		printString(bx);
	}
	else if(ax == 1){
		readString(bx);
	}
	else if(ax == 2){
		readSector(bx, cx);
	}
	else{
		printString("Out of bounds exception for interrupt");
	}

}
