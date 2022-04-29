#include "lsq.h"
#include "util.h"
#include "prochelpers.h"
#include <set>

// TODO: Add Capacity logic

LSQNode::LSQNode(std::string str): ReserveEntry(str)
{
    addr = -1;
    address = -1;
    busy = false;
    return;
}

bool LSQNode::isBusy()
{
    return busy;
}

int LSQNode::getAddress()
{
    return address;
}

void LSQNode::populateSources(std::string tag, int value)
{
    if (tag_pair.first.compare(tag) == 0)
    {
        valid_pair.first = 1;
        value_pair.first = value;
    }
    if (tag_pair.second.compare(tag) == 0)
    {
        
        valid_pair.second = 1;
        value_pair.second = value;
    }
    return;
}

LSQueue::LSQueue()
{
    capacity = 32;
    queue = new LinkedList<LSQNode>();
    processor = Processor::getProcessorInstance();
    return;
}

LSQNode* LSQueue::addToQueue(Instructions::Instruction *instrPtr)
{
    std::cout << "ADD_TO_QUEUE" << std::endl;
    LSQNode *node = new LSQNode(randomId(5));
    if (instrPtr->opcode == LW) 
    {
        populateLoad(node, instrPtr);
        queue->add(node);
        return node;
    }
    if (instrPtr->opcode == SW) 
    {
        populateStore(node, instrPtr);
        queue->add(node);
        return node;
    }

}

void LSQueue::removeFromQueue(std::string tag)
{
    LLNode<LSQNode> *head = queue->head;
    while(head != NULL)
    {
        LSQNode *l_node = head->payload;
        if (l_node->getTag().compare(tag) == 0)
        {
            queue->removeAndDestroy(head);
            return;
        }
        head = head->next;
    }
    return;
}

bool LSQueue::hasCapacity()
{
    return queue->size < capacity;
}

int LSQueue::getNumEntries()
{
    return queue->size;
}

void LSQueue::populateLoad(LSQNode* node, Instructions::Instruction *instrPtr)
{
    ScoreboardEntry *sb_one = processor->getSB()->getEntry(instrPtr->rs);

    std::pair<std::string, std::string> tp = std::pair<std::string, std::string>(sb_one->tag, "~");
    std::pair<int, int> vp = std::pair<int, int>(sb_one->valid, 1);
    std::pair<int, int> vap = std::pair<int, int>(sb_one->value, 0);
    node->updateTags(tp);
    node->updateValids(vp);
    node->updateValues(vap);
    node->addr = instrPtr->immediateOrAddress;
    node->destination = instrPtr->rt;
    node->opcode = LW;

    processor->getSB()->inValidate(instrPtr->rt, node->getTag());
}

void LSQueue::populateStore(LSQNode* node, Instructions::Instruction *instrPtr)
{
    ScoreboardEntry* sb_entry_one = processor->getSB()->getEntry(instrPtr->rs);
    ScoreboardEntry* sb_entry_two = processor->getSB()->getEntry(instrPtr->rt);

    node->updateTags(std::pair<std::string, std::string>(sb_entry_one->tag, sb_entry_two->tag));
    node->updateValids(std::pair<int, int>(sb_entry_one->valid, sb_entry_two->valid));
    node->updateValues(std::pair<int, int>(sb_entry_one->value, sb_entry_two->value));
    node->addr = instrPtr->immediateOrAddress;
    node->opcode = SW;

    return;
}

bool LSQueue::isValidLoad(LSQNode *node)
{
    LLNode<LSQNode> *head = queue->head;
    while(head != NULL)
    {
        LSQNode *l_node = head->payload;
        if (node == l_node) return true;
        if (l_node->opcode == SW)
        {
            if (l_node->getAddress() == -1 || l_node->getAddress() == node->getAddress()) return false;
        }
        head = head->next;
    }
    // TODO: Add panic or warning for load/store instruction not in queue.
    return false;
}

bool LSQueue::isValidStore(LSQNode *node)
{
    LLNode<LSQNode> *head = queue->head;
    while(head != NULL)
    {
        LSQNode *l_node = head->payload;
        if (node == l_node) return true;
        if (l_node->getAddress() == -1 || l_node->getAddress() == node->getAddress()) return false;
        head = head->next;
    }
    // TODO: Add panic or warning for load/store instruction not in queue.
    return false;
};

bool LSQueue::isOpValid(LSQNode *node)
{
    if (node->opcode == LW) return isValidLoad(node);
    if (node->opcode == SW) return isValidStore(node);
    return false;
};

void LSQueue::populateTags(std::string tag, int value)
{
    LLNode<LSQNode> *head = queue->head;
    while(head != NULL)
    {
        LSQNode *l_node = head->payload;
        l_node->populateSources(tag, value);
        head = head->next;
    }
    return;
};

LSQNode* LSQueue::getHead()
{
    return queue->head->payload;
}

void LSQueue::calculateAddrs()
{
    LLNode<LSQNode> *head = queue->head;
    // TODO: Verify Load/Store format
    while(head != NULL)
    {
        LSQNode *l_node = head->payload;
        if (!l_node->isReady && l_node->valid_pair.first)
        {
            l_node->address = l_node->value_pair.first + l_node->addr;
            if (l_node->valid_pair.second)
            {
                l_node->isReady = true;
            }
        }
        head = head->next;
    }
    return;
};

LSQNode* LSQueue::getValidInstruction()
{
    LLNode<LSQNode> *head = queue->head;
    while(head != NULL)
    {
        LSQNode *l_node = head->payload;
        if (!l_node->busy && l_node->isReady && isOpValid(l_node))
        {
            l_node->busy = true;
            return l_node;
        }
        head = head->next;
    }
    return NULL;
}

void LSQueue::nextTick()
{
    calculateAddrs();
    return;
}

void LSQueue::print()
{
    LLNode<LSQNode> *head = queue->head;
    while(head != NULL)
    {
        LSQNode* node = head->payload;
        std::string tag = node->getTag();
        if (node->opcode == LW)
        {
            std::cout << termcolor::bold << termcolor::red
            <<  "| opcode: LW" 
            << " | tag: " << tag 
            << " | src-tag: " << node->tag_pair.first
            << " | src-valid: " << node->valid_pair.first
            << " | src-value: " << node->value_pair.first
            << " | address: " << node->addr
            << " | dest: $r" << node->destination
            << " | " << termcolor::reset << std::endl;
        }
        if (node->opcode == SW)
        {
            std::cout << termcolor::bold << termcolor::blue
            <<  "| opcode: SW" 
            << " | tag: " << tag 
            << " | src-tag: " << node->tag_pair.first
            << " | src-valid: " << node->valid_pair.first
            << " | src-value: " << node->value_pair.first
            << " | src-tag: " << node->tag_pair.second
            << " | src-valid: " << node->valid_pair.second
            << " | src-value: " << node->value_pair.second
            << " | address: " << node->addr
            << " | " << termcolor::reset << std::endl;
        }
        head = head->next;
    }
    return;
}