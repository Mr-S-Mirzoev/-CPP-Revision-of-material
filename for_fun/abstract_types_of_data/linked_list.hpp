namespace Lab {
    template <typename T> 
    class LinkedList {
        struct ListNode {
            T item;
            ListNode* next = nullptr;
        };
        ListNode* head = nullptr;
        int size = 0;
    public:
        LinkedList() : size(0) {}
        LinkedList(T* items, int size) : LinkedList() {
            if (size < 0) 
                throw std::logic_error("Trying to make a list with a negative size: " + std::to_string(size));
            if (!items && size > 0)
                throw std::runtime_error("Passing nullptr, assuming it is a non-void array");
            ListNode** elem = &(this->head);
            for (int i = 0; i < size; ++i) {
                *elem = new ListNode;
                (*elem)->item = items[i];
                elem = &((*elem)->next);
            }
            this->size = size;
        }

        LinkedList(int size) : LinkedList() {
            if (size < 0) 
                throw std::logic_error("Trying to make a list with a negative size: " + std::to_string(size));

            ListNode** elem = &(this->head);
            for (int i = 0; i < size; ++i) {
                *elem = new ListNode;
                (*elem)->item = T();
                elem = &((*elem)->next);
            }
            this->size = size;
        }

        LinkedList(const LinkedList<T>& list) {
            ListNode* elem = list.head;
            ListNode** newelem = &(this->head);

            for (int i = 0; i < list.size; i++, elem = elem->next) {
                *newelem = new ListNode;
                (*newelem)->item = elem->item;
                newelem = &((*newelem)->next);
            }
            this->size = list.size;
        }

        virtual ~LinkedList() {
            ListNode* elem = this->head;
            ListNode* next;
            while (elem != nullptr) {
                next = elem->next;
                delete elem;
                elem = next;
            }
            this->size = 0;
        }

        T GetFirst() const {
            if (this->size == 0) 
                throw Lab::IndexOutOfRange("Trying to get the first element of an empty list");
            return this->head->item;
        }

        T GetLast() const {
            if (this->size == 0) 
                throw Lab::IndexOutOfRange("Trying to get the last element of an empty list");
            ListNode* elem = this->head;
            while (elem->next != nullptr) 
                elem = elem->next;
            return elem->item;
        }

        T Get(int index) const {
            if (index < 0) 
                throw Lab::IndexOutOfRange("Trying to get an element with a negative index");
            if (index >= this->size)
                throw Lab::IndexOutOfRange("Went out of range, trying to get " + std::to_string(index) + "-th element in array with length " + std::to_string(size));

            ListNode* elem = this->head;
            for (int i = 0; i < index; ++i)
                elem = elem->next;

            return elem->item;
        }

        void Set(const T& item, int index) {
            if (index < 0) 
                throw Lab::IndexOutOfRange("Trying to set an element with a negative index");
            if (index >= this->size)
                throw Lab::IndexOutOfRange("Went out of range, trying to set " + std::to_string(index) + "-th element in array with length " + std::to_string(size));

            ListNode* elem = this->head;
            for (int i = 0; i < index; ++i)
                elem = elem->next;
            elem->item = item;
        }

        LinkedList<T>* GetSubList(int start, int end) const { //end is excluding
            if (start < 0 || start >= this->size || end < 0 || end > this->size || start > end) 
                throw Lab::IndexOutOfRange("Trying to get sublist with the wrong bounds");

            LinkedList<T>* newList = new LinkedList<T>();
            ListNode* elem = this->head;
            ListNode** newelem = &newList->head;

            for (int i = 0; i < end; ++i, elem = elem->next) 
                if (i >= start) {
                    *newelem = new ListNode;
                    (*newelem)->item = elem->item;
                    newelem = &((*newelem)->next);
                }
            newList->size = end - start;
            return newList;
        }

        int GetLength() const { 
            return this->size; 
        }

        void Append(const T& item) {
            ListNode** elem = &(this->head);

            while (*elem != nullptr) 
                elem = &((*elem)->next);
            (*elem) = new ListNode;
            (*elem)->item = item;

            ++(this->size);
        }

        void Prepend(const T& item) {
            ListNode* elem = new ListNode{item, this->head};
            this->head = elem;
            ++(this->size);
        }

        void InsertAt(const T& item, int index) {
            if (index < 0) 
                throw Lab::IndexOutOfRange("Trying to set an element with a negative index");
            if (index > this->size)
                throw Lab::IndexOutOfRange("Went out of range, trying to set " + std::to_string(index) + "-th element in array with length " + std::to_string(size));

            if (!index) {
                ListNode *newhead = new ListNode;
                newhead->next = this->head;
                newhead->item = item;
                this->head = newhead;
            } else {
                ListNode *elem = this->head;
                for (int i = 0; i < index - 1; ++i)
                    elem = elem->next;
                ListNode *prevnext = elem->next;
                elem->next = new ListNode;
                elem->next->item = item;
                elem->next->next = prevnext;
            }
        
            ++(this->size);
        }

        LinkedList<T>* Concat(const LinkedList<T>& list) const {
            ListNode* first = this->head;
            ListNode* second = list.head;

            LinkedList<T>* newList = new LinkedList<T>();
            ListNode** elem = &(newList->head);
            while (first != nullptr) {
                *elem = new ListNode;
                (*elem)->item = first->item;
                (*elem)->next = first->next;
                first = first->next;
                elem = &((*elem)->next);
            }
            while (second != nullptr) {
                *elem = new ListNode;
                (*elem)->item = second->item;
                (*elem)->next = second->next;
                second = second->next;
                elem = &((*elem)->next);
            }

            newList->size = this->size + list.size;
            return newList;
        }
        friend std::ostream& operator<<(std::ostream &out, const LinkedList &ll) {
            if (ll.size == 0) {
                out << "Empty LinkedList";
            } else {
                ListNode* elem = ll.head;
                for (int i = 0; i < ll.size - 1; ++i) {
                    out << elem->item << " ";
                    elem = elem->next;
                }
                out << elem->item;
            }
            return out;
        }
    };
}