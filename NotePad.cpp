
#include <iostream>
#include <Windows.h>
#include<fstream>
#include<queue>
#include <filesystem>
using namespace std;
int consoleWidth;
int consoleHeight;
int textHeight;
int textWidth;
int searchWidth;
int suggestionsWidth;
//6x120=suggestions
//90x24=text 
//24x30 =search
void setColor(int textCol, int backgroundColor) {
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hC, (backgroundColor << 4) | textCol);
}
void gotoxy(int x, int y) {
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void RemoveScroll()  //function to remove scroll bar
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(hConsole, &bufferInfo);

	// Set the buffer height to the same as the window height
	SMALL_RECT window;
	window = bufferInfo.srWindow;  // Get current window size

	// Set the buffer size
	COORD Size;
	Size.X = bufferInfo.dwSize.X;
	Size.Y = window.Bottom - window.Top + 1; // Set buffer height to window height

	SetConsoleScreenBufferSize(hConsole, Size);
}
void getConsoleSize()	//This function will return the console size of each laptop
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;	//stores the number of columns
	consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;	//stores the number of rows
	textHeight = 0.8 * consoleHeight;
	textWidth = 0.75 * consoleWidth;
	searchWidth = 0.2 * consoleWidth;
	suggestionsWidth = consoleWidth;
}
void menu() {
	system("cls");
	gotoxy(50, 5);
	cout << "Menu:\n";
	gotoxy(45, 6);
	cout << "1. Create a new file\n";
	gotoxy(45, 7);
	cout << "2. Save the current file\n";
	gotoxy(45, 8);
	cout << "3. Load an existing file\n";
	gotoxy(45, 9);
	cout << "4. Exit (Save before exiting)\n";
	gotoxy(45, 10);
	cout << "Choose an option: ";
}

void drawLayout() {
	// Clear the console
	system("cls");

	gotoxy(1, 0);
	cout << "Welcome to NotePad! you can write your text in this area.";
	// Draw the text area
	for (int i = 0; i <= textHeight; i++) {
		gotoxy(0, i);
		cout << "|";
	}
	// Draw the search area
	for (int i = 1; i < textWidth; i++) {
		gotoxy(i, textHeight);
		cout << "_";
	}
	gotoxy(textWidth + 1, 0);
	cout << "Search:";
	gotoxy(1, textHeight + 1);
	cout << "Suggestions:";
	// Draw the suggestions area
	for (int i = 0; i <= textHeight; i++) {
		gotoxy(textWidth, i);
		cout << "|";
	}

	// Set cursor to the beginning of the text area
	gotoxy(1, 1);
}


struct Node {
	int x, y;
	Node* left;
	Node* right;
	Node* up;
	Node* down;
	char character;
	char isColor;
	Node() {
		left = NULL;
		right = NULL;
		up = NULL;
		down = NULL;
		isColor = 'n';
	}
	Node(char data, int x = 1, int y = 1) :x(x), y(y), character(data) {
		left = NULL;
		right = NULL;
		up = NULL;
		down = NULL;
		isColor = 'n';
	}
	~Node() {
	}

};
struct ChilliMilli {
	Node* startWord;
	Node* curr;
	ChilliMilli** next;
	int num;
public:
	ChilliMilli() {
		num = 0;
		startWord= NULL;
		curr = startWord;
		next = NULL;
	}
	void addNewCharacter(Node*& s) {
		if (startWord==NULL) {
			startWord= s;
			curr = startWord;
			return;
		}
		curr->right = s;
		curr = curr->right;
	}
	bool searchWord(Node* word, ChilliMilli*& h) {
		// Base case: if h is NULL, return false
		if (h == NULL) {
			return false;
		}
		Node* word2 = h->startWord;
		bool found = true;
		while (word2 && word) {
			if (word2->character != word->character) {
				found = false;
				break;  
			}
			word2 = word2->right; 
			word = word->right;   
		}
		if ((word2 && !word) || (!word2 && word)) {
			found = false;
		}
		if (found) {
			return true;
		}
		int k = h->num;
		for (int i = 0; i < k; i++) {
			if (searchWord(word, h->next[i])) {
				return true;
			}
		}
		return false;
	}
	void DisplayChilli(ChilliMilli* k) {
		if (k == NULL) {
			return;
		}
		else {
			Node* r = k->startWord;
			if (r) {
				while (r) {
					cout << r->character;
					r = r->right;
				}
				cout << " ";
			}
			int j = k->num;
			for (int i = 0; i < j; i++) {
				DisplayChilli(k->next[i]);
			}
		}
	}
	void displayChilliSugg(){
		for (int i = 0; i < num; i++) {
			if (next[i]) {
				Node* start = next[i]->startWord;
				while (start) {
					cout << start->character;
					start = start->right;
				}
				cout << endl;
			}
		}
}
	ChilliMilli*& addNext(Node* word, ChilliMilli* head, ChilliMilli*& curr) {
		if (next == NULL) {
			next = new ChilliMilli *[++num];
			next[0] = new ChilliMilli;
		}
		else {
			ChilliMilli** temp = next;
			next = new ChilliMilli * [++num];
			for (int i = 0; i < num - 1; i++) {
				next[i] = temp[i];
			}
		}
			bool j = searchWord(word, head);
			if (!j) {
				startWord = word;
				next[num - 1] = new ChilliMilli();
				return next[num - 1];
			}
			return curr;
	}
};
class Lines {
	Lines* lUp;
	Lines* lDown;
	Node* curr;
public:
	Lines() {
		lUp = nullptr;
		lDown = NULL;
		curr = NULL;
	}
	Lines*& getUp() {
		return lUp;
	}
	Lines*& getDown() {
		return lDown;
	}
	Node*& getCurr() {
		return curr;
	}
	void setLineCurr(Node*& r) { //set current line
		curr = r;
	}
};

