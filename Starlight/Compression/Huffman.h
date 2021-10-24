#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <unordered_map> 

#define EMPTY_STRING ""

// A Tree Node
struct Node
{
    char m_Character;
    int m_Frequency;
    Node* m_LeftNode;
    Node* m_RightNode;
};

// Function to allocate a new tree node.
Node* GetNode(char character, int frequency, Node* leftNode, Node* rightNode)
{
    Node* node = new Node();

    node->m_Character = character;
    node->m_Frequency = frequency;
    node->m_LeftNode = leftNode;
    node->m_RightNode = rightNode;

    return node;
}

// Utility function to check if the Huffman tree contains only a single node.
bool IsLeaf(Node* treeNode)
{
    return treeNode->m_LeftNode == nullptr && treeNode->m_RightNode == nullptr;
}

// Traverse the Huffman Tree and stores Huffman Codes in a map.
void Encode(Node* rootNode, std::string string, std::unordered_map<char, std::string>& huffmanCode)
{
    if (rootNode == nullptr)
    {
        return;
    }

    // Found a leaf node.
    if (IsLeaf(rootNode))
    {
        huffmanCode[rootNode->m_Character] = (string != EMPTY_STRING) ? string : "1";
    }

    Encode(rootNode->m_LeftNode, string + "0", huffmanCode);
    Encode(rootNode->m_RightNode, string + "1", huffmanCode);
}

// Traverse the Huffman Tree and decode the encoded string.
void Decode(Node* rootNode, int& index, std::string string)
{
    if (rootNode == nullptr)
    {
        return;
    }

    // Found a leaf node.
    if (IsLeaf(rootNode))
    {
        std::cout << rootNode->m_Character;
        return;
    }

    index++;

    if (string[index] == '0')
    {
        Decode(rootNode->m_LeftNode, index, string);
    }
    else
    {
        Decode(rootNode->m_RightNode, index, string);
    }
}

// Comparison object used to order the heap.
struct Compare
{
    bool operator()(const Node* leftNode, const Node* rightNode) const
    {
        // The highest priority item has the lowest frequency.
        return leftNode->m_Frequency > rightNode->m_Frequency;
    }
};

// Builds a Huffman Tree and decodes the given input text.
void BuildHuffmanTree(std::string& text)
{
    // Base case: empty string.
    if (text == EMPTY_STRING)
    {
        return;
    }

    // Count the frequency of appearence of each character and store it in a frequency map.
    std::unordered_map<char, int> frequencyMap;
    for (char character : text)
    {
        frequencyMap[character]++;
    }

    // Create a priority queue to store live nodes of the Huffman Tree.
    std::priority_queue<Node*, std::vector<Node*>, Compare> priorityQueue;

    // Create a leaf node for each character and add it.
    for (auto pair : frequencyMap)
    {
        priorityQueue.push(GetNode(pair.first, pair.second, nullptr, nullptr));
    }

    // Carry on until there is more than one node in the queue.
    while (priorityQueue.size() != 1)
    {
        // Remove the two nodes of the highest priority (the lowest frequency from the queue).
        Node* leftNode = priorityQueue.top();
        priorityQueue.pop();

        Node* rightNode = priorityQueue.top();
        priorityQueue.pop();

        // Create a new internal node with these two nodes as children and with a frequency equal to the sum of the two node's frequencies.
        // Add this new node to the priority queue.
        int sum = leftNode->m_Frequency + rightNode->m_Frequency;
        priorityQueue.push(GetNode('\0', sum, leftNode, rightNode));
    }

    // "Root" stores a pointer to the root of the Huffman Tree.
    Node* rootNode = priorityQueue.top();

    // Traverse the Huffman Tree and stores Huffman Codes in a map. Also prints them.
    std::unordered_map<char, std::string> huffmanCode;
    Encode(rootNode, EMPTY_STRING, huffmanCode);

    std::cout << "Huffman Codes are: \n\n";
    for (auto pair : huffmanCode)
    {
        std::cout << pair.first << " " << pair.second << "\n";
    }

    std::cout << "\nThe original string is: \n" << text << "\n";

    // Print encoded string.
    std::string string;
    for (char character : text)
    {
        string += huffmanCode[character];
    }

    std::cout << "\nThe encoded string is:\n" << string << "\n";
    std::cout << "\nThe decoded string is: \n";

    if (IsLeaf(rootNode))
    {
        // Special case, for input such as a, aa, aaa, etc.
        while (rootNode->m_Frequency--)
        {
            std::cout << rootNode->m_Character;
        }
    }
    else
    {
        // Traverse the tree again, this time decoding the encoded string.
        int index = -1;
        while (index < (int)string.size() - 1)
        {
            Decode(rootNode, index, string);
        }
    }
}


// Test Case
void TestCompression(std::string& string)
{
    BuildHuffmanTree(string);
}
 