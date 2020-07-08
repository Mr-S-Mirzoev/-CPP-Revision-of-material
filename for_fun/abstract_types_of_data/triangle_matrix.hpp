namespace Lab {
    template <typename T>
    class Triangle_Matrix {
    private:
        int size = 0;
        bool upper = 0;
        Lab::Sequence<Lab::Sequence<T>*>* TrMat;
    public:
        Triangle_Matrix(int size, bool upper) : size(size), upper(upper) {
            if (size < 0)
                throw std::logic_error("Negative size");
            TrMat = (Lab::Sequence<Lab::Sequence<T>*>*) new Lab::ListSequence<Lab::ListSequence<T>*>;
            for (int i = 0; i < size; i++)
                TrMat->Append(new Lab::ListSequence<T>(size));
        }
        
        T Get(int i, int j) const {
            if (i < 0 || i >= this->size || j < 0 || j >= this->size) 
                throw Lab::IndexOutOfRange("Trying to get element with wrong indices");
            return TrMat->Get(i)->Get(j);
        }

        void Fill(T item) {
            if (upper) {
                for (int i = 0; i < this->size; i++)
                    for (int j = 0; j < this->size; j++)
                        if (i <= j)
                            this->Set(item, i, j);
            } else {
                for (int i = 0; i < this->size; i++)
                    for (int j = 0; j < this->size; j++)
                        if (i >= j)
                            this->Set(item, i, j);
            }
        }

        bool wrong (int i, int j, const T &item) const {
            if (item != T()) {
                if (upper) {
                    if (i > j) {
                        return true;
                    }
                } else {
                    if (i < j) {
                        return true;
                    }
                }
            }
            return false;
        }

        void Set(T item, int i, int j) {
            if (item != T()) {
                if (upper) {
                    if (i > j) {
                        throw std::logic_error("Trying to set element of upper triangle matrix where i > j");
                    }
                } else {
                    if (i < j) {
                        throw std::logic_error("Trying to set element of lower triangle matrix where i < j");
                    }
                }
            }
            if (i < 0 || i >= this->size || j < 0 || j >= this->size)  
                throw Lab::IndexOutOfRange("Trying to set element with wrong indices");
            this->TrMat->Get(i)->Set(item, j);
        }

        int GetSize() const {
            return this->size;
        }

        bool GetIsUpper() const  {
            return this->upper;
        }

        double GetNorme() const {
            T sum = 0;
            for (int i = 0; i < this->size; i++)
                for (int j = 0; j < this->size; j++)
                    sum += (this->Get(i, j)) * (this->Get(i, j));
            return std::sqrt(sum);
        }
        
        Triangle_Matrix<T>* Sum(Triangle_Matrix<T>* B) {
            if (this->size != B->size)
                throw Lab::DifferentSizes("Matrices have different sizes");
            if (this->upper ^ B->upper) {
                throw Lab::UpperAndLower("Triangle matrices are upper and lower");
            } 

            for (int i = 0; i < this->size; i++)
                for (int j = 0; j < this->size; j++)
                    this->Set(this->Get(i, j) + B->Get(i, j), i, j);
            return this;
        }

        Triangle_Matrix<T>* Scalar(T c) {
            for (int i = 0; i < this->size; i++)
                for (int j = 0; j < this->size; j++)
                    this->Set(this->Get(i, j) * c, i, j);
            return this;
        }

        void Print() const {
            for (int i = 0; i < this->GetSize(); i++) {
                for (int j = 0; j < this->GetSize(); j++)
                    std::cout << std::setiosflags(std::ios::left)  << std::setw(5) << this->Get(i, j) << " ";
                std::cout << std::endl;
            }
        }
    };
}