#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>

#define EMPTY_STRING ""

// A tree node.
struct Node
{
    char m_Character;
    int m_Frequency;
    Node* m_LeftNode;
    Node* m_RightNode;
};

// Function to allocate a new tree node.
Node* AllocateNode(char character, int frequency, Node* leftNode, Node* rightNode)
{
    Node* node = new Node();

    node->m_Character = character;
    node->m_Frequency = frequency;
    node->m_LeftNode = leftNode;
    node->m_RightNode = rightNode;

    return node; 
}

struct Comparison
{
    bool operator()(const Node* leftNode, const Node* rightNode) const
    {
        // The highest priority item has the lowest frequency.
        // A priority queue is ordered from the Highest Frequency to the Lowest Frequency. Hence, the highest priority item is the right.
        return leftNode->m_Frequency > rightNode->m_Frequency;
    }
};

// Utility function to check if Huffman Tree contains only a single node.
bool IsLeaf(Node* rootNode)
{
    return rootNode->m_LeftNode == nullptr && rootNode->m_RightNode == nullptr;
}

// Traverse the Huffman Tree and store Huffman Codes in a map.
void Encode(Node* root, std::string string, std::unordered_map<char, std::string>& huffmanCode)
{
    if (root == nullptr)
    {
        return;
    }

    // Found a leaf node.
    if (IsLeaf(root))
    {
        huffmanCode[root->m_Character] = (string != EMPTY_STRING) ? string : "1";
    }

    // Recursive.
    Encode(root->m_LeftNode, string + "0", huffmanCode);
    Encode(root->m_RightNode, string + "1", huffmanCode);
}

// Traverse the Huffman Tree and decode the decoded string.
void Decode(Node* rootNode, int& index, std::string string)
{

}

// Builds Huffman Tree and decodes the given input text.
void BuildHuffmanTree(std::string text)
{
    // Base Case:: Empty String
    if (text == EMPTY_STRING)
    {
        return;
    }

    // Count the frequency of appearence of each character and store it in a map.
    std::unordered_map<char, int> frequencyMap;
    for (char character : text)
    {
        frequencyMap[character]++;
    }

    // Create a priority queue to store live nodes of the Huffman Tree.
    // The comparison function will automatically arrange the queue linearly in terms of frequency from the Highest Frequency to the Lowest Frequency.
    std::priority_queue<Node*, std::vector<Node*>, Comparison> priorityQueue;

    // Create a leaf node for each character and add it to the priority queue.
    // Each node is empty and does not have any child nodes for now until we encode the code.
    for (auto pair : frequencyMap)
    {
        priorityQueue.push(AllocateNode(pair.first, pair.second, nullptr, nullptr));
    }

    // Do until there is 1 node left in the queue (our lowerest frequency node which is our root node).
    while (priorityQueue.size() != 1)
    {
        // Remove the two nodes of the highest priority (the lowest frequency) from the queue.
        Node* leftNode = priorityQueue.top();
        priorityQueue.pop();
        Node* rightNode = priorityQueue.top();
        priorityQueue.pop();

        // Create a new internal node with these two nodes as children and with a frequency equal to the sum of the two node's frequencies.
        // Add the new node to the priority queue.
        int frequencySum = leftNode->m_Frequency + rightNode->m_Frequency;
        // '0' equals 0. It is a numerical escape sequence.
        // push sorts the queue according to our compare() functor. Hence, the priority queue pushes the highest frequency to the front of the queue.
        priorityQueue.push(AllocateNode('\0', frequencySum, leftNode, rightNode));
    }

    // "Root" stores pointer to the root of the Huffman Tree, aka the node with the highest frequency.
    Node* rootNode = priorityQueue.top();

    // Traverse the Huffman Tree and stores Huffman Codes in a map.
    std::unordered_map<char, std::string> huffmanCode;
    Encode(rootNode, EMPTY_STRING, huffmanCode);

    std::cout << "Huffman Codes are: \n" << std::endl;
    for (auto pair : huffmanCode)
    {
        std::cout << pair.first << " " << pair.second << std::endl;
    }

    std::cout << "\n The original string is: \n" << text << std::endl;

    // Print encoded string.
    std::string string;
    for (char character : text)
    {
        string += huffmanCode[character];
    }

    std::cout << "\n The encoded string is: \n" << string << std::endl;
    std::cout << "\n The decoded string is: \n";

    if (IsLeaf(rootNode))
    {
        // Special case: For input like a, aa, aaa, etc. This means that our tree only has 1 node.
        while (rootNode->m_Frequency--)
        {
            std::cout << rootNode->m_Character;
        }
    }
    else
    {
        // Traverse the Huffman Tree again and this time, decode the encoded string.
        int index = -1;
        while (index < (int)string.size() - 1)
        {
            Decode(rootNode, index, string);
        }
    }
}