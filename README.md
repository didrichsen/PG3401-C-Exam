# PG3401 C Programming Exam

## How to Run

Tasks are written for linux/unix-like system. Navigate to folder and type "make"
to compile task and run. 

```bash
git clone <repository_url>
```
```bash
cd oppgave_2
```
```bash
make
```
```bash
./oppgave_2
```

## Exam Details
- **Duration**: 14 days
- **Grading Scale**: National grading scale A - F
- **Date**: April 26 - May 10, 2024
- **Programming Language**: C-89 (Other languages will not earn points)
- **Total Tasks**: 6

## Tasks Overview

### Task 1: Theory (5%)
1. Explain what the C programming language can be used for.
2. Who is Linus Torvalds, and what is he known for in Information Technology?
3. Explain the steps in compiling a C program, from source code to executable file.

### Task 2: File Handling and Functions (15%)

**Requirements**:
1. Read words from the input text file and analyze using the provided functions.
2. Create a struct to store metadata for each word.
3. Write the struct and word to a binary output file.
4. Create a makefile and necessary headers for compilation.

### Task 3: List Handling (20%)
- **Structure**:
    - Doubly linked list for table reservations.
    - Singly linked list for food orders per table.

**Functions**:
1. Add reservation, sorted by customer name.
2. Retrieve and display reservation details.
3. Search by customer name.
4. Delete reservation.
5. Add food/drink order.
6. Print orders and calculate total for a table.
7. Print orders and calculate total for an individual at a table.

**Main Function**:
1. Interactive menu for user operations.
2. Cleanup before exiting.

**Makefile and Headers**:
1. Ensure correct compilation and execution.

**Documentation**:
1. Include a screenshot (oppgave3_scrnshot.png).

### Task 4: Threads (15%)
- **Provided Files**:
    - Thread Application
    - Test File (Hamlet)

**Requirements**:
1. Modify global variables to local.
2. Accept file name as a command line parameter.
3. Replace conditions with semaphores.
4. Explicitly initialize mutexes and semaphores.
5. Compute SHA1 hash of the file using provided sha1.c and sha1.h files.

**Makefile and Headers**:
1. Ensure correct compilation and execution.

**Documentation**:
1. Include a screenshot (oppgave4_scrnshot.png).

### Task 5: Networking (25%)
- **Server and Client Implementation**:
    - Protocol for communication.
    - Server listens on user-specified port.
    - Client connects to server and sends messages.
    - Server prints received messages.

**Makefile and Headers**:
1. Ensure correct compilation and execution.

**Documentation**:
1. Include a screenshot (oppgave5_scrnshot.png).

### Task 6: File Handling and Text Parsing (20%)
- **Code Beautifier Application**:
    1. Replace while loops with for loops.
    2. Enforce Hungarian notation for char * variables.
    3. Replace three spaces with a tab.

**Makefile and Headers**:
1. Ensure correct compilation and execution.

**Documentation**:
1. Include the original and beautified files.
2. Test file named oppgave5_test.c.