struct Stack {
private:
	Node** data; //array of words
	int top;
	const int size;
public:
	Stack() : size(5)
	{
		data = new Node * [5];
		top = -1;
	}
	~Stack()
	{
		delete[] data;
	}
	Node*& pop()
	{
		if (!isEmpty()) {
			return data[top--];
		}
	}
	bool isEmpty() {
		if (top == -1) {
			return true;
		}
		return false;
	}
	bool isFull() {
		if (top == size - 1) {
			return true;
		}
		return false;
	}
	void push(Node* x) {
		if (isFull()) {
			return;
		}
		data[top + 1] = new Node(x->character, x->x, x->y);
		Node* temp = data[top + 1];
		while (x->right) {
			x = x->right;
			temp->right = new Node(x->character, x->x, x->y);
			temp->right->left = temp;
			temp = temp->right;
		}
		top++;
	}
	void push(Node* x, Node*& a, Node*& a2)
	{
		if (isFull()) {
			return;
		}
		else if (x->character == ' ') {
			data[top + 1] = new Node(x->character, x->x, x->y);
			Node* temp = data[top + 1];
			while (temp->right) {
				temp = temp->right;
			}
			top++;
			while (a->right) {
				a = a->right;
			}
			if (a == nullptr) {
				a = new Node('i');
				a2 = a;
			}
			else {
				a2->right = new Node('i');
				a2->right->left = a2;
				a2 = a2->right;
			}
		}
		else if (x->x == 1 && x->y == 1) {
			data[top + 1] = new Node(x->character, x->x, x->y);
			top++;
			if (a == nullptr) {
				a = new Node('i');
				a2 = a;
			}
			else {
				a2->right = new Node('i');
				a2->right->left = a2;
				a2 = a2->right;
			}
		}
		else {
			Node* temp = data[top];
			while (temp->right) {
				temp = temp->right;
			}
			temp->right = new Node(x->character, x->x, x->y);
			temp->right->left = temp;
		}
	}
	Node*& getTop()
	{
		return data[top];
	}
};

struct NAryTree {
	NAryTree** alphabets;
	char data;
	bool end;
	int freq;
	int currN;
	int* y;
	int* x;
	Node** dataN;
	NAryTree() {
		dataN = new Node * [26];
		alphabets = new NAryTree * [26];
		for (int i = 0; i < 26; i++) {
			alphabets[i] = NULL;
			dataN[i] = nullptr;
		}
		end = false;
		freq = 0;
		currN= 0;
	}
	NAryTree(char data1, int l, int x1) {
		currN = 0;
		dataN = new Node * [26];
		alphabets = new NAryTree * [26];
		for (int i = 0; i < 26; i++) {
			alphabets[i] = NULL;
			dataN[i] = nullptr;
		}
		end = false;
		x[currN] = x1;
		y[currN++] = l;
		data = data1;
	}
	NAryTree(Node*& data1) {
		currN = 0;
		y = new int[26];
		x = new int[26];
		dataN = new Node * [26];
		alphabets = new NAryTree * [26];
		for (int i = 0; i < 26; i++) {
			alphabets[i] = NULL;
			dataN[i] = nullptr;
		}
		end = false;
		dataN[currN++] = data1;
		data = data1->character;
	}
	void deleteNodeNary(NAryTree*& root, char data, int x, int y) {
		
	}
	NAryTree*& addData(Node*& node) {
		int i = 0;
		if (node->character >= 97)
			i -= 32;
		i += (node->character - 65);
		if (alphabets[i] == NULL)
			alphabets[i] = new NAryTree(node);
		else {
			alphabets[i]->freq++;
			alphabets[i]->dataN[alphabets[i]->currN++] = node;
		}
		return alphabets[i];
	}
	void addData(char data, int l, int x) {
		int i = 0;
		if (data >= 97)
			i -= 32;
		i += (data - 65);
		if (alphabets[i] == NULL)
			alphabets[i] = new NAryTree(data, l, x);
		else {
		alphabets[i]->freq++;
		alphabets[i]->y[currN] = l;
		alphabets[i]->x[currN++] = x;
	    }
	}
	NAryTree*& searchNode(NAryTree*& root, char data) {
		int i = 0;
		if (data >= 97)
			data -= 32;
		i = data - 65;
		return (alphabets[i]);
	}
	void traverseTree(NAryTree*& root, char* arr) {

		NAryTree* r = root;
		int curr = r->currN - 1;
		for (int i = 0; i < curr; i++) {
			Node* currNode = r->dataN[i];
			cout << currNode->y << ":";
			cout << arr;
			for (int i = 0; currNode && currNode->character != ' '; currNode = currNode->right)
				cout << currNode->character;
			cout << " ";
		}

	}
	void traverseNTree(NAryTree*& r) {
		if (r == NULL)
			return;
		else {
			cout << r->data;
			for (int i = 0; i < 26; i++) {
				traverseNTree(r->alphabets[i]);
			}
		}
	}

