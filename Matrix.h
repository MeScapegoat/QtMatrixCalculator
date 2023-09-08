#pragma once
#include <vector>
#include <iostream>
class Matrix
{
    friend std::ifstream& operator>>(std::ifstream& is, Matrix& matrix);
    using Matrix_t = std::vector<double>;
    using iterator = Matrix_t::iterator;
    using const_iterator = Matrix_t::const_iterator;
    using reverse_iterator = Matrix_t::reverse_iterator;
    using const_reverse_iterator = Matrix_t::const_reverse_iterator;
	using row_iterator = iterator;
	using const_row_iterator = const_iterator;
	using reverse_row_iterator = reverse_iterator;
	using const_reverse_row_iterator = const_reverse_iterator;
	size_t rows;
	size_t cols;
    Matrix_t matrix;
//iterators
	class column_iterator;
	class const_column_iterator;
	class reverse_column_iterator;
	class const_reverse_column_iterator;
private:
	class column_iterator
	{
		friend const_column_iterator;
        Matrix& mat;
		size_t row;
		size_t col;
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = double;
		using reference = double&;
		using pointer = double*;
		using difference_type = size_t;

        column_iterator(Matrix& m, const size_t& r, const size_t& c) : mat(m), row(r), col(c) {}
		column_iterator(const column_iterator&) = default;
		column_iterator(column_iterator&&) = default;
		column_iterator operator++();
		column_iterator operator++(int);
		column_iterator operator--();
		column_iterator operator--(int);
		column_iterator operator+=(const difference_type&);
		column_iterator operator-=(const difference_type&);
		column_iterator operator+(const difference_type&);
		column_iterator operator-(const difference_type&);
		difference_type operator-(const column_iterator&) const;
		column_iterator& operator=(const column_iterator& other);
		bool operator==(const column_iterator&);
		bool operator!=(const column_iterator&);
		bool operator<(const column_iterator&);
		bool operator<=(const column_iterator&);
		bool operator>(const column_iterator&);
		bool operator>=(const column_iterator&);
		reference operator[] (const difference_type&);
		reference operator*();
	};

	class const_column_iterator
	{
        Matrix& mat;
		size_t row;
		size_t col;
	public:
		using iterator_category = std::random_access_iterator_tag;
        using value_type = double;
		using reference = const double&;
		using pointer = const double*;
		using difference_type = size_t;

        const_column_iterator(const Matrix& m, const size_t& r, const size_t& c) : mat(const_cast<Matrix&>(m)), row(r), col(c) {}
		const_column_iterator(const column_iterator&);
		const_column_iterator(const const_column_iterator&) = default;
		const_column_iterator(const_column_iterator&&) = default;
		const_column_iterator operator++();
		const_column_iterator operator++(int);
		const_column_iterator operator--();
		const_column_iterator operator--(int);
		const_column_iterator operator+=(const difference_type&);
		const_column_iterator operator-=(const difference_type&);
		const_column_iterator operator+(const difference_type&);
		const_column_iterator operator-(const difference_type&);
		difference_type operator-(const const_column_iterator&) const;
		const_column_iterator& operator=(const const_column_iterator& other);
		bool operator==(const const_column_iterator&);
		bool operator!=(const const_column_iterator&);
		bool operator<(const const_column_iterator&);
		bool operator<=(const const_column_iterator&);
		bool operator>(const const_column_iterator&);
		bool operator>=(const const_column_iterator&);
		value_type operator[] (const difference_type&);
		value_type operator*();
	};
	
	class reverse_column_iterator
	{
		friend const_reverse_column_iterator;
        Matrix& mat;
		size_t row;
		size_t col;
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = double;
		using reference = double&;
		using pointer = double*;
		using difference_type = size_t;

