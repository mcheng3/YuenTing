## Dec. 7, 2018

- Integrated cppjieba with openFrameworks.  
- Created Dictionary class, which holds Chinese to English dictionary and methods used to parse and translate Chinese text.
- Display UTF-8 Chinese text with openFrameworks successfully.
- Add feature which allows user to hover of phrase to view definitions.


Still to do:  
- Add method to input Chinese text (through GUI? Text files?)
- Make GUI prettier
- Support pronunciations of different dialects
- Support Simplified Chinese


## Dec. 12, 2018

- Created settings and load file GUI
- Implemented Cantonese, Mandarin, Korean
- Implemented Simplified Chinese support
- Properly displays punctuation and newlines now.
- Refactored dictionary class to make the hover function much faster.
- Implemented 2-pass parsing to reduce amount of undefined words.