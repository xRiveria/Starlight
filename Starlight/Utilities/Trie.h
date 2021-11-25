#pragma once
#include <iostream>

// Define the character size.
#define CHAR_SIZE 128

// Data structure to store a Trie node.
class Trie
{
public:
    // Constructor
    Trie()
    {
        this->m_IsLeaf = false;

        for (int i = 0; i < CHAR_SIZE; i++)
        {
            this->m_Character[i] = nullptr;
        }
    }

    void Insert(std::string);
    bool Delete(Trie*&, std::string);
    bool Search(std::string);
    bool HaveChildren(Trie const*);

public:
    bool m_IsLeaf;
    Trie* m_Character[CHAR_SIZE];
};

// Iterative function to insert a Key into a Trie.
void Trie::Insert(std::string key)
{
    // Start from the root node.
    Trie* currentNode = this;
    for (int i = 0; i < key.length(); i++)
    {
        // Create a new node if the path doesn't exist.
        if (currentNode->m_Character[key[i]] == nullptr)
        {
            currentNode->m_Character[key[i]] = new Trie();
        }

        // Go to the next node.
        currentNode = currentNode->m_Character[key[i]];
    }

    // Once we've reached the end of our string, ask the current node as a leaf.
    currentNode->m_IsLeaf = true;
}

// Iterative function to search a key in a Trie. It returns true if they key is found in the Trie, otherwise, it returns false.
bool Trie::Search(std::string key)
{
    // Return false if the Trie is empty.
    if (this == nullptr)
    {
        return false;
    }

    Trie* currentNode = this;
    for (int i = 0; i < key.length(); i++)
    {
        // Go to the next node.
        currentNode = currentNode->m_Character[key[i]];

        // If the string is invalid (reached end of a path in the Trie).
        if (currentNode == nullptr)
        {
            return false;
        }
    }

    // Return true if the current node is a leaf and the end of the string is reached.
    return currentNode->m_IsLeaf;
}

// Returns true if a given node has any children.
bool Trie::HaveChildren(Trie const* currentNode)
{
    for (int i = 0; i < CHAR_SIZE; i++)
    {
        if (currentNode->m_Character[i])
        {
            return true; // Child found.
        }
    }

    return false;
}

// Recursive function to delete a key in the Trie.
bool Trie::Delete(Trie*& currentNode, std::string key)
{
    // Return if True is empty.
    if (currentNode == nullptr)
    {
        return false;
    }

    // If the end of the key is not reached...
    if (key.length())
    {
        // Recur for the node corresponding to the next character in the key and if it returns true, delete the current node (if it is non-leaf).
        if (currentNode != nullptr && currentNode->m_Character[key[0]] != nullptr
            && Delete(currentNode->m_Character[key[0]], key.substr(1)) && currentNode->m_IsLeaf == false)
        {
            // Only delete if node does not have children, as it would mean its not being used for another word.
            if (!HaveChildren(currentNode))
            {
                delete currentNode;
                currentNode = nullptr;
                return true;
            }
            else // We are being used!
            {
                return false;
            }
        }
    }

    // If the end of the key is reached, meaning that we have found a leaf.
    if (key.length() == 0 && currentNode->m_IsLeaf)
    {
        // If the current node is a leaf node and doesn't have any children.
        if (!HaveChildren(currentNode))
        {
            // Delete the current node.
            delete currentNode;
            currentNode = nullptr;

            // Delete the non-leaf parent nodes.
            return true;
        }
        else // If the current node is a leaf node and has children.
        {
            // Mark the current node as a non-leaf node (don't delete it!)
            currentNode->m_IsLeaf = false;

            // Do not delete its parent nodes.
            return false;
        }
    }

    return false;
}

int FindSuperDigit(long long int digit)
{
    if (digit <= 9)
    {
        return digit;
    }

    long long int sumDigit = 0;
    long long int m = 0;

    while (digit != 0)
    {
        m = digit % 10;
        sumDigit = sumDigit + m;
        digit = digit / 10;
    }

    return FindSuperDigit(sumDigit);
}

void TestTrie()
{
    long long int digit = 2510;
    std::cout << FindSuperDigit(digit) << "\n";
    

    std::string testCase = "Zealous";

    for (int i = 0; i < testCase.length(); i++)
    {
        int characterTest = testCase[i] - 'a';
        std::cout << characterTest << "\n";
    }

    Trie* head = new Trie();

    head->Insert("hello");
    std::cout << head->Search("hello") << " " << "\n"; // Print 1
    
    head->Delete(head, "hello");
    
    if (head == nullptr)
    {
        std::cout << "Trie empty!!\n"; // Trie is empty now.
    }
}