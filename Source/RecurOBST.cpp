#include <iostream>
#include <vector>
#include <limits.h>

using namespace std;

struct Node {
    int key;
    Node *left, *right;
    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

vector<int> prefixSum;

// Recursively builds all possible binary search trees for keys[i..j] and calculates their costs.
// Returns a vector of pairs, where each pair contains the tree root and its associated cost.
vector<pair<Node*, int>> buildtree(vector<int> keys, vector<int> freq, int i, int j) {
    vector<pair<Node*, int>> tree;
    // Base case: if no keys are available, return a null tree with zero cost.
    if (i > j) {
        tree.push_back({nullptr, 0});
        return tree;
    }

    // Try every key in the range as the root.
    for (int r = i; r <= j; r++) {
        // Recursively build left and right subtrees.
        vector<pair<Node*, int>> tree_left = buildtree(keys, freq, i, r - 1);
        vector<pair<Node*, int>> tree_right = buildtree(keys, freq, r + 1, j);
        // Compute the sum of frequencies for the current interval [i, j].
        int sum = prefixSum[j] - ((i == 0) ? 0 : prefixSum[i - 1]);

        // Combine every left and right subtree with the current root.
        for (auto &left : tree_left)
            for (auto &right : tree_right) {
                Node *root = new Node(keys[r]);
                root->left = left.first;
                root->right = right.first;
                // Total cost = cost(left subtree) + cost(right subtree) + sum of frequencies for the current interval.
                int cost = left.second + right.second + sum;
                tree.push_back({root, cost});
            }
    }
    return tree;
}

// Post-order traversal: prints left subtree, then right subtree, then the node key.
void printTreeLRN(Node* root) {
    if (!root) return;
    printTreeLRN(root->left);
    printTreeLRN(root->right);
    cout << root->key << " ";
}

//free tree
void release(Node* root) {
	if (root == nullptr) return;
	release(root->left);
	release(root->right);
	delete root;
}

int main() {
    vector<int> keys = {10, 12, 20, 30, 40, 50};
    vector<int> freq = {34, 8, 50, 3, 7, 19};
    int n = keys.size();

    // Build prefix sum for frequency values for efficient range sum queries.
    prefixSum.resize(n, 0);
    for (int i = 0; i < n; i++) {
        prefixSum[i] = ((i == 0) ? freq[i] : (prefixSum[i - 1] + freq[i]));
    }

    // Build all possible trees with their associated costs.
    vector<pair<Node*, int>> trees = buildtree(keys, freq, 0, n - 1);
    int tree_index = 1;

    pair<Node*, int> ansTree; 
    ansTree.second = INT_MAX;
    // Print each tree and its cost.
    for (auto &t : trees) {
        cout << "Tree " << tree_index++ << " (Cost: " << t.second << "):\n";
        printTreeLRN(t.first);
        cout << "\n";
        if (ansTree.second > t.second) ansTree = t;
        release(t.first); // free tree
    }
    cout << "Minimum tree is cost: " << ansTree.second << "\n";
    printTreeLRN(ansTree.first);

    return 0;
}

