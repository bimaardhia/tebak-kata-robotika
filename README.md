# 🤖 Word Guessing - Robotics Edition

![C](https://img.shields.io/badge/language-C-blue.svg)
![Status](https://img.shields.io/badge/status-completed-brightgreen.svg)
![Course](https://img.shields.io/badge/course-Basic_Programming-orange.svg)

**Final Project for Basic Programming Practicum**

An interactive Command Line Interface <CLI> application for a Hangman game themed around Robotics and Artificial Intelligence terminology. This project is built applying _Software Engineering_ principles, including memory management, _pointer arithmetic_, _binary search_, and _insertion sort_.

---

## 🛠️ Compilation and Execution

This program is written in pure C and can be compiled using `gcc`.

**1. Compiling the Program**
Open your terminal and run the following command:

```bash
gcc -Wall -o word_guess word_guess.c
```

_Note: The `-Wall` flag is used to ensure no warnings occur during compilation._

**2. Running the Program**

- **Linux / macOS:**
  ```bash
  ./word_guess
  ```
- **Windows:**
  ```cmd
  word_guess.exe
  ```

---

## ✨ Features List

1.  **Play New Game:** The classic word-guessing game. The player guesses letter by letter of a randomly selected secret word.
2.  **Dynamic Scoring System:** Score calculation takes into account remaining lives and the length of the secret word <Formula: Remaining Lives x 100 + Word Length x 20>.
3.  **Hall of Fame:** Saves the top 10 players using a _Bubble Sort_ algorithm in descending order. Integrated with `<time.h>` for automatic date recording.
4.  **Add Word to Bank:** Players can expand the vocabulary bank. The program automatically checks for duplicates using a _Binary Search_ algorithm O<log n>, then inserts the new word alphabetically using _Insertion Sort_.
5.  **Robust Input Handling:** Implements an _input buffer clearing_ system to prevent _infinite loops_ when users enter space characters or excessive input.

---

## ⚠️ Known Limitations

- **Volatile Storage:** Data for new words and the Hall of Fame is only stored in memory <RAM> while the program is running. Data will reset if the program is closed, as it does not yet utilize _File I/O_ operations.
- **Fixed Capacity:** The word bank is limited to a maximum of 50 entries `MAX_WORDS`, and the Hall of Fame is limited to 10 entries `HOF_SIZE`.
- **Alphabet Characters Only:** The game currently does not specifically handle number or symbol inputs within the secret word, although inputs are automatically converted to uppercase.

---

## 📊 Main System Flowcharts

Below is the logical flow of the program depicted using Mermaid diagrams. As per standard practice, angle brackets are used for explanatory parameters.

### A. Main Menu Flow

```mermaid
flowchart TD
    Start[Start Program] --> Init[Initialize Random Seed]
    Init --> Show[Show Main Menu]
    Show --> Input[Read Input Choice]
    Input --> Switch{Evaluate Choice}

    Switch -->|1| Play[Execute play_game]
    Switch -->|2| HOF[Execute show_hall_of_fame]
    Switch -->|3| Add[Execute add_word]
    Switch -->|Other| Invalid[Print Invalid Choice Message]
    Switch -->|0| Exit[Print Thank You Message]

    Play --> CheckLoop
    HOF --> CheckLoop
    Add --> CheckLoop
    Invalid --> CheckLoop

    CheckLoop{Choice != 0 ?}
    CheckLoop -->|Yes| Show
    CheckLoop -->|No| Selesai[Exit Program]
    Exit --> Selesai
```

### B. Overall Game Session Flow <play_game>

```mermaid
flowchart TD
    Start[Start play_game] --> InputName[Input Player Name]
    InputName --> Init[Init Game Variables & Pick Random Word]
    Init --> LoopCond{wrong < 6 AND Not Won?}

    LoopCond -->|Yes| Draw[Draw Hangman & Print Info]
    Draw --> InputGuess[Input Letter Guess]
    InputGuess --> Process[Execute process_guess]
    Process --> LoopCond

    LoopCond -->|No| EndDraw[Draw Final Hangman]
    EndDraw --> CheckWin{Won?}

    CheckWin -->|Yes| CalcScore[Calculate Score]
    CalcScore --> AskHOF{Save to HOF?}
    AskHOF -->|Yes| Save[Execute save_score] --> Selesai[End play_game]
    AskHOF -->|No| Selesai

    CheckWin -->|No| ShowWord[Show Correct Word] --> Selesai
```

### C. Single Guess Process <process_guess>

```mermaid
flowchart TD
    Start[Start process_guess] --> Search[Search Letter in Guessed Array <Linear Search>]
    Search --> CheckExist{Letter Exists?}

    CheckExist -->|Yes| Ret1[Return Status 1 <Already Guessed>]
    CheckExist -->|No| AddGuessed[Record Letter to Guessed Array]

    AddGuessed --> Traversal[Traverse Secret Word Using Pointer]
    Traversal --> CheckChar{Letter Matches?}

    CheckChar -->|Yes| Buka[Reveal Letter in Display Mask] --> LanjutTelusur[Proceed to Next Character]
    CheckChar -->|No| LanjutTelusur
    LanjutTelusur --> CheckEnd{End of String?}
    CheckEnd -->|Not Yet| CheckChar
    CheckEnd -->|Reached| EvalMatch{Any Letter Matched?}

    EvalMatch -->|Yes| Ret2[Return Status 2 <Correct>]
    EvalMatch -->|No| IncWrong[Increment Wrong Count <wrong++>]
    IncWrong --> Ret0[Return Status 0 <Incorrect>]

    Ret1 --> Selesai[End process_guess]
    Ret2 --> Selesai
    Ret0 --> Selesai
```

### D. Word Addition Process <add_word & insert_word_sorted>

```mermaid
flowchart TD
    Start[Start add_word] --> CheckLimit{Word Bank Full?}
    CheckLimit -->|Yes| PrintFull[Print Full Message] --> Selesai[End add_word]
    CheckLimit -->|No| InputWord[Read New Word from Input]

    InputWord --> Upper[Convert to Uppercase]
    Upper --> BinSearch[Execute binary_search_word]

    BinSearch --> CheckFound{Result != -1 ?}
    CheckFound -->|Yes| PrintExist[Print Word Exists Message] --> Selesai
    CheckFound -->|No| InsertSort[Execute insert_word_sorted]

    InsertSort --> Alloc[Dynamic Memory Allocation <malloc>]
    Alloc --> LoopShift[Shift Larger Array Elements to the Right]
    LoopShift --> Assign[Assign Word Memory Address to Array]
    Assign --> IncCount[Increment word_count++]

    IncCount --> PrintSukses[Print Success Message] --> Selesai
```

### E. Score Saving and Sorting Flow <save_score & bubble_sort_hof>

```mermaid
flowchart TD
    Start[Start save_score] --> CheckQualify{Board Full & Score Lower?}
    CheckQualify -->|Yes| Reject[Print Unqualified for HOF Message] --> Selesai[End save_score]

    CheckQualify -->|No| GetTime[Get System Time <localtime>]
    GetTime --> InsertData[Write Name, Score, Date to Bottom of HOF Array]
    InsertData --> CheckCount{hof_count < HOF_SIZE?}
    CheckCount -->|Yes| IncCount[hof_count++] --> Sort[Execute bubble_sort_hof]
    CheckCount -->|No| Sort

    Sort --> BubbleOut[HOF Array Loop <Outer>]
    BubbleOut --> BubbleIn[HOF Array Loop <Inner>]
    BubbleIn --> CheckPoints{Current Points < Next Points?}
    CheckPoints -->|Yes| Swap[Swap Data Struct Positions] --> BubbleInNext[Continue Inner Loop]
    CheckPoints -->|No| BubbleInNext
    BubbleInNext --> BubbleOutNext[Continue Outer Loop]
    BubbleOutNext --> PrintSuccess[Print Date & Score Saved Message] --> Selesai
```

---

_This documentation is compiled to meet the submission standards for the Basic Programming Practicum Final Project._