	void DisplayNary() {
		system("cls");
		drawLayout();
		//gotoxy(1, 1);
		for (int i = 0; i < 26; i++) {
			if (alphabets[i] != NULL) {
				traverseNTree(alphabets[i]); // Traverse and display starting from each child
				cout << " ";
			}
		}
	}

};
class NotePad {
	Stack stack;  //undo insert stack
	Stack redoStack;  //redo insert stack
	Stack stackBack;
	Stack redoStackBack;
	Node* action;
	Node* latestAction;
	Node* currStack;
	Node* head;
	Node* cursor;
	NAryTree* headN;
	NAryTree* currHeadN;
	bool back;
	Lines* lineCursor;
	bool newWord;
	ChilliMilli* chilliHead;
	ChilliMilli* currChilli;
	Node* word;
	Node* currWord;
public:
	Node* searchVal;
	Node* currSearch;
	NotePad() {
		searchVal = NULL;
		currChilli = NULL;
		chilliHead = NULL;
		newWord = false;
		action = nullptr;
		latestAction = nullptr;
		head = nullptr;
		cursor = NULL;
		back = false;
		currStack = NULL;
		lineCursor = new Lines;
		currHeadN = new NAryTree;
		headN = currHeadN;
		word=NULL;
		currWord = NULL;
	}
	Stack& getStack() {
		return stack;
	}
	Lines*& getLineCursor() {
		return lineCursor;
	}
	void addNodeNary(Node*& n) {
		if (n->character == ' ') { //new word
			currHeadN->end = true;
			currHeadN = headN;
			return;
		}
		else { //if new word is being entered and it's it's first letter
			currHeadN = currHeadN->addData(n);
		}
	}
	void addToChilliMilli(char s) {
		if (s == ' ') {
			if (chilliHead == NULL) {
				return;
			}
			else {
				currChilli = currChilli->addNext(word, chilliHead, currChilli);
				word = NULL;
				currWord = NULL;
			}
		}
		else {
			if (chilliHead == NULL) {
				chilliHead = new ChilliMilli();
				currChilli = chilliHead;
			}
				if (word == NULL) {
					word = new Node(s);
					currWord = word;
				}
				else {
					currWord->right = new Node(s);
					currWord = currWord->right;
				}
		}
	}
	void searchChilliTree(ChilliMilli* tempChilli) {
		if (cursor == NULL)
			return;
		int k = tempChilli->num;
		if(tempChilli) {
			Node* temp = tempChilli->startWord;
			Node* word = searchVal;
			int l = tempChilli->num;
			bool found = true;
			while (temp && word) {
				if (temp->character != word->character) {
					found = false;
					break;
				}
				else {
					temp = temp->right;
					word = word->right;
				}
			}
			if (found) {
				gotoxy(1, 26);
				for (int i = 0; i < k; i++) {
					(tempChilli->next[i])->displayChilliSugg();
				}
				return;
			}
			else {
				for (int i = 0; i < k; i++) {
					searchChilliTree(tempChilli->next[i]);
				}
			}
		}
		
	}
	void moveToNextLine(char data, int& x, int& y) {
		int y1 = cursor->y;
		Lines* temp = lineCursor;
		while (temp->getCurr()->y != y1) {
			temp = temp->getDown();
		}
		if (data == ' ') {
			Node* curr = nullptr;
			Lines* temp2 = temp->getDown();
			temp->getDown() = new Lines;
			temp->getDown()->getUp() = temp;
			temp->getDown()->getDown() = temp2;
			if (temp2)
				temp2->getUp() = temp->getDown();
			temp->getDown()->getCurr() = new Node(data, 1, y1 + 1);
			temp2 = temp->getDown()->getDown();
			int x1 = 1;
			y1++;
			x = x1;
			y = y1;
			cursor = temp->getDown()->getCurr();
			while (temp2) {
				curr = temp2->getCurr();
				while (curr /*&& curr->right*/ != NULL) {
					curr->x = x1;
					curr->y = y1 + 1;
					x1++;
					curr = curr->right;
				}
				x1 = 1;
				y1++;
				temp2 = temp2->getDown();
			}
			return;
		}
		else {
			Node* curr = cursor;
			while (curr->left->character != ' ') {
				curr = curr->left;
			}
			Lines* temp2 = temp->getDown();
			temp->getDown() = new Lines;
			temp->getDown()->getUp() = temp;
			temp->getDown()->getDown() = temp2;
			if (temp2)
				temp2->getUp() = temp->getDown();
			temp->getDown()->getCurr() = curr;
			int x1 = 1;
			cursor = curr;
			curr->left->right = nullptr;
			while (cursor && cursor->right != NULL) {
				cursor->x = x1;
				cursor->y = y1 + 1;
				x1++;
				cursor = cursor->right;
			}
			if (cursor) {
				cursor->x = x1++;
				cursor->y = y1 + 1;
			}
			x = x1;
			y = y1 + 1;
			temp2 = temp->getDown()->getDown();
			x1 = 1;
			y1++;
			while (temp2) {
				curr = temp2->getCurr();
				while (curr && curr->right != NULL) {
					curr->x = x1;
					curr->y = y1 + 1;
					x1++;
					curr = curr->right;
				}
				x1 = 1;
				y1++;
				temp2 = temp2->getDown();
			}
		}
	}
	void searchText(char d) {
		Node* curr = cursor;
		char arr[20];
		arr[0] = '\0';
		if (cursor == NULL)
			return;
		while (curr->left !=nullptr && curr->left->character != ' ') {
			curr = curr->left;
		}
		NAryTree* temp = headN;
		bool found = false;
		if (curr == cursor)
		{
			gotoxy(1, 26);
			temp->traverseTree(currHeadN, arr);
			return;
		}
		int i = 0;
		while (curr!=cursor->right) {
			NAryTree* node = temp->searchNode(temp, curr->character);
			if(curr!=cursor)
			arr[i++] = curr->character;
			if (node != nullptr) {
				temp = node;
				curr = curr->right;
			}
		}
		arr[i] = '\0';
		gotoxy(1, 26);
		temp->traverseTree(temp, arr);

	}
	void insertData(char data, int x, int y) {
		//checks if alphaneumeric
		if (data >= 65 && data <= 90 || data >= 97 && data <= 122 || data == 32) {
			//checks if within the text area
			if (x >= textWidth || y >= textHeight) {
				if (x >= textWidth && y >= textHeight) {
					return;
				}
				moveToNextLine(data, x, y);
			}
			Node* newChar = new Node(data, x, y);
			addNodeNary(newChar);
			addToChilliMilli(data);
			//if first node			
			if (head == nullptr) {
				head = newChar;
				lineCursor->setLineCurr(head);
				cursor = head;
				stack.push(newChar, action, latestAction);
				return;
			}
			if (cursor->character == '\0') {
				cursor->character = data;
				cursor->x = x;
				cursor->y = y;
				cursor->right = newChar;
				cursor = cursor->right;
				return;
			}
			else {
				Node* temp = cursor->right;
				cursor->right = newChar;
				newChar->left = cursor;
				newChar->right = temp;
				if (temp) {
					newChar->up = temp->up;
					newChar->down = temp->down;
					temp->left = newChar;
					//adjust nodes after adding a node in between
					while (temp->right) {
						temp->y = temp->right->y;
						temp->up = temp->right->up;
						temp->down = temp->right->down;
						if (temp->down)
							temp->down->up = temp;
						if (temp->up)
							temp->up->down = temp;
						temp = temp->right;
					}
					temp = newChar->right;
					while (temp) {
						temp->x++;
						temp = temp->right;
					}
				}
			}
			cursor = newChar;
			stack.push(newChar, action, latestAction);

		}
		else
			return;
	}
	void addToStackCursor() {
		if (cursor->character == ' ' &&/* back == false*/ currStack == NULL || cursor->right == nullptr && currStack == NULL) {
			back = true;
			currStack = new Node(cursor->character, cursor->x, cursor->y);
			currStack->right = NULL;
		}
		else if (back == true && cursor->left == nullptr || back == true && cursor->left->character == ' ') {
			back = false;
			currStack->left = new Node(cursor->character, cursor->x, cursor->y);
			currStack->left->right = currStack;
			currStack = currStack->left;
			stackBack.push(currStack);
			if (action == nullptr) {
				action = new Node('b');
				latestAction = action;
			}
			else {
				latestAction->right = new Node('b');
				latestAction->right->left = latestAction;
				latestAction = latestAction->right;
			}
			currStack = NULL;
		}
		if (back == true && cursor->character != ' ') {
			currStack->left = new Node(cursor->character, cursor->x, cursor->y);
			currStack->left->right = currStack;
			currStack = currStack->left;
		}
	}
	void removeData() {
		if (cursor == nullptr) return; // Nothing to remove
		addToStackCursor();
		Node* currN = cursor;        // Current node to delete
		Node* PrevN = currN->left;   // Previous node
		if (PrevN) {
			// If there's a previous node
			PrevN->right = currN->right; // Link the previous node to the next node
			if (currN->right) {
				currN->right->left = PrevN; // Link the next node back to PrevN
			}
			Node* temp = PrevN->right;
			while (temp) {
				temp->x--;
				temp = temp->right;
			}
		}
		else if (currN != head) {
			Lines* newLine = lineCursor;
			int y = cursor->y - 1;
			while (newLine->getCurr()->y != y) {
				newLine = newLine->getDown();
			}
			cursor = newLine->getCurr();
			while (cursor && cursor->right) {
				cursor = cursor->right;
			}
			if (cursor)
				cursor->right = currN->right;
			if (cursor && cursor->right) {
				Node* temp1 = cursor;
				while (temp1->right) {
					temp1->right->x = temp1->x + 1;
					temp1->right->y = temp1->y;
					temp1 = temp1->right;
				}

			}
			Lines* l = newLine->getDown(); // Update head
			if (l) {
				newLine->getDown() = l->getDown();
				l->getUp() = newLine;
			}
			else {
				newLine->getDown() = nullptr;
			}
			if (newLine->getCurr()) {
				newLine->getCurr()->left = nullptr;
			}
			return;
		}
		else if (head == currN && currN->right == nullptr) {
			head = new Node('\0', 1, 1);
			lineCursor->setLineCurr(head);
			cursor = head;
			return;
		}
		else {
			if (cursor->right) {
				cursor->right->x = cursor->x;
				cursor->right->y = cursor->y;
				cursor = cursor->right;
			}
			else {
				cursor = new Node('\0', 1, 1);
			}
			lineCursor->setLineCurr(cursor);
			head = cursor;
			return;
		}
		cursor = PrevN;

	}
	void display() {
		system("cls");
		drawLayout();
		int x = 1;
		int y = 1;
		gotoxy(x, y);
		Lines* row = lineCursor;
		while (row != nullptr) {
			Node* current = row->getCurr();
			while (current != nullptr) {
				gotoxy(current->x, current->y);
				if (current->isColor == 'Y')
					setColor(13, 0);
				cout << current->character;
				setColor(15, 0);
				current = current->right; // Move to the right node
			}
			cout << endl;
			row = row->getDown(); // Move to the next row
		}
		gotoxy(92, 1);
		Node* t = searchVal;
		while (t!=NULL) {
			cout << t->character;
			t = t->right;
		}
	}
	void updateCursor(char ch) {
		if (ch == 'u' && lineCursor && cursor) {
			Lines* line = lineCursor;
			while (line->getCurr()->y != cursor->y) {
				line = line->getDown();
			}
			line = line->getUp();
			if (line) {
				Node* temp = line->getCurr();
				while (temp->x != cursor->x) {
					temp = temp->right;
				}
				cursor = temp;
			}
			return;
		}
		if (ch == 'd' && lineCursor && cursor) {
			Lines* line = lineCursor;
			while (line->getCurr()->y != cursor->y) {
				line = line->getDown();
			}
			line = line->getDown();
			if (line) {
				Node* temp = line->getCurr();
				while (temp->x != cursor->x) {
					temp = temp->right;
				}
				cursor = temp;
			}
			return;
		}
		if (ch == 'r' && cursor) {
			if (cursor->right == nullptr) {
				Lines* line = lineCursor;
				while (line->getCurr()->y != cursor->y) {
					line = line->getDown();
				}
				line = line->getDown();
				if (line)
					cursor = line->getCurr();
				else return;
			}
			else
				cursor = cursor->right;
			return;
		}
		if (ch == 'l') {
			if (cursor->left == nullptr) {
				Lines* line = lineCursor;
				while (line->getCurr()->y != cursor->y) {
					line = line->getDown();
				}
				line = line->getUp();
				if (line)
					cursor = line->getCurr();
				else return;
				while (cursor->right) {
					cursor = cursor->right;
				}
			}
			else
				cursor = cursor->left;
			return;
		}
		if (ch == 'e' && cursor) {
			Lines* newLine = lineCursor;
			int y = cursor->y;
			while (newLine->getCurr()->y != y) {
				newLine = newLine->getDown();
			}
			Lines* t = newLine->getDown();
			newLine->getDown() = new Lines; // Link the new line to the current line
			newLine->getDown()->getUp() = newLine;
			newLine->getDown()->getDown() = t;
			newLine = newLine->getDown();
			if (newLine->getDown()) {
				Node* curr = newLine->getDown()->getCurr();
				while (curr) {
					curr->y++;
					curr = curr->right;
				}
			}
			// Set the cursor to the leftmost position in the new line
			if (cursor->right == nullptr) {
				Node* newNode = new Node('\0', 1, cursor->y + 1);
				newLine->setLineCurr(newNode); // Start new line with cursor position
			}
			else {
				Node* newNode = new Node(cursor->right->character, 1, cursor->y + 1);
				newNode->right = cursor->right->right;
				newNode->right->left = newNode;
				newLine->setLineCurr(newNode); // Start new line with cursor position
				Node* temp = newNode->right;
				while (temp) {
					temp->x = temp->left->x + 1;
					temp->y = y + 1;
					temp = temp->right;
				}
				cursor->right = NULL;

				//newLine->setLineCurr(cursor->right); // Start new line with cursor position
				//newLine->getCurr()->x = 0;
				//newLine->getCurr()->y = y+1;
				//cursor->right = NULL;
			}
			/*newLine->getCurr()->up = newLine->getUp()->getCurr();
			newLine->getCurr()->down = newLine->getDown()->getCurr();*/
			cursor = newLine->getCurr(); // Update cursor to point to the new line
			return;
		}
	}
	Node*& getCursor() {
		return cursor;
	}
	void UndoInsert() {
		if (!stack.isEmpty()) {
			Node* temp = stack.pop(); //pop latest word added
			redoStack.push(temp);
			Lines* l = lineCursor;
			while (l->getCurr()->y != temp->y) { //find the line 
				l = l->getDown();
			}
			Node* curr = l->getCurr();
			while (curr->x != temp->x) { //find the word's x-coordinate
				curr = curr->right;
			}
			Node* temp2 = curr->left;
			while (temp->right) {
				temp = temp->right; //find where the word ends
			}
			while (curr->x != temp->x) {
				curr = curr->right;
			}
			temp2->right = curr->right;
			if (curr && curr->right) {
				curr->right->left = temp2;
			}
			int x = temp2->x;
			while (curr) { //update the nodes' coordinates accordingly
				curr->x = ++x;
				curr = curr->right;
			}
			cursor = temp2;
		}
	}
	void popWord() {
		if (!stackBack.isEmpty()) {
			Node* temp = stackBack.pop(); // pop latest word deleted
			redoStackBack.push(temp);
			Lines* l = lineCursor;
			while (l->getCurr()->y != temp->y) { //find its line
				l = l->getDown();
			}
			Node* curr = l->getCurr();
			while (curr->x != temp->x) { //find its x-coordinate
				curr = curr->right;
			}
			Node* temp2 = curr;
			temp->left = temp2->left;
			if (curr->left)
				curr->left->right = temp;
			else {
				l->getCurr() = temp;
			}
			while (temp->right) {
				temp = temp->right;
			}
			temp->right = temp2;
			int x = temp->x;
			while (temp2) {
				temp2->x = ++x;  //adjust nodes' coordinates accordingly
				temp2 = temp2->right;
			}
			cursor = temp;
		}

	}
	Node*& GetLatestAct() {
		return latestAction;
	}
	void redoInsert() {
		if (!redoStack.isEmpty()) {
			Node* temp = redoStack.pop(); //pop latest thing that was undo
			Lines* l = lineCursor;
			while (l->getCurr()->y != temp->y) { //find its line
				l = l->getDown();
			}
			Node* curr = l->getCurr();
			while (curr->x != temp->x - 1) {
				curr = curr->right;
			}
			Node* temp2 = curr->right;
			temp->left = curr;
			curr->right = temp;
			while (temp->right) {
				temp = temp->right;
			}
			temp->right = temp2;
			int x = temp->x;
			while (temp2) {  //adjust nodes's coordinates
				temp2->x = ++x;
				temp2 = temp2->right;
			}
			cursor = temp; //update cursor
		}
	}
	void redo() {
		if (!redoStackBack.isEmpty()) {
			Node* temp = redoStackBack.pop(); //pop latest thing that was undo from back stack
			Lines* l = lineCursor;
			while (l->getCurr()->y != temp->y) {  //find line
				l = l->getDown();
			}
			Node* curr = l->getCurr();
			while (curr->x != temp->x) {
				curr = curr->right;
			}
			Node* temp2 = curr->left;
			while (temp->right) {
				temp = temp->right;
			}
			while (curr->x != temp->x) {
				curr = curr->right;
			}
			temp2->right = curr->right;
			if (curr && curr->right) {
				curr->right->left = temp2;
			}
			int x = temp2->x;
			while (curr) {
				curr->x = x++;  //adjust coordinates
				curr = curr->right;
			}
			cursor = temp2; //update cursor
		}
	}
	void insertSearchVal(char ch) {
		if (searchVal == nullptr) {
			searchVal = new Node(ch);
			currSearch = searchVal;
			return;
		}
		else {
			currSearch->right = new Node(ch);
			currSearch=currSearch->right;
		}
	}
	
