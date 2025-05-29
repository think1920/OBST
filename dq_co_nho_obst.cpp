#include <iostream>
#include <vector>
#include <cstring>
#include <limits.h>

using namespace std;

vector<int> prefixSum; // Global prefix sum array for frequency sums

struct Node {
    int key;
    Node *left, *right;
    Node (int k) : key(k), left(nullptr), right(nullptr) {}
};

// Computes minimum cost for an optimal binary search tree using DP.
// Also fills the 'root' matrix with the optimal root indices.
int cost(vector<int> freq, vector<vector<int>> &root, vector<vector<int>>& check, int i, int j) {
    // Base case: if the range is empty, cost is 0
    if (j < i) return 0;
    // Base case: if there is only one node, cost is its frequency
    if (j == i) return freq[i];
    // If this subproblem has already been solved, return the stored result
    if (check[i][j] != -1) return check[i][j];
    int val = INT_MAX;
    // Calculate the sum of frequencies from i to j
    // Assumes prefixSum is a globally defined array
    int fsum = prefixSum[j] - ((i == 0) ? 0 : prefixSum[i - 1]);
    for (int r = i; r <= j; r++) {
        // Recursively calculate the cost of left and right subtrees
        int c = cost(freq, root, check, i, r - 1) + cost(freq, root, check, r + 1, j);
        if (c < val) {
            val = c;
            root[i][j] = r;
        }
    }
    // Store and return the minimum cost including the frequency sum
    return check[i][j] = fsum + val;
}

// Reconstructs the optimal binary search tree using the root matrix.
Node *trace_tree(vector<vector<int>> root, vector<int> keys, int i, int j) {
    if (i > j) return nullptr;
    int r = root[i][j];
    Node* new_node = new Node(keys[r]);
    // Recursively build the left subtree with keys from i to r - 1
    new_node->left = trace_tree(root, keys, i, r - 1);
     // Recursively build the right subtree with keys from r + 1 to j
    new_node->right = trace_tree(root, keys, r + 1, j);
    return new_node;
}

// Prints the tree in post-order (Left, Right, Node)
void printTreeLRN(Node* root) {
    if (!root) return;
    printTreeLRN(root->left);
    printTreeLRN(root->right);
    cout << root->key << " ";
}

// free tree
void release(Node* root) {
	if (root == nullptr) return;
	release(root->left);
	release(root->right);
	delete root;
}

int main() {
    vector<int> keys = {10, 12, 20, 30, 40, 50};
    vector<int> freq = {34, 8, 50, 10, 7, 9};
    int n = keys.size();
    vector<vector<int>> root(n, vector<int>(n, -1));
    vector<vector<int>> check(n, vector<int>(n, -1));

    // Build prefix sum for efficient frequency summation
    prefixSum.resize(n, 0);
    for (int i = 0; i < n; i++)
        prefixSum[i] = ((i == 0) ? freq[i] : prefixSum[i - 1] + freq[i]);
    
    // in base case, there is one node
    for (int i = 0; i < n; i++)
        root[i][i] = i;
    cout << "Minimum cost: " << cost(freq, root, check, 0, n - 1) << "\n";
    cout << "Optimal BST (post-order traversal):\n";

    // trace tree from the root matrix
    Node* tree = trace_tree(root, keys, 0, n - 1);
    printTreeLRN(tree);
    release(tree); // free tree
    return 0;
}
