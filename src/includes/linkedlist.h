template <class T>
class LLNode {
    public:
        LLNode();
        LLNode(T *value);
        T *payload;
        LLNode* next;
        LLNode* prev;
        int isTail();
        int isHead();
        T* get();
};

template <class T>
class LinkedList {
    public:
        LinkedList();
        int size;
        LLNode<T> *head;
        LLNode<T> *tail;
        T* pop();
        void add(T* value);
        T* remove(int i);
        T* remove(LLNode<T> *ll_node);
        void removeAndDestroy(int i);
        void removeAndDestroy(LLNode<T> *node);
        void flushAfterNode(LLNode<T> *node);
};