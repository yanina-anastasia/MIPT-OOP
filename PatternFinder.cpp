#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <chrono>
#include <queue>

using namespace std;

int counter = 0;

vector <int> prefixFunc(string text) {
	vector <int> pattern;
	pattern.resize(text.size());
	pattern[0] = 0;
	for (int i = 1; i < pattern.size(); ++i) {
		int position = pattern[i - 1];
		while (position > 0 && text[position] != text[i]) {
			position = pattern[position - 1];
		}
		pattern[i] = position + (text[position] == text[i] ? 1 : 0);
	}
	return pattern;
}

int kmp(string text, string pattern) {
  vector <int> z = prefixFunc(pattern);
  int position = 0;
  counter = 0;
  for (int i = 0; i < text.size(); ++i) {
	  while (position > 0 && (position >= pattern.size() || pattern[position] != text[i])) {
		  position = z[position - 1];
	  }
	  if (text[i] == pattern[position]) {
		  position++;
	  }
	  if (position == pattern.size()) {
		  counter++;
	  }
  }
  return counter;
}
 
void badCharHeuristic(string str, int size, int badchar[256]) {
	for (int i = 0; i < 256; i++) {
		badchar[i] = -1;
	}
    for (int i = 0; i < size; i++) {
		badchar[(int)str[i]] = i;
	}
}
 
int BM(string text, string pattern) {
	counter = 0;
    int badchar[256];
	badCharHeuristic(pattern, pattern.length(), badchar);
    int s = 0;
    while(s <= (text.length() - pattern.length())) {
        int j = pattern.length() - 1;
        while(j >= 0 && pattern[j] == text[s + j]) {
			j--;
		}
        if (j < 0) {
            counter++;
            s += (s+pattern.length() < text.length()) ? pattern.length() - badchar[text[s + pattern.length()]] : 1;
        } else {
            s += max(1, j - badchar[text[s + j]]);
        }
    }
	return counter;
}


int RK(string text, string pattern) {
	counter = 0;
	const int p = 31;
	int counter = 0;
	vector<long long> p_pow;
	p_pow.resize(max(pattern.length(), text.length()));
	p_pow[0] = 1;
	for (size_t i = 1; i < p_pow.size(); ++i)
		p_pow[i] = p_pow[i - 1] * p;
	
	vector<long long> h;
	h.resize(text.length());
	for (size_t i = 0; i < text.length(); ++i) {
		h[i] = (text[i] - 'a' + 1) * p_pow[i];
		if (i) {
			h[i] += h[i-1];
		}
	}
	long long h_s = 0;
	for (size_t i = 0; i < pattern.length(); ++i)
		h_s += (pattern[i] - 'a' + 1) * p_pow[i];
	for (size_t i = 0; i + pattern.length() - 1 < text.length(); ++i) {
		long long cur_h = h[i + pattern.length() - 1];
		if (i)  cur_h -= h[i - 1];
		if (cur_h == h_s * p_pow[i]) {
			counter++;
		}
	}
	return counter;
}

int SS(string text, string pattern) {
	counter = 0;
	string example;
	for (int i = 0; i < text.length() - pattern.length() + 1; ++i) {
		if (text.substr(i, pattern.length()) == pattern) {
			counter++;
		}
	}
	return counter;
}

class Trie
{
private:
        static const int alphabet_size = 256;
        struct Node 
        {
                Node * children[alphabet_size];
                Node * jumps[alphabet_size];
                Node * parent;
                Node * suff_link;
                Node * term_link;
                unsigned char char_to_parent;
                size_t terminal_count;
                size_t level;

                Node(Node * parent, unsigned char char_to_parent)
                {
                        memset(children, 0, sizeof(Node*) * alphabet_size);
                        memset(jumps, 0, sizeof(Node*) * alphabet_size);
                        suff_link = NULL;
                        this->parent = parent;
                        this->char_to_parent = char_to_parent;
                        terminal_count = 0;
                        term_link = NULL;
                }

                ~Node()
                {
                        for(size_t index = 0; index < alphabet_size; ++index)
                        {
                                if (children[index] != NULL)
                                        delete children[index];
                        }
                }
        };

        static unsigned char get_char_index(unsigned char letter)
        {
                return letter;
        }

        Node * root;

        Node * get_suff_link(Node * node)
        {
                if (!node->suff_link)
                {
                        if (node == root || node->parent == root)
                        {
                                node->suff_link = root;
                        }
                        else
                        {
                                node->suff_link = get_go(get_suff_link(node->parent), node->char_to_parent);
                        }
                }
                return node->suff_link;
        }

        Node * get_go(Node * node, unsigned char next_char)
        {
                if (!node->jumps[next_char])
                {
                        if (node->children[next_char])
                        {
                                node->jumps[next_char] = node->children[next_char];
                        }
                        else
                        {
                                node->jumps[next_char] = (node == root) ? root : get_go(get_suff_link(node), next_char);
                        }
                }
                return node->jumps[next_char];
        }

