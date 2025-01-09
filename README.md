# Console-Based-Notepad
A console based notepad made using c++ programing language. it using OOP (Object Oriented Programing) DS (Data Structures) concepts.
---

## Features

### **Core Features**
- **Create, Save, and Load Files:**  
  Start a new text document, load an existing file, or save changes at any time.

- **Text Editing:**  
  Perform operations such as adding, deleting, and modifying text seamlessly. Includes:
  - **Undo and Redo**: using stacks.

- **Cursor Control and Navigation:**  
  Move the cursor through text using arrow keys, with multi-line support.

### **Advanced Features**
- **ChilliMilli Tree for Suggestions:**  
  A custom data structure designed for storing and suggesting words for sentence completion based on previosly entered 
  sentences.

- **Word Highlights:**  
  When searching for a substring, all occurrences of the word are visually highlighted.

- **Substring Search with Suggestions:**  
  - Perform real-time substring searches.  
  - Search results include line numbers and highlight matches in the text.  
  - Suggestions are displayed in the **Suggestions** area based on the context.

- **Efficient Text Storage with N-ary Tree:**  
  All words entered are stored in an N-ary tree for fast substring matching, word-completion, and searching.

---

## Key Data Structures

### **1. ChilliMilli Tree**
The ChilliMilli Tree is a custom trie-like structure used to store and retrieve words efficiently. It supports:  
- **Dynamic Word Addition:** Tracks each word entered during text editing.  
- **Suggestions:** Offers suggestions based on prefixes of words typed.  

### **2. N-ary Tree**  
The N-ary Tree is used for substring matching and search functionality. It provides:  
- **Fast Substring Search:** Traverses efficiently to locate matching nodes.  
- **Line and Position Tracking:** Each node stores information about its position within the text.  
- **Search and Color:** Highlights all occurrences of a substring in real-time.
  
### **3. Linked Lists**  
Used to manage text across lines.

### **4. Stacks**  
- **Undo Stack:** Reverses the most recent edits.  
- **Redo Stack:** Restores undone changes.

## Controls

### **Menu Options**
- **1:** Create a new file.  
- **2:** Save the current file.  
- **3:** Load an existing file.  
- **4:** Exit the application.

### **Keyboard Shortcuts**
- **Arrow Keys:** Navigate the cursor.  
- **Backspace:** Delete the character at the cursor.  
- **Enter:** Move to a new line or execute search.
- **@:** request for word completion.
- *aisterik:** request for sentence completion.
- **Tab:** go to the search area.  
- **Esc:** Return to the main menu.  
