#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"

const int PASS_LEN = 20;        // Maximum any password will be
const int HASH_LEN = 33;        // Length of MD5 hash strings


// Given a target plaintext word, use it to try to find
// a matching hash in the hashFile.
// Get this function working first!
char * tryWord(char * plaintext, char * hashFilename)
{
    // Loop to find length of plaintext
    int i = 0;
    while(*(plaintext + i) != '\0' && *(plaintext + i) != '\n') i++;

    // then remove potential newline character and hash it
    if(*(plaintext + i) == '\n') *(plaintext + i) = '\0';
    char * testHash = md5(plaintext, i);

    // Open the hash file
    FILE *hashed = fopen(hashFilename, "r");

    // Set up string to contain a hash, a return string, and boolean
    char * listHash = malloc(HASH_LEN);
    char * output = malloc(HASH_LEN);
    int found = 0;

    // Loop through the hash file, one line at a time.
    while(fgets(listHash, 255, hashed) != NULL)
    {
        // remove newline character from hash
        int j = 0;
        while(*(listHash + j) != '\n' && j < HASH_LEN) j++;
        *(listHash + j) = '\0';

        // Attempt to match the hash from the file to the
        // hash of the plaintext.

        // If there is a match, you'll return the hash.
        // If not, return NULL.

        if(strcmp(listHash, testHash) == 0) 
        {
            strcpy(output, listHash);
            found = 1;
            break;
        }
    }

    // makes sure if no hashes matched, output is NULL
    if(strcmp(listHash, testHash) != 0) return NULL;

    // Before returning, do any needed cleanup:
    //   Close files?
    fclose(hashed);
    //   Free memory?
    free(listHash);

    // Modify this line so it returns the hash
    // that was found, or NULL if not found.
    if(found)
        return output;
    else
        return NULL;
}


int main(int argc, char *argv[])
{
    // set up variables
    char * word = malloc(PASS_LEN);
    char * hash = malloc(HASH_LEN);
    int numCracked = 0;

    if (argc < 3) 
    {
        fprintf(stderr, "Usage: %s hash_file dict_file\n", argv[0]);
        exit(1);
    }

    // Open the dictionary file for reading.
    FILE * dictionary = fopen(argv[2], "r");

    // For each dictionary word, pass it to tryWord, which
    // will attempt to match it against the hashes in the hash_file.
    while(fgets(word, PASS_LEN, dictionary) != NULL)
    {
        hash = tryWord(word, argv[1]);
        // If we got a match, display the hash and the word. For example:
        //   5d41402abc4b2a76b9719d911017c592 hello
        if(hash != NULL)
        {
            printf("%s %s\n", hash, word);
            numCracked++;
        }
    }
    // Close the dictionary file.
    fclose(dictionary);

    // Display the number of hashes that were cracked.
    printf("%d hashes cracked!\n", numCracked);
    
    // Free up any malloc'd memory?
    free(word);
    free(hash);
}

