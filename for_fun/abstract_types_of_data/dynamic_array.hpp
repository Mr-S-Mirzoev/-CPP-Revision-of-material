namespace Lab {
    template <class T>
    class DynamicArray {
        T* arr;
        int size = 0;
    public:
        DynamicArray() : size(0), arr(nullptr) {}
        DynamicArray(T* items, int size): size(size) {
            if (size < 0) {
                throw std::logic_error("Trying to make an array with a negative size: " + std::to_string(size));
            } else if (size == 0) {
                arr = nullptr;
            } else {
                arr = new(std::nothrow) T[size];
                if (!arr)
                    throw std::runtime_error("Allocation during construction failed");
                if (!items && size > 0)
                    throw std::runtime_error("Passing nullptr, assuming it is a non-void array");
                for (int i = 0; i < size; ++i)
                    arr[i] = items[i];
            }
        }
        DynamicArray (int size): size(size) {
            if (size < 0)
                throw std::logic_error("Trying to make an array with a negative size: " + std::to_string(size));
            else if (size == 0) {
                arr = nullptr;
            } else {
                arr = new(std::nothrow) T[size];
                if (!arr)
                    throw std::runtime_error("Allocation during construction failed");
            }
        }
        DynamicArray(const DynamicArray<T> &dynamicArray): size(dynamicArray.size) {
            arr = new(std::nothrow) T[size];
            if (!arr)
                throw std::runtime_error("Allocation during construction failed");
            for (int i = 0; i < size; ++i)
                arr[i] = dynamicArray.arr[i];
        }
        T Get(int index) {
            if (index >= size)
                throw Lab::IndexOutOfRange("Went out of range, requesting " + std::to_string(index) + "-th element in array with length " + std::to_string(size));
            if (index < 0) 
                throw Lab::IndexOutOfRange("Went out of range, requesting element with a negative index");
            return arr[index];
        }
        int GetSize() const {
            return size;
        }
        void Set(int index, T value) {
            if (index < 0)
                throw Lab::IndexOutOfRange("Went out of range, trying to set value to a negative index");
            if (index >= size)
                throw Lab::IndexOutOfRange("Went out of range, trying to set " + std::to_string(index) + "-th element in array with length " + std::to_string(size));
            arr[index] = value;
        }
        void Resize(int newSize) {
            if (newSize == size)
                return;
            if (newSize < 0)
                throw std::logic_error("Trying to resize the array to a negative size: " + std::to_string(newSize));
            T* ptr;
            ptr = new (std::nothrow) T[newSize];
            if (!ptr)
                throw std::runtime_error("Allocation during Resize failed");
            for (int i = 0; i < std::min(newSize, size); ++i)
                ptr[i] = arr[i];
            if (arr)
                delete []arr;
            arr = ptr;
            size = newSize;
        }
        ~DynamicArray () {
            delete []arr;
        }
        friend std::ostream& operator<<(std::ostream &out, const DynamicArray &da) {
            if (da.size == 0) {
                out << "Empty DynamicArray";
            } else {
                for (int i = 0; i < da.size - 1; ++i)
                    out << da.arr[i] << " ";
                out << da.arr[da.size - 1];
            }
            return out;
        }
    };
}