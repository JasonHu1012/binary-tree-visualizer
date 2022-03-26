#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>

using namespace std;

int const max_size = 1500;

class line
{
public:
    typedef enum
    {
        left,
        right
    }direction;
    direction dir;
    int len;
    bool exist;
    line(direction d, bool e): dir(d), len(1), exist(e) {}
    line() {}
};

class node
{
public:
    node *parent, *left, *right;
    static node *root;
    int val;
    int len;
    line parent_line;
    node(int v, node *l, node *r): parent(nullptr), left(l), right(r), val(v), len(to_string(v).size()) {}
    int connect()
    {
        return this->parent_line.dir == line::left ? (this->len + 1) / 2 : this->len / 2 - 1;
    }
    int pos()
    {
        if(this == root)
            return 0;
        return this->parent_line.dir == line::left ? this->parent->pos() - this->parent_line.len - this->connect() :
            this->parent->pos() + this->parent->len - 1 + this->parent_line.len - this->connect();
    }
};

node *node::root;

node *build_tree(vector<int> &in, vector<int> &pre, int in_start, int pre_start, int size, unordered_map<int, int> &index)
{
    if(size == 0)
        return nullptr;
    int left_size = index[pre[pre_start]] - in_start;
    node *output = new node(pre[pre_start], build_tree(in, pre, in_start, pre_start + 1, left_size, index),
        build_tree(in, pre, in_start + left_size + 1, pre_start + left_size + 1, size - left_size - 1, index));
    if(output->left)
        output->left->parent = output;
    if(output->right)
        output->right->parent = output;
    return output;
}

void kill_tree(node *root)
{
    if(root == nullptr)
        return;
    kill_tree(root->left);
    kill_tree(root->right);
    delete root;
}

void prolong(node *a, node *b, int l, vector<vector<line *> > lines, int i)
{
    while(a != b)
    {
        a = a->parent;
        b = b->parent;
        i--;
    }
    for(int j = 0; j < lines[i].size(); j++)
        lines[i][j]->len += l;
}

bool is_number(char c)
{
    if(c == '-' or (c >= '0' and c <= '9'))
        return true;
    return false;
}

void input(vector<int> &in, vector<int> &pre)
{
    string s;
    int start, end;
    cout << "In-order: ";
    getline(cin, s);
    if(s[0] == 'e' || s[0] == 'E')
        exit(0);
    start = 0;
    while(start < s.size() and is_number(s[start]))
    {
        end = start;
        while(end < s.size() and is_number(s[end]))
            end++;
        in.push_back(stoi(string(s.begin() + start, s.begin() + end)));
        start = end + 1;
    }
    cout << "Pre-order: ";
    getline(cin, s);
    start = 0;
    while(start < s.size() and is_number(s[start]))
    {
        end = start;
        while(end < s.size() and is_number(s[end]))
            end++;
        pre.push_back(stoi(string(s.begin() + start, s.begin() + end)));
        start = end + 1;
    }
}