        reverse_column_iterator(Matrix& m, const size_t& r, const size_t& c) : mat(m), row(r), col(c) {}
		reverse_column_iterator(const reverse_column_iterator&) = default;
		reverse_column_iterator(reverse_column_iterator&&) = default;
		reverse_column_iterator operator++();
		reverse_column_iterator operator++(int);
		reverse_column_iterator operator--();
		reverse_column_iterator operator--(int);
		reverse_column_iterator operator+=(const difference_type&);
		reverse_column_iterator operator-=(const difference_type&);
		reverse_column_iterator operator+(const difference_type&);
		reverse_column_iterator operator-(const difference_type&);
		difference_type operator-(const reverse_column_iterator&) const;
		reverse_column_iterator& operator=(const reverse_column_iterator& other);
		bool operator==(const reverse_column_iterator&);
		bool operator!=(const reverse_column_iterator&);
		bool operator<(const reverse_column_iterator&);
		bool operator<=(const reverse_column_iterator&);
		bool operator>(const reverse_column_iterator&);
		bool operator>=(const reverse_column_iterator&);
		reference operator[] (const difference_type&);
		reference operator*();
		column_iterator base();
	};
	
	class const_reverse_column_iterator 
	{
        Matrix& mat;
		size_t row;
		size_t col;
	public:
		using iterator_category = std::random_access_iterator_tag;
        using value_type = double;
		using reference = const double&;
		using pointer = const double*;
		using difference_type = size_t;

        const_reverse_column_iterator(const Matrix& m, const size_t& r, const size_t& c) : mat(const_cast<Matrix&>(m)), row(r), col(c) {}
		const_reverse_column_iterator(const const_reverse_column_iterator&) = default;
		const_reverse_column_iterator(const_reverse_column_iterator&&) = default;
		const_reverse_column_iterator(const reverse_column_iterator&);
		const_reverse_column_iterator operator++();
		const_reverse_column_iterator operator++(int);
		const_reverse_column_iterator operator--();
		const_reverse_column_iterator operator--(int);
		const_reverse_column_iterator operator+=(const difference_type&);
		const_reverse_column_iterator operator-=(const difference_type&);
		const_reverse_column_iterator operator+(const difference_type&);
		const_reverse_column_iterator operator-(const difference_type&);
		difference_type operator-(const const_reverse_column_iterator&) const;
		const_reverse_column_iterator& operator=(const const_reverse_column_iterator& other);
		bool operator==(const const_reverse_column_iterator&);
		bool operator!=(const const_reverse_column_iterator&);
		bool operator<(const const_reverse_column_iterator&);
		bool operator<=(const const_reverse_column_iterator&);
		bool operator>(const const_reverse_column_iterator&);
		bool operator>=(const const_reverse_column_iterator&);
		value_type operator[] (const difference_type&);
		value_type operator*();
		const_column_iterator base();
	};
//iterators

//matrix specific member functions
private:
	//see det()
    double do_det(Matrix) const;

	//see minor()
	double do_minor(const size_t&, const size_t&) const;

	//see transpose()
    Matrix do_transpose() const;

	//see invertible()
    Matrix do_inverse() const;
public:
    Matrix();

	/* Creates row x col matrix filled with 0.0.*/
    Matrix(const size_t& row, const size_t& col);

	/* Creates row x col matrix filled with number.*/
    Matrix(const size_t& row, const size_t& col, const double& number);

	/* Creates row x col matrix filled with initializer list elements, row by row.
	* Extra elements are cut off.
	* Missing elements becomes 0.*/
    Matrix(const size_t& row, const size_t& col, std::initializer_list<double>);

	/* Creates row x col matrix filled with user input from std::istream.
	* Every failed reading is skipped, reads until all row * col elements are read.
	* No special format of input required.*/
    Matrix(const size_t& row, const size_t& col, std::istream&);

    Matrix(const Matrix&) = default;
    Matrix(Matrix&&) = default;
    Matrix& operator=(const Matrix&);

	/* Return amount of rows of the matrix.*/
	size_t row() noexcept { return rows; }
	size_t row() const noexcept { return rows; }

	/* Return amount of columns of the matrix.*/
	size_t col() noexcept { return cols; }
	size_t col() const noexcept { return cols; }

	/* Returns matrix determinant.
	UB if matrix is not square matrix.*/
	double det() { return do_det(*this); }
	double det() const { return do_det(*this); }

	/* Checks if matrix is square.*/
	bool is_square() { return rows == cols; }
	bool is_square() const { return rows == cols; }

	/* Returns determinant of matrix with row and col removed.
	Does not change the matrix.
	UB if there is no such row or col in the matrix.*/
	double minor(const size_t& row, const size_t& col) { return do_minor(row, col); }
	double minor(const size_t& row, const size_t& col) const { return do_minor(row, col); }

