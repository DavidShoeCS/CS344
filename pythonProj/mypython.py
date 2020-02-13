import sys
import random


randomCharacters = "abcdefghijklmnopqrstuvwxyz" #initialize what characters we can choose
randomChar = random.choice(randomCharacters); #choose a random char from character array


#a function to print what is in a list without commas and brackets messing up everything
def convertToString(string):
    newSTR = ""
    for i in string:
        newSTR+=i
    return newSTR

def createTwoNumsToMultiply():
    num1 = random.randint(1,42)
    num2 = random.randint(1,42)
    print(num1)
    print(num2)
    return (num1*num2)


def main():
    charArr =[] #initialize empty array

    f = open("fileOne.txt", "w+")

    #run through and pick 10 characters at random
    for i in range(0,10):
        randomChar = random.choice(randomCharacters);
        charArr.append(randomChar)

    print(convertToString(charArr))
    f.write(convertToString(charArr)) #write the random string to the file
    f.write("\n")
    f.close() #close fileOne

    charArr = [] #re-initialize character array to empty so we can use it again

    f = open("fileTwo.txt", "w+")
    #pick 10 characters at random again
    for i in range(0,10):
        randomChar = random.choice(randomCharacters);
        charArr.append(randomChar)

    print(convertToString(charArr))
    f.write(convertToString(charArr)) #write the random string to the file
    f.write("\n")
    f.close() #close fileTwo


    f = open("fileThree.txt", "w+")

    charArr = [] #re-initialize character array to empty so we can use it again
    #pick 10 characters at random again
    for i in range(0,10):
        randomChar = random.choice(randomCharacters);
        charArr.append(randomChar)

    print(convertToString(charArr))
    f.write(convertToString(charArr)) #write the random string to the file
    f.write("\n")
    f.close() #close fileThree


    print(createTwoNumsToMultiply()) #make 2 random numbers, multiply them


if __name__ == '__main__':
    main()
