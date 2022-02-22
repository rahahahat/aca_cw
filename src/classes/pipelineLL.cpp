#include "pipeline.h"

PipelineLLNode::PipelineLLNode()
{
    payload = new Instructions::Instruction();
    next = NULL;
    prev = NULL;
};

PipelineLLNode::PipelineLLNode(Instructions::Instruction *instrPtr)
{
    payload = instrPtr;
    next = NULL;
    prev = NULL;
}

int PipelineLLNode::isTail()
{
    return next == NULL;
}

int PipelineLLNode::isHead()
{
    return prev == NULL;
}

PipelineLL::PipelineLL()
{
    head = NULL;
    tail = NULL;
    size = 0;
}

Instructions::Instruction* PipelineLLNode::get()
{
    return payload;
}

Instructions::Instruction* PipelineLL::addInstructionForFetch()
{
    PipelineLLNode *pl_node = new PipelineLLNode();
    if (head == NULL && size == 0)
    {
        head = pl_node;
        tail = pl_node;
        size++;
        return pl_node->payload;
    }
    tail->next = pl_node;
    pl_node->prev = tail;
    tail = pl_node;
    pl_node->next = NULL;
    size++;
    return pl_node->payload;
};

void PipelineLL::add(Instructions::Instruction *instrPtr)
{
    PipelineLLNode *pl_node = new PipelineLLNode(instrPtr);
    if (head == NULL && size == 0)
    {
        head = pl_node;
        tail = pl_node;
        size++;
        return;
    }
    tail->next = pl_node;
    pl_node->prev = tail;
    pl_node->next = NULL;
    tail = pl_node;
    size++;
    return;
};

Instructions::Instruction* PipelineLL::pop()
{
    if (size == 0) return NULL;
    PipelineLLNode* temp = head->next;
    Instructions::Instruction *temp_instr = head->payload;
    head->next = NULL;
    head->prev = NULL;
    head->payload = NULL;
    if (temp != NULL)
    {
        temp->prev = NULL;
    }
    delete head;
    head = temp;
    size--;
    if (size <= 1)
    {
        tail = head;
    }
    return temp_instr;
}

Instructions::Instruction* PipelineLL::remove(PipelineLLNode* pl_node)
{
    if (pl_node == NULL)
    {
        std::cout << "PipelineLLNode is NULL" << std::endl;
        return NULL;
    }
    PipelineLLNode* prev = pl_node->next;
    PipelineLLNode* next = pl_node->prev;
    Instructions::Instruction* ret_instr;
    if (prev != NULL)
    {
        prev->next = next;
    }
    if (next != NULL) {
        next->prev = prev;
    }
    if (pl_node == tail)
    {
        tail = prev;
    }
    if (pl_node == head)
    {
        head = next;
    }
    ret_instr = pl_node->payload;
    pl_node->prev = NULL;
    pl_node->next = NULL;
    pl_node->payload = NULL;
    size--;
    delete pl_node;
    return ret_instr;
}

Instructions::Instruction* PipelineLL::remove(int i)
{
    if (i >= size)
    {
        std::cout << "PipelineLL index out of range" << std::endl;
        return NULL;
    }
    int count = 0;
    PipelineLLNode* prev;
    PipelineLLNode* next;
    Instructions::Instruction* ret_instr;
    PipelineLLNode* curr = head;
    while(count < i)
    {
        curr = curr->next;
        count++;
    }
    prev = curr->prev;
    next = curr->next;
    if (prev != NULL)
    {
        prev->next = next;
    }
    if (next != NULL) {
        next->prev = prev;
    }
    if (i == 0)
    {
        head = next;
    }
    if (i == size - 1)
    {
        tail = prev;
    }
    ret_instr = curr->payload;
    curr->prev = NULL;
    curr->next = NULL;
    curr->payload = NULL;
    size--;
    delete curr;
    return ret_instr;
}

void PipelineLL::removeAndDestroy(int i)
{
    Instructions::Instruction* ret_instr = remove(i);
    delete ret_instr;
    return;
}

void PipelineLL::flushCompletedInstructions()
{
    PipelineLLNode* curr = head;
    int count = 0;
    while(curr != NULL)
    {
        PipelineLLNode* next = curr->next;
        curr = NULL;
        if (curr->payload->stage == DONE)
        {
            removeAndDestroy(count);
        }
        curr = next;
        count++;
    }
    return;
}