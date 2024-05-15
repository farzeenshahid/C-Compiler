#include <iostream>
#include<stdio.h>
#include<fstream>
#include "wordBreak.h"
#include <string>
#include "Token.h"
#include "LinkedList.h"
#include "SyntaxAnalyzer.h"

using namespace std;

int main()
{
    WordBreak *word = new WordBreak("input2.txt");
    LinkedList *tokens = word->getTokens();

    // Create an output file and open it for writing
    std::ofstream outputFile("output.txt");
    if (!outputFile.is_open())
    {
        std::cerr << "Unable to open output file." << std::endl;
        return 1;
    }

    // Write the tokens to the output file
    tokens->append("$", "_", -1);
    std::streambuf *originalBuffer = std::cout.rdbuf(outputFile.rdbuf());
    tokens->print();
    std::cout.rdbuf(originalBuffer);
    
    // Close the output file
    outputFile.close();
    // Now, create the SyntaxAnalyzer and pass the tokens to it
    SyntaxAnalyzer* syntax_analyzer = new SyntaxAnalyzer(tokens);
    // Call the syntax analysis function here, or you can do it within the SyntaxAnalyzer constructor
    
    return 0;
}

