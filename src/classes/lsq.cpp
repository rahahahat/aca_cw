#include "lsq.h"
#include <set>

LSQNode::LSQNode(Instructions::Instruction *instrPtr, int entry_num)
{
    instr = instrPtr;
    entry_number = entry_num;
    return;
}

Instructions::Instruction* LSQNode::getInstruction()
{
    return instr;
}

int LSQNode::getAddress()
{
    return instr->immediateOrAddress;
}

int LSQNode::getEntryNumber()
{
    return entry_number;
}

LSQueue::LSQueue()
{
    queue = new LinkedList<LSQNode>();
    processor = Processor::getProcessorInstance();
    return;
}

void LSQueue::addToQueue(Instructions::Instruction *instrPtr)
{
    total_entries += 1;
    LSQNode *node = new LSQNode(instrPtr, total_entries);
    queue->add(node);
    return;
}

bool LSQueue::isOpValid(Instructions::Instruction *instrPtr)
{
    LLNode<LSQNode> *head = queue->head;
    std::set<int> addr_set;
    while(head != NULL)
    {
        LSQNode *l_node = head->payload;
        addr_set.insert(l_node->getAddress());
        if (instrPtr == l_node->getInstruction())
        {
            auto itr = addr_set.find(l_node->getAddress());
            if (itr != addr_set.end()) 
            {
                addr_set.clear();
                return false;
            }
            addr_set.clear();
            return true;
        }
        head = head->next;
    }
    // TODO: Add panic or warning for load/store instruction not in queue.
    return false;
}

void LSQueue::addInstructionOnEvent(const EventBase& base)
{
   const Event<Instructions::Instruction*>& event = static_cast<const Event<Instructions::Instruction*>&>(base);
   addToQueue(event.payload);
   return;
};