	void searchSubString() {
		if (searchVal == NULL)
			return;
		else {
			char ch = searchVal->character;
			NAryTree* tr = headN;
			int size = tr->currN;
			Node* q = searchVal;
			while (q) {
				if (q->character == ' ') {
					if (tr->end == true) {
						tr = headN;
						q = q->right;
						continue;
					}
					else
						return;
				}
				ch = q->character;
				tr = tr->searchNode(tr, ch);
				if (tr == nullptr)
					return;
				q = q->right;
			}
				if (tr->end==true) {
					searchAndColor();
					display();
					int l = tr->currN;
					for (int i = 0; i < l; i++) {
						gotoxy(92, i + 2);
						cout << "Line number: " << tr->dataN[i]->y << endl;
					}
				}
		}
	}
	void searchAndColor() {
		Node* s = searchVal;
		Node* h = head;
		NAryTree* t = headN->searchNode(headN,searchVal->character);
		int l = t->currN;
		for (int i = 0; i < l; i++) {
			bool found = true;
			Node* n = t->dataN[i];
			Node* x = searchVal;
			while (x&&n) {
				if (x->character != n->character) {
					found = false;
					break;
				}
				x = x->right;
				n = n->right;
			}
			if (found) {
				n = t->dataN[i];
				x = searchVal;
				while (x&&n) {
					n->isColor = 'Y';
					x = x->right;
					n = n->right;
				}
			}
		}
		//headN->DisplayNary();
	}
	void removeColor() {
		Lines* row = lineCursor;
		while (row != NULL) {
			Node* ptr = row->getCurr();
			row = row->getDown();
			while (ptr != NULL) {
				if (ptr->isColor == 'Y')
					ptr->isColor = 'n';
				ptr = ptr->right;

			}
		}
	}
	NAryTree*& getHeadNary() {
		return headN;
	}
	~NotePad() {
		//delete all nodes in notepad
		Lines* row = lineCursor;
		while (row != NULL) {
			Node* ptr = row->getCurr();
			row = row->getDown();
			while (ptr != NULL) {
				Node* temp = ptr;
				ptr = ptr->right;
				delete temp;
			}
		}
	}
	ChilliMilli*& getchillHead() {
		return chilliHead;
	}
};