void print(vector<vector<node *> > &nodes, vector<vector<line *> > &lines, int shift)
{
    vector<string> output(max_size, string(max_size, 0));
    int i, j, k, p, oi = 1, oj;
    char target;
    string val_string;
    val_string = to_string(nodes[0][0]->val);
    copy(val_string.begin(), val_string.end(), output[0].begin() + shift);
    for(p = shift - 1; p > -1 and output[0][p] == 0; p--)
        output[0][p] = ' ';
    output[0].resize(shift + nodes[0][0]->len);
    for(i = 0; i < lines.size(); i++)
    {
        oj = 0;
        j = 0;
        while(j < lines[i].size())
        {
            while(not is_number(output[oi - 1][oj]))
                oj++;
            if(lines[i][j]->exist)
            {
                for(k = 0; k < lines[i][j]->len; k++)
                {
                    output[oi + k][oj - 1 - k] = '/';
                    for(p = oj - 2 - k; p > -1 and output[oi + k][p] == 0; p--)
                        output[oi + k][p] = ' ';
                }
            }
            j++;
            while(is_number(output[oi - 1][oj]))
                oj++;
            if(lines[i][j]->exist)
            {
                for(k = 0; k < lines[i][j]->len; k++)
                {
                    output[oi + k][oj + k] = '\\';
                    for(p = oj + k - 1; p > -1 and output[oi + k][p] == 0; p--)
                        output[oi + k][p] = ' ';
                }
            }
            j++;
        }
        oi += lines[i][0]->len;
        oj = 0;
        i++;
        for(j = 0; j < nodes[i].size(); j++)
        {
            if(nodes[i][j]->parent_line.dir == line::left)
                target = '/';
            else
                target = '\\';
            while(output[oi - 1][oj + nodes[i][j]->connect()] != target)
                oj++;
            val_string = to_string(nodes[i][j]->val);
            copy(val_string.begin(), val_string.end(), output[oi].begin() + oj);
            for(p = oj - 1; p > -1 and output[oi][p] == 0; p--)
                output[oi][p] = ' ';
            oj += nodes[i][j]->len;
        }
        i--;
        oi++;
    }
    cout << endl;
    for(i = 0; i < oi; i++)
        cout << output[i] << endl;
    cout << endl;
}

int main()
{
    vector<int> in, pre;
    unordered_map<int, int> index;
    node *curr;
    int i, j, temp;
    vector<vector<node *> > nodes;
    vector<vector<line *> > lines;
    vector<line *> trash;
    queue<node *> q;
    bool finish;
    cout << "***************************************" << endl;
    cout << " Separate the integers with one space." << endl;
    cout << " The integers must be distinct." << endl;
    cout << " Press E to exit." << endl;
    cout << "***************************************" << endl << endl;
    while(true)
    {
        input(in, pre);
        for(i = 0; i < in.size(); i++)
            index[in[i]] = i;
        node::root = build_tree(in, pre, 0, 0, in.size(), index);
        q.push(node::root);
        while(not q.empty())
        {
            j = q.size();
            nodes.push_back(vector<node *>());
            lines.push_back(vector<line *>());
            for(i = 0; i < j; i++)
            {
                curr = q.front();
                q.pop();
                nodes.back().push_back(curr);
                if(curr->left)
                {
                    q.push(curr->left);
                    curr->left->parent_line = line(line::left, true);
                    lines.back().push_back(&curr->left->parent_line);
                }
                else
                {
                    trash.push_back(new line(line::left, false));
                    lines.back().push_back(trash.back());
                }
                if(curr->right)
                {
                    q.push(curr->right);
                    curr->right->parent_line = line(line::right, true);
                    lines.back().push_back(&curr->right->parent_line);
                }
                else
                {
                    trash.push_back(new line(line::right, false));
                    lines.back().push_back(trash.back());
                }
            }
        }
        lines.pop_back();
        finish = false;
        while(not finish)
        {
            finish = true;
            for(i = 1; i < nodes.size(); i++)
            {
                for(j = 0; j < nodes[i].size() - 1; j++)
                {
                    temp = nodes[i][j]->pos() + nodes[i][j]->len - nodes[i][j + 1]->pos();
                    if(temp >= 0)
                    {
                        prolong(nodes[i][j], nodes[i][j + 1], (temp + 2) / 2, lines, i);
                        finish = false;
                    }
                }
            }
        }
        temp = 0;
        for(i = 1; i < nodes.size(); i++)
            temp = min(temp, nodes[i][0]->pos());
        print(nodes, lines, -temp);
        for(i = 0; i < trash.size(); i++)
            delete trash[i];
        in.clear();
        pre.clear();
        index.clear();
        nodes.clear();
        lines.clear();
        trash.clear();
        kill_tree(node::root);
    }
    return 0;
}
