#ifndef __DOUBLY_LIST__
#define __DOUBLY_LIST__

struct charNode {
    charNode *prev;
    charNode *next;
    int value;

    charNode() {
        prev = next = nullptr;
    }

    charNode(int value) {
        prev = next = nullptr;
        this->value = value;
    }
};

struct lineNode {
    lineNode *prev;
    lineNode *next;
    charNode *head;
    charNode *tail;
    int length;

    lineNode() {
        prev = next = nullptr;
        head = tail = nullptr;
        length = 0;
    }
};

class DoublyList2D {
public:
    DoublyList2D();
    ~DoublyList2D();

    void insert_row();
    void delete_row();
    void insert_col(int x);
    void delete_col();

    int get_row_size();
    lineNode* get_first_row();

private:
    lineNode *document = nullptr;
    lineNode *current_row = nullptr;
    charNode *current_col = nullptr;
};

#endif /* __DOUBLY_LIST__  */