void MenuOptions(int ch, filesystem::path filename, NotePad& note) {
	/*ofstream outFile(filename);*/
	std::fstream outFile(filename, std::ios::in | std::ios::out | std::ios::app);
	if (!outFile.is_open()) {
		cout << "Error: Could not create file " << filename << endl;
		return;
	}

	if (ch == 2 || ch == 4) {
		Lines* row = note.getLineCursor();
		while (row != nullptr) {
			Node* current = row->getCurr();
			while (current != nullptr) {
				outFile << current->character;
				current = current->right; // Move to the right node
			}
			outFile << endl;
			row = row->getDown(); // Move to the next row
		}
	}
	// Close the file
	outFile.close();
}
int main(int argc, char* argv[]) {
	RemoveScroll(); //function to remove scroll bar
	getConsoleSize(); //console sizes
	NotePad notePad;
	system("cls");
	HANDLE  rhnd = GetStdHandle(STD_INPUT_HANDLE);  // handle to read console

	DWORD Events = 0;     // Event count
	DWORD EventsRead = 0; // Events read from console
	filesystem::path filePath;;
	bool Running = true;

	int x = 1, y = 1;

	menu(); //print menu
	int ch;
	cin >> ch; //get choice
	if (ch == 1 || ch == 3) {
		gotoxy(45, 11);
		cout << "Enter file name: ";
		cin >> filePath;
	}
	MenuOptions(ch, filePath, notePad); //function for file handling
	drawLayout();
	gotoxy(x, y);
	//programs main loop
	while (Running) {
		getConsoleSize();
		// gets the systems current "event" count
		GetNumberOfConsoleInputEvents(rhnd, &Events);

		if (Events != 0) { // if something happened we will handle the events we want

			// create event buffer the size of how many Events
			INPUT_RECORD eventBuffer[200];

			// fills the event buffer with the events and saves count in EventsRead
			ReadConsoleInput(rhnd, eventBuffer, Events, &EventsRead);

			// loop through the event buffer using the saved count
			for (DWORD i = 0; i < EventsRead; ++i) {

				// check if event[i] is a key event && if so is a press not a release
				if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown) {


					// check if the key press was an arrow key
					switch (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode) {
					case '4':
						MenuOptions(4, filePath, notePad);
						Running = false; //break loop
						break;
					case VK_BACK:
						x--;
						notePad.removeData();  //function to remove node where cursor is
						notePad.display();
						if (notePad.getCursor()) { //update coodinates
							x = notePad.getCursor()->x;
							y = notePad.getCursor()->y;
						}
						gotoxy(x, y);
						break;
					case VK_ESCAPE:
						menu();
						cin >> ch;
						if (ch == 1 || ch == 3) {
							gotoxy(45, 11);
							cout << "Enter file name: ";
							cin >> filePath;
						}
						notePad.display();
						gotoxy(x, y);
						break;
					case '1':
						system("cls");
						ch = 1; //get choice
						gotoxy(45, 11);
						cout << "Enter file name: ";
						cin >> filePath;
						MenuOptions(1, filePath, notePad);
						notePad.display();
						gotoxy(x, y);
						break;
					case '3':
						system("cls");
						ch = 3; //get choice
						gotoxy(45, 11);
						cout << "Enter file name: ";
						cin >> filePath;
						if (!std::filesystem::exists(filePath))
							cout << "The file does not exist. Creating a new file at " << filePath << std::endl;
						notePad.display();
						gotoxy(x, y);
						break;
					case '2':
						ch = 2;
						MenuOptions(2, filePath, notePad);
						notePad.display();
						gotoxy(x, y);
						break;
					case VK_TAB:
						x = 92;
						y = 1;
						gotoxy(x, y);
						//notePad.getchillHead()->DisplayChilli(notePad.getchillHead());
						//notePad.getHeadNary()->DisplayNary();
						break;
					case VK_CAPITAL:
						break; //ignore capsLock key press
					case VK_MENU:
						//check if latest thing done was insert or backspace word
						if (notePad.GetLatestAct()) {
							char act = notePad.GetLatestAct()->character;
							if (act == 'i') {
								notePad.redoInsert();
							}
							else if (act == 'b')
								notePad.redo();

							notePad.GetLatestAct() = notePad.GetLatestAct()->left;
						}
						notePad.display();
						if (notePad.getCursor()) {
							x = notePad.getCursor()->x;
							y = notePad.getCursor()->y;
						}
						gotoxy(x, y);
						break;
					case VK_SHIFT: // Check if Shift is pressed
						if ((GetAsyncKeyState(VK_SHIFT) & 0x8000)
							&& (GetAsyncKeyState('2') & 0x8000)) {
							notePad.searchText(notePad.getCursor()->character);
						}
						else if((GetAsyncKeyState(VK_SHIFT) & 0x8000) 
							&& (GetAsyncKeyState('8') & 0x8000)) {
							notePad.searchChilliTree(notePad.getchillHead());
						}
					//case '=':
					//	//check if latest thing done was insert or backspace word
					//	if (notePad.GetLatestAct()) {
					//		char act = notePad.GetLatestAct()->character;
					//		if (act == 'i') {
					//			notePad.UndoInsert();
					//		}
					//		else if (act == 'b')
					//			notePad.popWord();
					//	}
					//	notePad.display();
						if (notePad.getCursor()) {
							x = notePad.getCursor()->x;
							y = notePad.getCursor()->y;
						}
						gotoxy(x, y);
						break;
					case VK_UP: //up
						notePad.updateCursor('u');
						x = notePad.getCursor()->x + 1;
						y = notePad.getCursor()->y;
						gotoxy(x, y);
						break;
					case VK_DOWN: //down
						notePad.updateCursor('d');
						x = notePad.getCursor()->x + 1;
						y = notePad.getCursor()->y;
						gotoxy(x, y);
						break;
					case VK_RIGHT: //right
						notePad.updateCursor('r');
						x = notePad.getCursor()->x + 1;
						y = notePad.getCursor()->y;
						gotoxy(x, y);
						break;
					case VK_RETURN: // Enter key
						if (x >= 92) {
							x = 92;
							y++;
							gotoxy(x, y);
							notePad.searchSubString();
							//notePad.display();
							x = notePad.getCursor()->x+1;
							y = notePad.getCursor()->y;
							gotoxy(x, y);
							notePad.removeColor();
							delete notePad.searchVal;
							notePad.searchVal = nullptr;
							notePad.currSearch = nullptr;
							break;
						}
						else {
							x = 1; // Reset cursor to the start of the new line, adjust as needed
							y++;   // Move cursor down to the next line
							notePad.updateCursor('e');
							notePad.display();
							gotoxy(x, y);
							break;
						}
					case VK_LEFT: //left
						notePad.updateCursor('l');
						x = notePad.getCursor()->x + 1;
						y = notePad.getCursor()->y;
						gotoxy(x, y);
						break;
					default:
						if (x >= 92) {
							notePad.insertSearchVal(eventBuffer->Event.KeyEvent.uChar.AsciiChar);
							notePad.display();
						}
						else {
							notePad.insertData(eventBuffer->Event.KeyEvent.uChar.AsciiChar, x, y);
							notePad.display();
							x = notePad.getCursor()->x + 1;
							y = notePad.getCursor()->y;
							gotoxy(x, y);
							break;
						}
					}
				}

			} // end EventsRead loop

		}

	} // end program loop
	system("cls");
	//save data in file if esc is pressed
	return 0;
}