        size_t count_terminals(Node * node)
        {
                if (node == NULL)
                {
                        return 0;
                }

                return node->terminal_count + count_terminals(node->term_link);
        }

public:
        Trie()
        {
                root = new Node(NULL, 0);
        }

        void add_string(const std::string & st)
        {
                Node * current = root;
                for(auto it = st.begin(); it != st.end(); ++it)
                {
                        unsigned char char_index = get_char_index(*it);
                        if (current->children[char_index] == NULL)
                        {
                                current->children[char_index] = new Node(current, char_index);
                        }
                        current = current->children[char_index];
                }
                current->terminal_count += 1;
                current->level = st.size();
        }

        int process_text(const std::string & text) 
        {
                Node * current = root;
                size_t answer = 0;
                for(auto it = text.begin(); it != text.end(); ++it)
                {
                        unsigned char char_index = get_char_index(*it);
                        current = get_go(current, char_index);
                        answer += count_terminals(current);
                }
                return answer;
        }

        void calculate_links()
        {
                std::queue<Node *> queue;
                queue.push(root);
                while(!queue.empty())
                {
                        Node * current_node = queue.front();
                        queue.pop();

                        if (current_node == root || current_node->parent == root)
                        {
                                current_node->suff_link = root;
                                current_node->term_link = NULL;
                        }
                        else
                        {
                                current_node->suff_link = get_go(current_node->parent->suff_link, current_node->char_to_parent);
                                current_node->term_link = current_node->suff_link->terminal_count > 0 ? current_node->suff_link : current_node->suff_link->term_link;
                        }

                        for(size_t index = 0; index < alphabet_size; ++index)
                        {
                                Node * child = current_node->children[index];
                                if (child != NULL)
                                {
                                        queue.push(child);
                                }
                        }
                }
        }

        ~Trie()
        {
                delete root;
        }
};



int main() {
	ifstream ifs;
	string text;
	string pattern;
	ifs.open("D:\\input.txt");
	vector <string> texts;
	istringstream itext(pattern);
	while (getline(ifs, text)) {
		texts.push_back(text);
    }
	text.clear();
	for (int i = 0; i < texts.size(); ++i) {
		text += texts[i];
		text += " ";
	}
	transform(text.begin(), text.end(), text.begin(), ::tolower);
	ifs.close();
	ifs.open("D:\\str_input.txt");
	vector <string> patterns;
	istringstream iss(pattern);
	while (getline(ifs, pattern)) {
		if (pattern.size() == 0) {
			continue;
		}
		patterns.push_back(pattern);
    }
	ifs.close();
	for (int i = 0; i < patterns.size(); ++i) {
		transform(patterns[i].begin(), patterns[i].end(), patterns[i].begin(), ::tolower);
	}

	freopen("D:\\output.txt", "wt", stdout);
	int result = 0;
	auto t1 = chrono::high_resolution_clock::now();
	for (int i = 0; i < patterns.size(); ++i) {
		result += kmp(text, patterns[i]);
	}
	auto t2 = chrono::high_resolution_clock::now();
	auto KMPTime = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    cout << "KMP: " << result << " " << KMPTime << endl;
	auto bestTime = KMPTime;
	string bestSort = "KMP";

	result = 0;
	t1 = chrono::high_resolution_clock::now();
	for (int i = 0; i < patterns.size(); ++i) {
		result += BM(text, patterns[i]);
	}
	t2 = chrono::high_resolution_clock::now();
	auto BMTime = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
	cout << "BM: " << result << " " << BMTime << endl;
	if (BMTime < bestTime) {
		bestTime = BMTime;	
		bestSort = "BM";
	}

	result = 0;
	t1 = chrono::high_resolution_clock::now();
	for (int i = 0; i < patterns.size(); ++i) {
		result += RK(text, patterns[i]);
	}
	t2 = chrono::high_resolution_clock::now();
	auto RKTime = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
	cout << "RK: " << result << " " << RKTime << endl;
	if (RKTime < bestTime) {
		bestTime = RKTime;	
		bestSort = "RK";
	}

	result = 0;
	t1 = chrono::high_resolution_clock::now();
	for (int i = 0; i < patterns.size(); ++i) {
		result += SS(text, patterns[i]);
	}
	t2 = chrono::high_resolution_clock::now();
	auto SSTime = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
	cout << "SS: " << result << " " << SSTime << endl;
	if (SSTime < bestTime) {
		bestTime = SSTime;	
		bestSort = "SS";
	}

	Trie trie;
	for (int i = 0; i < patterns.size(); ++i) {
		trie.add_string(patterns[i]);
	}
	trie.calculate_links();
	result = 0;
	t1 = chrono::high_resolution_clock::now();
	result = trie.process_text(text);
	t2 = chrono::high_resolution_clock::now();
	auto AKTime = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
	cout << "AHO-KORASIK: " << result << " " << AKTime << endl;
	if (SSTime < bestTime) {
		bestTime = AKTime;	
		bestSort = "AK";
	}

	cout << "BEST: " << bestSort << " - " << bestTime << endl;
}