	/* Return lvalue to [row, col] element of the matrix.
	UB if there is no such element.*/
	double& operator()(const size_t& row, const size_t& col);
	const double& operator() (const size_t&, const size_t&) const;

	/* Returns inverse matrix.
	Does not change the matrix.
	UB if matrix is not invertible.*/
    Matrix inverse() { return do_inverse(); }
    Matrix inverse() const { return do_inverse(); }

	/* Returns transposed matrix.
	Does not change the matrix.*/
    Matrix transpose() { return do_transpose(); }
    Matrix transpose() const { return do_transpose(); }

    /* Removes index column from the matrix.
    UB if there is no such col.
	Returns changed matrix.*/
    Matrix remove_col(const size_t& index);

    /* Removes index row from the matrix.
    UB if there is no such row.
	Returns changed matrix.*/
    Matrix remove_row(const size_t& index);

    /* Removes amount columns from the matrix starting at index included.
    UB if there is no such col or amount is above matrix size.
    Returns changed matrix.*/
    Matrix remove_cols(const size_t& index, const size_t& amount);

    /* Removes amount rows from the matrix starting at index included.
    UB if there is no such row or amount is above matrix size.
    Returns changed matrix.*/
    Matrix remove_rows(const size_t& index, const size_t& amount);

    /* Add an empty column to the matrix at the index place.
    Returns changed matrix.*/
    Matrix add_col(const size_t& index);

    /* Add an empty row to the matrix at the index place.
    Returns changed matrix.*/
    Matrix add_row(const size_t& index);

    /* Add amount empty columns at the index place.
    Returns changed matrix.*/
    Matrix add_cols(const size_t& index, const size_t& amount);

    /* Add amount empty rows at the index place.
    Returns changed matrix.*/
    Matrix add_rows(const size_t& index, const size_t& amount);

	/* Makes matrix identity.
	Returns changed matrix.
	UB if matrix is not a square.*/
    Matrix make_identity();

	/* swap row1 and row2 with each other in the matrix.
	UB if there is no a row.*/
	void swap_rows(const size_t& row1, const size_t& row2);

	/* swap col1 and col2 with each other in the matrix.
	UB if there is no a col.*/
	void swap_cols(const size_t& col1, const size_t& col2);

	/* Adds row2 to row1, element by element.
	UB if there is no a row.*/
	void sum_rows(const size_t& row1, const size_t& row2);

	/* Adds col2 to col1, element by element.
	UB if there is no a col.*/
	void sum_cols(const size_t& col1, const size_t& col2);

    /* Substract row2 from row1, element by element.
    UB if there is no a row.*/
    void sub_rows(const size_t& row1, const size_t& row2);

    /* Substract col2 from col1, element by element.
    UB if there is no a col.*/
    void sub_cols(const size_t& col1, const size_t& col2);

	/* Add number to all elements of row.
	UB if there is no such row.*/
	void row_add(const size_t& row, const double& number);

	/* Multiply all elements of row by number.
	UB if there is no such row.*/
	void row_mult(const size_t& row, const double& number);

	/* Divide all elements of row by number.
	UB if there is no such row.*/
	void row_divide(const size_t& row, const double& number);

	/* Subtract number from each elements of row.
	UB if there is no such row.*/
	void row_sub(const size_t& row, const double& number);

	/* Add number to all elements of col.
	UB if there is no such col.*/
	void col_add(const size_t& col, const double& number);

	/* Multiply all elements of col by number.
	UB if there is no such col.*/
	void col_mult(const size_t& col, const double& number);

	/* Divide all elements of col by number.
	UB if there is no such col.*/
	void col_divide(const size_t& col, const double& number);

	/* Subtract number from each elements of col.
	UB if there is no such col.*/
	void col_sub(const size_t& col, const double& number);

