#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

vector<int> prefixSum; // Global prefix sum array for frequency sums

struct Node {
    int key;
    Node *left, *right;
    Node (int k) : key(k), left(nullptr), right(nullptr) {}
};

// Computes minimum cost for an optimal binary search tree using DP.
// Also fills the 'root' matrix with the optimal root indices.
int cost(vector<int> freq, vector<vector<int>> &root) {
    int n = freq.size();
    // dp[i][j] will hold the minimum cost for keys i to j
    int dp[n + 1][n + 1];
    memset(dp, sizeof(dp), 0);
    for (int i = 0; i < n; i++) {
        dp[i][i] = freq[i]; // Base case: cost of a single key tree
        root[i][i] = i;
    }
    for (int l = 2; l <= n; l++) {
        for (int i = 0; i <= n - l; i++) {
            int j = i + l - 1;
            dp[i][j] = 1e9 + 7;
            int sum = prefixSum[j] - ((i == 0) ? 0 : prefixSum[i - 1]); // Total frequency for interval [i, j]
            for (int r = i; r <= j; r++) {
                int val = ((r > i) ? dp[i][r - 1] : 0) + ((j > r) ? dp[r + 1][j] : 0) + sum;
                if (val < dp[i][j]) {
                    dp[i][j] = val;
                    root[i][j] = r; // Record the optimal root for this interval
                }
            }
        }
    }
    return dp[0][n - 1];
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

    // Build prefix sum for efficient frequency summation
    prefixSum.resize(n, 0);
    for (int i = 0; i < n; i++)
        prefixSum[i] = ((i == 0) ? freq[i] : prefixSum[i - 1] + freq[i]);

    cout << "Minimum cost: " << cost(freq, root) << "\n";
    cout << "Optimal BST (post-order traversal):\n";

    Node* tree = trace_tree(root, keys, 0, n - 1);
    printTreeLRN(tree);
    // free tree
    release(tree);
    return 0;
}
