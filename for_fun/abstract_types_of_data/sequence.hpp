namespace Lab {
    template <typename T> 
    class Sequence {
    protected:
        int size = 0;
    public:
        virtual T GetFirst() const = 0;
        virtual T GetLast() const = 0;
        virtual T Get(int index) const = 0;
        virtual int GetSize() const { return this->size; }
        virtual Sequence<T>* GetSubSequence(int start, int end) const = 0; //end excluding
        virtual void Set(const T& item, int index) = 0;
        virtual void Append(const T& item) = 0;
        virtual void Prepend(const T& item) = 0;
        virtual void InsertAt(const T& item, int index) = 0;
        virtual Sequence<T>* Concat(const Sequence<T>& Sequence) const = 0;
        virtual void Print () const = 0;
    };

    template <typename T> 
    class ArraySequence : public Sequence<T> {
    protected:
        DynamicArray<T>* arr;
    public:
        ArraySequence() {
            this->arr = new DynamicArray<T>();
            this->size = 0;
        }
        ArraySequence(const ArraySequence<T>& Sequence) {
            this->arr = new DynamicArray<T>(*Sequence.arr);
            this->size = Sequence.size;
        }
        ArraySequence(DynamicArray<T>* items) {
            this->arr = items;
            this->size = items->GetSize();
        }
        ArraySequence(T* items, int size) {
            this->arr = new DynamicArray<T>(items, size);
            this->size = size;
        }
        ArraySequence(int size) {
            this->arr = new DynamicArray<T>(size);
            this->size = size;
        }
        virtual ~ArraySequence() {
            delete this->arr;
            this->size = 0;
        }
        virtual T GetFirst() const override {
            return this->arr->Get(0);
        }
        virtual T GetLast() const override {
            return this->arr->Get(this->size - 1);
        }
        virtual T Get(int index) const override {
            return this->arr->Get(index);
        }
        virtual void Set(const T& item, int index) override {
            if (index < 0 || index >= this->size) 
                throw Lab::IndexOutOfRange("Trying to set an element with a wrong index");
            this->arr->Set(index, item);
        }
        virtual ArraySequence<T>* GetSubSequence(int start, int end) const override {
            if (start < 0 || start >= this->size || end < 0 || end > this->size || start > end) 
                throw Lab::IndexOutOfRange("Trying to get subSequence with the wrong bounds");
            ArraySequence<T> *sub = new ArraySequence<T>(end - start);
            for (int i = 0; i < end - start; ++i)
                sub->Set(this->arr->Get(i + start), i);
            return sub;
        }
        virtual void Append(const T& item) override {
            this->arr->Resize((this->size) + 1);
            this->arr->Set((this->size), item);
            ++(this->size);
        }
        virtual void Prepend(const T& item) override {
            this->arr->Resize(this->size + 1);
            T a = this->arr->Get(0);
            T b;
            for (int i = 0; i < this->size; ++i) {
                b = a;
                a = this->arr->Get(i + 1);
                this->arr->Set(i + 1, b);
            }
            this->arr->Set(0, item);
            ++(this->size);
        }
        virtual void InsertAt(const T& item, int index) override {
            if (index < 0) 
                throw Lab::IndexOutOfRange("Trying to set an element with a negative index");
            if (index >= this->size)
                throw Lab::IndexOutOfRange("Went out of range, trying to set " + std::to_string(index) + "-th element in array with length " + std::to_string(this->arr->GetSize()));
            this->arr->Resize(this->size + 1);
            T a = this->arr->Get(index);
            T b;
            for (int i = index; i < this->size; ++i) {
                b = a;
                a = this->arr->Get(i + 1);
                this->arr->Set(i + 1, b);
            }
            this->arr->Set(index, item);
            ++(this->size);
        }
        virtual ArraySequence<T>* Concat(const Sequence<T>& Sequence) const override {
            ArraySequence<T>* newSequence = new ArraySequence<T>(this->size + Sequence.GetSize());
            for (int i = 0; i < this->size; ++i)
                newSequence->Set(this->Get(i), i);
            for (int i = 0; i < Sequence.GetSize(); ++i) {
                newSequence->Set(Sequence.Get(i), i + this->size);
            }
            return newSequence;
        }
        virtual void Print () const override {
            std::cout << *(this->arr) << std::endl;
        }
        DynamicArray<T> *getptr() {
            return arr;
        }
    };

    template <typename T> 
    class ListSequence : public Sequence<T> {
    protected:
        LinkedList<T>* list;
    public:
        ListSequence() {
            this->list = new LinkedList<T>();
            this->size = 0;
        }
        ListSequence(const ListSequence<T>& Sequence) {
            this->list = new LinkedList<T>(*Sequence.list);
            this->size = Sequence.size;
        }
        ListSequence(LinkedList<T>* list) {
            this->list = list;
            this->size = list->GetLength();
        }
        ListSequence(T* items, int size) {
            this->list = new LinkedList<T>(items, size);
            this->size = size;
        }
        ListSequence(int size) {
            this->list = new LinkedList<T>(size);
            this->size = size;
        }
        virtual ~ListSequence() {
            delete this->list;
            this->size = 0;
        }
        virtual T GetFirst() const override {
            return this->list->GetFirst();
        }
        virtual T GetLast() const override {
            return this->list->GetLast();
        }
        virtual T Get(int index) const override {
            return this->list->Get(index);
        }
        virtual ListSequence<T>* GetSubSequence(int start, int end) const override {
            LinkedList<T>* subList = this->list->GetSubList(start, end);
            ListSequence<T>* Sequence = new ListSequence<T>(subList);
            return Sequence;
        }
        virtual void Set(const T& item, int index) override {
            this->list->Set(item, index);
        }
        virtual void Append(const T& item) override {
            this->list->Append(item);
            ++(this->size);
        }
        virtual void Prepend(const T& item) override {
            this->list->Prepend(item);
            ++(this->size);
        }
        virtual void InsertAt(const T& item, int index) override {
            this->list->InsertAt(item, index);
            ++(this->size);
        }
        virtual ListSequence<T>* Concat(const Sequence<T>& Sequence) const override {
            ListSequence<T>* newSequence = new ListSequence<T>();
            for (int i = 0; i < this->size; ++i)
                newSequence->Append(this->Get(i));
            for (int i = 0; i < Sequence.GetSize(); ++i)
                newSequence->Append(Sequence.Get(i));
            return newSequence;
        }
        virtual void Print () const override {
            std::cout << *(this->list) << std::endl;
        }
    };
}