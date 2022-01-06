// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"
//#include "Math/UnrealMathUtility.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(HiddenWords);
    
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if (BeGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else //checking player guess
    {
        ProcessGuess(PlayerInput); 
    }    
}    
    

    
void UBullCowCartridge::SetupGame()
{
       //welcoming the player
    PrintLine (TEXT("Welcome to bulls and cows!"));
    
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() -1)];  
    Lives =  HiddenWord.Len();
    BeGameOver = false;
    
    PrintLine(TEXT("Can you guess the %i letter isogram?"), HiddenWord.Len()); 
    PrintLine(TEXT("The Hidden word is: %s."), *HiddenWord); // debug line
    PrintLine(TEXT("You have %i lives"), Lives);                
    PrintLine (TEXT("Type in your guess and press enter")); //prompt player  guess    
     
}


void UBullCowCartridge::EndGame()
{
    BeGameOver = true;
    
    PrintLine(TEXT("\nPress enter to play again"));
}


void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
     if (Guess == HiddenWord)
        { 
            PrintLine(TEXT("You have won!"));
            EndGame();
            return;
        }

    //check isogram
    if (!IsIsogram(Guess))
    {
        
        PrintLine(TEXT("no repeating letters, Guess again!"));
        return;
    }
    
    
    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The hidden word is %i letters long"),HiddenWord.Len());
        PrintLine(TEXT("Sorry try again you have %i lives remaining"),Lives);
        
        return;
    }
    //remove life 
    PrintLine(TEXT("You lost a life!"));
    --Lives;
    
    //checklives >0
    if (Lives <= 0)
    {   
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("The hidden word was: %s"),*HiddenWord);
        EndGame();
        return;
    }
            
    //show player the bulls and the cows
    
    FBullCowCount Score = GetBullCows(Guess); 

    PrintLine(TEXT("You have %i Bulls and %i Cows"),Score.Bulls, Score.Cows);
    
    PrintLine(TEXT("Guess again, you have %i lives left"),Lives);         
}


bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index +1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;

}
TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
   TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8  && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);    
        }
    }
    return ValidWords; 
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
        
        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            } 
        }
    }
    return Count;
}