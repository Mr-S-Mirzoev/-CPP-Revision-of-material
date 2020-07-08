template <typename T>
Lab::Triangle_Matrix<T>* Input(T type) {
	int n;
	std::string input = "wow";
	std::cout << "Input matrix size:" << std::endl;
	std::cin >> n;
	std::cout << "Input \"true\" if matrix is upper triangle, \"false\" else" << std::endl;
	while (input != "true" && input != "false") {
		std::cin >> input;
		if (input != "true" && input != "false")
			std::cout << "Input \"true\" if matrix is upper triangle, \"false\" else. Try again" << std::endl;
	}
	Lab::Triangle_Matrix<T>* matrix;
	
	matrix = new Lab::Triangle_Matrix<T>(n, input == "true");
	bool flag = false;
	std::cout << "Input matrix:" << std::endl;
	
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			T in;
			std::cin >> in;
			if (matrix->wrong(i, j, in)) {
				in = T();
				flag = true;
			}
			matrix->Set(in, i, j);
		}
	if (flag) {
		std::cout << "All wrong elements were equaled to 0. Now your matrix is:" << std::endl;
		matrix->Print();
		std::cout << std::endl;
	}
	return matrix;
}


template <typename T>
void interface(T type) 
{
	int act = 0;
	double norme;
	Lab::Triangle_Matrix<T>* MainMatrix = nullptr;

	while (act != 7)
	{
		std::cout << "Choose action with triangle matrix:" << std::endl;
		std::cout << "\t1. Input new matrix (previous will be deleted)" << std::endl;
		std::cout << "\t2. Set existing matrix element" << std::endl;
		std::cout << "\t3. Add two matrixes" << std::endl;
		std::cout << "\t4. Scalar multiply of matrix" << std::endl;
		std::cout << "\t5. Get norme of matrix" << std::endl;
		std::cout << "\t6. Print matrix" << std::endl;
		std::cout << "\t7. exit" << std::endl;
		std::cout << "Action number:";
		std::cin >> act;
		switch (act)
		{
		case 1:
			if (MainMatrix != nullptr)
				delete MainMatrix;
			MainMatrix = Input(type);
			break;
		case 2:
			int i, j;
			T x;
			std::cout << "Input element coordinates i, j:" << std::endl;
			std::cin >> i >> j;
			std::cout << "Input new element" << std::endl;
			std::cin >> x;
			MainMatrix->Set(x, i, j);
			break;
		case 3:
			Lab::Triangle_Matrix<T>* AddMatrix;
			AddMatrix = Input(type);
			MainMatrix->Sum(AddMatrix);
			std::cout << "The summury matrix is:" << std::endl;
			MainMatrix->Print();
			break;
		case 4:
			T c;
			std::cout << "Input c:" << std::endl;
			std::cin >> c;
			MainMatrix->Scalar(c);
			MainMatrix->Print();
			break;
		case 5:
			norme = MainMatrix->GetNorme();
			std::cout << "The norme of matrix is: " << norme << std::endl;
			break;
		case 6:
			MainMatrix->Print();
			break;
		case 7:
			break;
		default:
			std::cout << "WRONG ACTION NUMBER"<<std::endl;
			break;
		}
	}
}
