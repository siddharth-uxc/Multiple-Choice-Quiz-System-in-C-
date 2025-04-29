#include<iostream>
#include<fstream>
#include<cstdlib>
#include<ctime>
#include<cstring>

using namespace std;

const int maxQuestion = 30;
const int OPTIONS = 4;

struct Question {
    char text[200];
    char options[OPTIONS][100];
    int correctAnswer;
};

void readQuestions(Question q[], int &count) {
    ifstream file("question.txt");
    if (!file) {
        cout << "Unable to open question.txt\n"; 
        exit(1);
    }

    count = 0;
    while (file.getline(q[count].text, 200)) {
        for (int i = 0; i < OPTIONS; i++) {
            file.getline(q[count].options[i], 100);
        }
        file >> q[count].correctAnswer;
        file.ignore(); // Skip newline after number
        count++;

        if (count >= maxQuestion) {
            cout << "Reached maximum number of questions (" << maxQuestion << ").\n";
            break;
        }
    }

    file.close();
}

void shuffleQuestions(Question q[], int count) {
    srand(time(0));
    for (int i = count - 1; i > 0; i--) { 
        int j = rand() % (i + 1);
        Question temp = q[i];
        q[i] = q[j];
        q[j] = temp;
    }
}

void writeScore(int score, int total) {
    ofstream out("score.txt");
    out << "Your final score : " << score << "/" << total << endl;
    out.close();
}

int main() {
    Question questions[maxQuestion];
    int totalQuestion;
    readQuestions(questions, totalQuestion);

    shuffleQuestions(questions, totalQuestion);

    ofstream ansFile("answers.txt"); //  Open file to save answers

    int score = 0;
    int questionsToAsk = (totalQuestion < 30) ? totalQuestion : 30;

    for (int i = 0; i < questionsToAsk; i++) {
        cout << "\nQ" << (i + 1) << ": " << questions[i].text << endl;
        for (int j = 0; j < OPTIONS; j++) {
            cout << j + 1 << ". " << questions[i].options[j] << endl;
        }

        int userAnswer;
        cout << "Your answer (1-4): ";
        cin >> userAnswer;

      
        while (userAnswer < 1 || userAnswer > 4) {
            cout << "Invalid choice. Please enter a number between 1 and 4: ";
            cin >> userAnswer;
        }

        ansFile << "Q" << (i+1) << ": " << questions[i].text 
                << "\nCorrect Answer: " << questions[i].correctAnswer
                << ". " << questions[i].options[questions[i].correctAnswer - 1]
                << "\n\n";

        if (userAnswer == questions[i].correctAnswer) {
            cout << "Correct!\n";
            score += 10;
        } else {
            cout << "Wrong! Correct answer was: " << questions[i].correctAnswer 
                 << ". " << questions[i].options[questions[i].correctAnswer - 1] << "\n";
        }
    }

    ansFile.close(); 

    cout << "\nTotal Score: " << score << " out of " << (questionsToAsk * 10) << endl;
    writeScore(score, questionsToAsk * 10);

    return 0;
}
