#pragma once

// Implementation of max-heap and min-heap data structures.

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

// push() and pop() takes O(log(N)) time.
// peek() and size() and isEmpty() takes O(1) time.

// Data structure to store a max-heap node.
struct PriorityQueue
{
public:
    // Return size of our heap.
    unsigned int size() { return m_Elements.size(); }

    // Function to check if the heap is empty or not.
    bool empty() { return size() == 0; }

    // Insert key into the heap.
    void push(int key)
    {
        // Insert a new element at the end of the vector.
        m_Elements.push_back(key);

        // Get element index and call heapify_up procedure.
        int index = size() - 1;
        HeapifyUp(index);
    }

    // Function to remove an element with the highest priority (presently at the root).
    void pop()
    {
        try
        {
            // If the heap has no elements, throw an exception.
            if (size() == 0)
            {
                throw std::out_of_range("Vector<X>::at(): Index is out of range (Heap Overflow)");
            }

            // Replace the root of the heap with the last element.
            m_Elements[0] = m_Elements.back();
            m_Elements.pop_back();

            // Call heapify down on the root node.
            HeapifyDown(0);
        }
        catch (const std::out_of_range& error)
        {
            std::cout << std::endl << error.what();
        }
    }

    // Function to return an element with the highest priority (present at the root).
    int top()
    {
        try
        {
            if (size() == 0)
            {
                throw std::out_of_range("Vector<X>::at(): Index is out of range (Heap Overflow)");
            }

            // Otherwise, return the top (first) element.
            return m_Elements.at(0); // Or return m_Elements[0].
        }
        catch (const std::out_of_range& error)
        {
            // Catch and print the exception.
            std::cout << std::endl << error.what();
        }
    }
    
private:
    // Return parent of m_Elements[i]. Do not call this function if 'i' is already the root node.
    int GetParent(int i) { return (i - 1) / 2; }

    // Returns left child of A[i].
    int GetLeftChild(int i) { return (2 * i + 1); }

    // Returns right child of A[i]/
    int GetRightChild(int i) { return (2 * i + 2); }

    // Recursive heapify_down algorithm. The node at index 'i' and its two direct children violates the heap propery.
    void HeapifyDown(int i)
    {
        // Get left and right child of node at index 'i'.
        int leftChildIndex = GetLeftChild(i);
        int rightChildIndex = GetRightChild(i);

        int largest = i;

        // Compare m_Elements[i] with its left and right child and find their largest value.
        if (leftChildIndex < size() && m_Elements[leftChildIndex] > m_Elements[i])
        {
            largest = leftChildIndex;
        }

        if (rightChildIndex < size() && m_Elements[rightChildIndex] > m_Elements[largest])
        {
            largest = rightChildIndex;
        }

        // Swap with a child having greater value and call heapify_down again on the child.
        if (largest != i)
        {
            std::swap(m_Elements[i], m_Elements[largest]);
            HeapifyDown(largest);
        }
    }

    // Recursive heapify-up algorithm.
    void HeapifyUp(int i)
    {
        // Check if the node at index 'i' and its parent violate the heap property.
        // Remember that for max heaps, elements are sorted in a descending fashion.
        if (i && m_Elements[GetParent(i)] < m_Elements[i])
        {
            // Swap the two if heap property is violated.
            std::swap(m_Elements[i], m_Elements[GetParent(i)]);

            // Call heapify-up on the parent.
            HeapifyUp(GetParent(i));
        }
    }

private:
    // Vector to store heap elements,.
    std::vector<int> m_Elements;
};

void TestMaxHeap()
{
    PriorityQueue priorityQueue;

    // Note: The element's value decides priority.
    priorityQueue.push(3);
    priorityQueue.push(2);
    priorityQueue.push(15);

    std::cout << "Size is " << priorityQueue.size() << std::endl;
    std::cout << priorityQueue.top() << " ";
    priorityQueue.pop();

    std::cout << priorityQueue.top() << " ";
    priorityQueue.pop();

    priorityQueue.push(5);
    priorityQueue.push(4);
    priorityQueue.push(45);

    std::cout << std::endl << "Size is " << priorityQueue.size() << std::endl;

    std::cout << priorityQueue.top() << " ";
    priorityQueue.pop();

    std::cout << priorityQueue.top() << " ";
    priorityQueue.pop();

    std::cout << priorityQueue.top() << " ";
    priorityQueue.pop();

    std::cout << priorityQueue.top() << " ";
    priorityQueue.pop();

    std::cout << std::endl << std::boolalpha << priorityQueue.empty();

    priorityQueue.pop(); // Top operation on an empty heap.
    priorityQueue.pop(); // Top operation on an empty heap.
}