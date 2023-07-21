#include<bits/stdc++.h>

using namespace std;

struct HuffmanNode {
    char data;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char data, int frequency) {
        this->data = data;
        this->frequency = frequency;
        left = right = nullptr;
    }
};

struct Compare {
    bool operator()(HuffmanNode* lhs, HuffmanNode* rhs) {
        return lhs->frequency > rhs->frequency;
    }
};

unordered_map<char, string> huffmanCodes;

void generateHuffmanCodes(HuffmanNode* root, string code) {
    if (root == nullptr) {
        return;
    }

    if (root->data != '\0') {
        huffmanCodes[root->data] = code;
    }

    generateHuffmanCodes(root->left, code + "0");
    generateHuffmanCodes(root->right, code + "1");
}

HuffmanNode* buildHuffmanTree(const unordered_map<char, int>& frequencyTable) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> minHeap;

    for (const auto& entry : frequencyTable) {
        minHeap.push(new HuffmanNode(entry.first, entry.second));
    }

    while (minHeap.size() > 1) {
        HuffmanNode* left = minHeap.top();
        minHeap.pop();
        HuffmanNode* right = minHeap.top();
        minHeap.pop();

        HuffmanNode* parent = new HuffmanNode('\0', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;

        minHeap.push(parent);
    }

    return minHeap.top();
}

unordered_map<char, int> countFrequency(const string& text) {
    unordered_map<char, int> frequencyTable;
    for (char c : text) {
        frequencyTable[c]++;
    }
    return frequencyTable;
}

string encodeText(const string& text) {
    string encodedText;
    for (char c : text) {
        encodedText += huffmanCodes[c];
    }
    return encodedText;
}

void writeCompressedFile(const string& inputFile, const string& compressedFile) {
    ifstream fin(inputFile, ios::binary);
    ofstream fout(compressedFile, ios::binary | ios::app);

    char c;
    string encodedText;
    while (fin.get(c)) {
        encodedText += huffmanCodes[c];
        while (encodedText.length() >= 8) {
            string byte = encodedText.substr(0, 8);
            fout.put(stoi(byte, nullptr, 2));
            encodedText = encodedText.substr(8);
        }
    }

    // Add remaining bits
    int padding = 8 - encodedText.length();
    encodedText.append(padding, '0');
    fout.put(stoi(encodedText, nullptr, 2));

    fin.close();
    fout.close();
}

int main() {
    string inputFile = "c:\\Users\\Rohit Yadav\\Desktop\\huffman.txt"; // Replace with the path to the input file
    string compressedFile = "c:\\Users\\Rohit Yadav\\Desktop\\huff_out.txt"; // Output compressed file

    ifstream fin(inputFile);
    if (!fin) {
        cerr << "Error opening input file." << endl;
        return 1;
    }

    // Step 1: Read input text from the file
    string inputText((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
    fin.close();

    // Step 2: Count character frequency in the input text
    unordered_map<char, int> frequencyTable = countFrequency(inputText);

    // Step 3: Build the Huffman tree
    HuffmanNode* root = buildHuffmanTree(frequencyTable);

    // Step 4: Generate Huffman codes
    generateHuffmanCodes(root, "");

    // Step 5: Encode the input text using Huffman codes
    string encodedText = encodeText(inputText);

    // Step 6: Write the compressed output to a file
    writeCompressedFile(inputFile, compressedFile);

    cout << "File compression successful." << endl;

    return 0;
}

/*This code compresses the contents of the "input.txt" file using Huffman coding and writes the compressed data
 to the "compressed.bin" file. 
To use this code, replace the file paths with the actual paths to your input file and specify the
desired output file. Please note that this implementation is a simplified version for educational
purposes and may not handle special cases or large files efficiently. 
For practical use, additional error handling and optimizations are recommended.*/