	/*Row iterators are just like common iterators. Added for additional clarity.
	* Example:
	* * * * * Matrix will be iterated like:
	* 1 2 3 * 1, 2, 3, 4, 5, 6, 7, 8, 9.
	* 4 5 6 *
	* 7 8 9 *
	* * * * */
	row_iterator rowbegin() noexcept { return matrix.begin(); }
	const_row_iterator rowbegin() const noexcept { return matrix.begin(); }
	row_iterator rowend() noexcept { return matrix.end(); }
	const_row_iterator rowend() const noexcept { return matrix.end(); }
	const_row_iterator crowbegin() const noexcept { return matrix.begin(); }
	const_row_iterator crowend() const noexcept { return matrix.end(); }
	reverse_row_iterator rrowbegin() noexcept { return matrix.rbegin(); }
	const_reverse_row_iterator rrowbegin() const noexcept { return matrix.rbegin(); }
	const_reverse_row_iterator crrowbegin() const noexcept { return matrix.crbegin(); }
	reverse_row_iterator rrowend() noexcept { return matrix.rend(); }
	const_reverse_row_iterator rrowend() const noexcept { return matrix.rend(); }
	const_reverse_row_iterator crrowend() const noexcept { return matrix.crend(); }

	/*Column iterators lets you iterate matrix through columns.
	* Example:
	* * * * * Matrix will be iterated like:
	* 1 2 3 * 1, 4, 7, 2, 5, 8, 3, 6, 9.
	* 4 5 6 *
	* 7 8 9 *
	* * * * */
	column_iterator colbegin() noexcept;
	const_column_iterator colbegin() const noexcept;
	column_iterator colend() noexcept;
	const_column_iterator colend() const noexcept;
	const_column_iterator ccolbegin() const noexcept;
	const_column_iterator ccolend() const noexcept;
	reverse_column_iterator rcolbegin() noexcept;
	const_reverse_column_iterator rcolbegin() const noexcept;
	const_reverse_column_iterator crcolbegin() const noexcept;
	reverse_column_iterator rcolend() noexcept;
	const_reverse_column_iterator rcolend() const noexcept;
	const_reverse_column_iterator crcolend() const noexcept;
//matrix specific member functions

//stolen from vector
public:
	bool empty() const noexcept { return matrix.empty(); }
    void swap(Matrix& other) { matrix.swap(other.matrix); std::swap(cols, other.cols); std::swap(rows, other.rows); }
	size_t capacity() const noexcept { return matrix.capacity(); }
	double& operator[](const size_t& pos) { return matrix[pos]; }
	const double& operator[](const size_t& pos) const { return matrix[pos]; }
	iterator begin() noexcept { return matrix.begin(); }
	const_iterator begin() const noexcept { return matrix.begin(); }
	iterator end() noexcept { return matrix.end(); }
	const_iterator end() const noexcept { return matrix.end(); }
	const_iterator cbegin() const noexcept { return matrix.begin(); }
	const_iterator cend() const noexcept { return matrix.end(); }
	reverse_iterator rbegin() noexcept { return matrix.rbegin(); }
	const_reverse_iterator rbegin() const noexcept { return matrix.rbegin(); }
	const_reverse_iterator crbegin() const noexcept { return matrix.crbegin(); }
	reverse_iterator rend() noexcept { return matrix.rend(); }
	const_reverse_iterator rend() const noexcept { return matrix.rend(); }
	const_reverse_iterator crend() const noexcept { return matrix.crend(); }
	iterator erase(const_iterator pos) { return matrix.erase(pos); }
	iterator erase(const_iterator first, const_iterator last) { return matrix.erase(first, last); }
};
//stolen from vector

//matrix specific non-member functions
/* Returns addition of matrix and number.*/
Matrix operator+(const Matrix& matrix, const double& number);

/* Returns subtract of matrix and number.*/
Matrix operator-(const Matrix& matrix, const double& number);

/* Returns division of matrix and number.*/
Matrix operator*(const Matrix& matrix, const double& number);

/* Returns multiplication of matrix and number.*/
Matrix operator/(const Matrix& matrix, const double& number);

/* Returns addition of two matrices.
UB if matrices cannot be added.*/
Matrix operator+(const Matrix& matrix1, const Matrix& matrix2);

/* Returns subtract of two matrices.
UB if matrices cannot be subtracted.*/
Matrix operator-(const Matrix& matrix1, const Matrix& matrix2);

/* Returns multiplication of two matrices.
UB if matrices cannot be multiplicated.*/
Matrix operator*(const Matrix& matrix1, const Matrix& matrix2);

/* Matrix formatted output.*/
std::ostream& operator<<(std::ostream& os, const Matrix& matrix);

/* Matrix file input. */
std::ifstream& operator>>(std::ifstream& is, Matrix& matrix);

bool operator==(const Matrix& matrix1, const Matrix& matrix2);
//matrix specific non-member functions
