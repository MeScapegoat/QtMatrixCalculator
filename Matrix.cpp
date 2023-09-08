#include <cmath>
#include <algorithm>
#include <string>
#include <iomanip>
#include <execution>
#include <fstream>
#include "Matrix.h"

//constructors
Matrix::Matrix() : rows(0), cols(0), matrix(){};
Matrix::Matrix(const size_t& r, const size_t& c) : rows(r), cols(c), matrix() { matrix.resize(r * c); matrix.shrink_to_fit(); }

Matrix::Matrix(const size_t& r, const size_t& c, const double& num) : rows(r), cols(c), matrix(r* c, num) { matrix.resize(r * c); matrix.shrink_to_fit(); }

Matrix::Matrix(const size_t& r, const size_t& c, std::initializer_list<double> il) : rows(r), cols(c), matrix(il) { matrix.resize(r * c); matrix.shrink_to_fit(); }

Matrix::Matrix(const size_t& r, const size_t& c, std::istream& is) : rows(r), cols(c), matrix()
{
	auto amount = r * c;
	double element;
	while (amount)
	{
		if (is >> element)
		{
			matrix.emplace_back(element);
			--amount;
		}
		else
		{

			is.clear();
			is.ignore(1, ' ');
		}
	}
	matrix.resize(r * c);
	matrix.shrink_to_fit();
}
//constructors

std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
{
    size_t max_length = 5;
    size_t max_factional = 0;
    size_t max_integer = 1;
    auto countDigits = [&](auto &el){
        size_t count = 0;
        auto stringNum = std::to_string(el);
        size_t integer = stringNum.find('.');
        count += integer;
        if (integer > max_integer)
            max_integer = integer;
        size_t factional = integer;
        for (auto index = stringNum.length() - 1; index > integer; --index)
        {
            if (stringNum[index] == '0' or stringNum[index] == '.')
            {
                continue;
            }
            factional = index;
            break;
        }
        if (factional > max_factional)
            max_factional = factional;
        if (factional != integer)
        {
        count += factional + 2 - integer;
        }
        if (count > max_length)
            max_length = count;
    };
    std::for_each(matrix.begin(), matrix.end(), countDigits);
    os.precision(max_integer > max_factional ? max_integer : max_factional);
	auto lcol = matrix.col();
    auto len = (max_length + 3) * lcol + 1;
	for (size_t row = 1, lrow = matrix.row(); row <= lrow; ++row)
	{
		os << std::string(len, '-') << std::endl;
		for (size_t col = 1; col <= lcol; ++col)
		{
            os << "| " << std::setw(max_length) << matrix(row, col) << " ";
        }
        os << "|" << std::endl;
	};
	os << std::string(len, '-') << std::endl;
	return os;
}

std::ifstream& operator>>(std::ifstream& is, Matrix& matrix)
{
    if (!matrix.empty())
        matrix = Matrix();
    size_t  rows = 0,
            columns = 0;
    bool    isColumnsCounted = false,
            isReserved = false;
    double value;
    std::string text;
    std::stringstream ss;
    while(getline(is, text))
    {
        if (!getline(is, text))
            break;
        ss << text;
        ss.ignore(1);
        while (ss >> value)
        {
            if (!isColumnsCounted)
                ++columns;
            matrix.matrix.push_back(value);
            ss.ignore(2);
        }
        isColumnsCounted = true;
        if (!isReserved)
        {
            matrix.matrix.reserve(columns*columns);
            isReserved = true;
        }
        ++rows;
        ss.clear();
    }
    matrix.rows = rows;
    matrix.cols = columns;
    matrix.matrix.shrink_to_fit();
    return is;
}

 Matrix operator+(const Matrix& matrix, const double& num)
{
    Matrix mat(matrix);
	transform(std::execution::par, mat.begin(), mat.end(), mat.begin(), [&num](auto& el) {return el += num; });
	return mat;
}

 Matrix operator-(const Matrix& matrix, const double& num)
{
	return matrix + -num;
}

 Matrix operator*(const Matrix& matrix, const double& num)
{
    Matrix mat(matrix);
	transform(std::execution::par, mat.begin(), mat.end(), mat.begin(), [&num](auto& el) { return el *= num; });
	return mat;
}

 Matrix operator/(const Matrix& matrix, const double& num)
{
    Matrix mat(matrix);
	transform(std::execution::par, mat.begin(), mat.end(), mat.begin(), [&num](auto& el) {return el /= num; });
	return mat;
}

 bool operator==(const Matrix& matrix1, const Matrix& matrix2)
{
	if (matrix1.col() != matrix2.col()) return false;
	if (matrix1.row() != matrix2.row()) return false;
	return equal(std::execution::par, matrix1.begin(), matrix1.end(), matrix2.begin());
}

 Matrix operator+(const Matrix& matrix1, const Matrix& matrix2)
{
    Matrix mat(matrix1);
	transform(std::execution::par, mat.begin(), mat.end(), matrix2.begin(), mat.begin(), [](auto& el1, auto& el2) { return el1 += el2; });
	return mat;
}

 Matrix operator-(const Matrix& matrix1, const Matrix& matrix2)
{
    Matrix mat(matrix1);
	transform(std::execution::par, mat.begin(), mat.end(), matrix2.begin(), mat.begin(), [](auto& el1, auto& el2) { return el1 -= el2; });
	return mat;
}

  Matrix operator*(const Matrix& matrix1, const Matrix& matrix2)
{
	auto row1 = matrix1.row(), col1 = matrix1.col(), col2 = matrix2.col();
    Matrix mat(row1, col2);
	for (size_t col = 1; col <= col2; ++col)
	{
		for (size_t row = 1; row <= row1; ++row)
		{
			auto el = 0.0;
			for (size_t index = 1; index <= col1; ++index)
			{
				el += matrix1(row, index) * matrix2(index, col);
			}
			mat(row, col) = el;
		}
	}
	return mat;
}

 double Matrix::do_det(Matrix mat) const
{
	 if (mat.col() == 0) return 0.0;
	 else if (mat.col() == 1) return mat[0];
	 else if (mat.col() == 2) return mat(1, 1) * mat(2, 2) - mat(1, 2) * mat(2, 1);
	 else if (mat.col() == 3) return mat(1, 1) * mat(2, 2) * mat(3, 3) + mat(1, 2) * mat(2, 3) * mat(3, 1) + mat(1, 3) * mat(2, 1) * mat(3, 2) -
		 mat(1, 3) * mat(2, 2) * mat(3, 1) - mat(1, 1) * mat(2, 3) * mat(3, 2) - mat(1, 2) * mat(2, 1) * mat(3, 3);
	 else {
		 if (mat(1, 1) == 0)
         {
            auto beg = mat.colbegin() + 1, end = mat.colbegin() + mat.col();
            auto found = std::find_if(std::execution::par, beg, end, [](auto& el) {return el != 0; });
			 if (found == end) return 0;
             mat.swap_rows(1, found - beg + 2);
			 return -1 * mat.det();
		 }
		 else
		 {
			 for (size_t r = 2, lr = mat.row(); r <= lr; ++r)
			 {
				 auto c = mat.col();
				 auto row1_beg = mat.begin(),
					 row1_end = mat.begin() + c,
					 row2_beg = mat.begin() + (r - 1) * c;
				 auto mult = mat(r, 1) / mat(1, 1) * -1;
				 auto sum_rows = [&mult](auto& el1, auto& el2) { return el2 + el1 * mult; };
				 transform(row1_beg, row1_end, row2_beg, row2_beg, sum_rows);
			 }
			 auto mult = mat(1, 1);
			 auto det = mult * mat.minor(1, 1);
			 return det;
		 }
	 }
}

 Matrix Matrix::remove_col(const size_t& n)
{
	auto beg = begin() + n - 1;
	for (signed long long col = cols * (rows - 1); col >= 0; col -= cols)
    {
        erase(beg + col);
	}
	--cols;
	matrix.shrink_to_fit();
	return *this;
}

 Matrix Matrix::remove_row(const size_t& n)
{
	auto beg = begin() + (n - 1) * cols;
	erase(beg, beg + cols);
	--rows;
	matrix.shrink_to_fit();
	return *this;
}

Matrix Matrix::remove_cols(const size_t& col, const size_t& amount)
{
    auto beg = begin() + col - 1;
    for (signed long long column = (amount - 1) + cols * (rows - 1); column >= 0; column -= cols)
    {
        for (auto counter = amount; counter > 0; --counter, --column)
        {
             erase(beg + column);
        }
        column += amount;
    }
    cols -= amount;
    matrix.shrink_to_fit();
    return *this;
}

Matrix Matrix::remove_rows(const size_t& row, const size_t& amount)
{
    auto beg = begin() + (row - 1) * cols;
    erase(beg, beg + cols * amount);
    rows -= amount;
    matrix.shrink_to_fit();
    return *this;
}

Matrix Matrix::add_col(const size_t& col)
{
    Matrix mat(rows, cols + 1, 0);
    for (size_t row = 1; row <= rows; ++row)
    {
        size_t oldcolumn = 1;
        for (size_t newcolumn = 1; newcolumn <= cols + 1; ++newcolumn)
        {
             if (newcolumn == col) continue;
             mat(row, newcolumn) = (*this)(row, oldcolumn);
             ++oldcolumn;
        }
    }
    *this = mat;
    return *this;
}

Matrix Matrix::add_row(const size_t& row)
{
    Matrix mat(rows + 1, cols, 0);
    for (size_t column = 1; column <= cols; ++column)
    {
        size_t oldrow = 1;
        for (size_t newrow = 1; newrow <= rows + 1; ++newrow)
        {
             if (newrow == row) continue;
             mat(newrow, column) = (*this)(oldrow, column);
             ++oldrow;
        }
    }
    *this = mat;
    return *this;
}

Matrix Matrix::add_cols(const size_t& col, const size_t& amount)
{
    Matrix mat(rows, cols + amount, 0);
    for (size_t row = 1; row <= rows; ++row)
    {
        size_t oldcolumn = 1;
        for (size_t newcolumn = 1; newcolumn <= cols + amount; ++newcolumn)
        {
             if (newcolumn >= col and newcolumn < (col + amount)) continue;
             mat(row, newcolumn) = (*this)(row, oldcolumn);
             ++oldcolumn;
        }
    }
    *this = mat;
    return *this;
}

Matrix Matrix::add_rows(const size_t& row, const size_t& amount)
{
    Matrix mat(rows + amount, cols, 0);
    for (size_t column = 1; column <= cols; ++column)
    {
        size_t oldrow = 1;
        for (size_t newrow = 1; newrow <= rows + amount; ++newrow)
        {
             if (newrow >= row and newrow < (row + amount)) continue;
             mat(newrow, column) = (*this)(oldrow, column);
             ++oldrow;
        }
    }
    *this = mat;
    return *this;
}

 Matrix Matrix::do_transpose() const
{
    Matrix mat(cols, rows);
	for (size_t col = 1; col <= cols; ++col)
	{
		for (size_t row = 1; row <= rows; ++row)
		{
			mat(col, row) = (*this)(row, col);
		}
	}
	return mat;
}

 double Matrix::do_minor(const size_t& row, const size_t& col) const
{
    Matrix mat(*this);
    mat.remove_col(col);
    mat.remove_row(row);
    auto det = mat.det();
    return det;
}

 Matrix Matrix::do_inverse() const
{
	auto deter = det();
    Matrix minors(rows, cols);
	for (size_t row = 1; row <= rows; ++row)
	{
		for (size_t col = 1; col <= cols; ++col)
		{
			auto sign = (row + col) % 2 == 0 ? 1 : -1;
            minors(row, col) = minor(row, col) * sign;
            if (minors(row,col) == -0) minors(row,col) = 0;
		}
	}
	minors = minors.transpose();
	return minors / deter;
}
#include <QDebug>
 double& Matrix::operator()(const size_t& r, const size_t& c)
{
	return operator[]((c - 1) + (r - 1) * cols);
}

 const double& Matrix::operator()(const size_t& r, const size_t& c) const
{
	return operator[]((c - 1) + (r - 1) * cols);
}

 Matrix Matrix::make_identity()
{
	auto b = begin(), e = end();
	while (b != e) {
		*b = 1;
		++b;
		if (b == e) break;
		b = fill_n(std::execution::par, b, cols, 0);
	}
	return *this;
}

 void Matrix::swap_rows(const size_t& r1, const size_t& r2)
{
	if (r1 == r2) return;
    auto row1_beg = this->rowbegin() + (r1 - 1) * cols,
        row1_end = this->rowbegin() + r1 * cols,
        row2_beg = this->rowbegin() + (r2 - 1) * cols;
    swap_ranges(std::execution::par, row1_beg, row1_end, row2_beg);
}

 void Matrix::swap_cols(const size_t& c1, const size_t& c2)
{
	if (c1 == c2) return;
	auto col1_beg = this->colbegin() + (c1 - 1) * rows,
		col1_end = this->colbegin() + c1 * rows,
		col2_beg = this->colbegin() + (c2 - 1) * rows;
    std::swap_ranges(col1_beg, col1_end, col2_beg);
}

void Matrix::sum_rows(const size_t& r1, const size_t& r2)
{
	auto row2_beg = this->begin() + (r2 - 1) * cols,
		row2_end = this->begin() + r2 * cols,
		row1_beg = this->begin() + (r1 - 1) * cols;
	transform(std::execution::par, row2_beg, row2_end, row1_beg, row1_beg, [](auto& el1, auto& el2) { return el1 + el2; });
}

void Matrix::sum_cols(const size_t& c1, const size_t& c2)
{
	 auto col2_beg = colbegin() + (c2 - 1) * rows,
		 col2_end = colbegin() + c2 * rows,
		 col1_beg = colbegin() + (c1 - 1) * rows;
	 std::transform(col2_beg, col2_end, col1_beg, col1_beg, [](auto& el1, auto& el2) { return el1 + el2; });
}

void Matrix::sub_rows(const size_t& r1, const size_t& r2)
{
     auto row2_beg = this->begin() + (r2 - 1) * cols,
         row2_end = this->begin() + r2 * cols,
         row1_beg = this->begin() + (r1 - 1) * cols;
     transform(std::execution::par, row2_beg, row2_end, row1_beg, row1_beg, [](auto& el1, auto& el2) { return el2 - el1; });
}

void Matrix::sub_cols(const size_t& c1, const size_t& c2)
{
     auto col2_beg = colbegin() + (c2 - 1) * rows,
         col2_end = colbegin() + c2 * rows,
         col1_beg = colbegin() + (c1 - 1) * rows;
     std::transform(col2_beg, col2_end, col1_beg, col1_beg, [](auto& el1, auto& el2) { return el2 - el1; });
}

 void Matrix::row_add(const size_t& r, const double& num)
{
	auto row_beg = this->begin() + (r - 1) * cols,
		row_end = this->begin() + r * cols;
	transform(std::execution::par, row_beg, row_end, row_beg, [&num](auto& el1) { return el1 + num; });
}

 void Matrix::row_mult(const size_t& r , const double& num)
{
	auto row_beg = this->begin() + (r - 1) * cols,
		row_end = this->begin() + r * cols;
	transform(std::execution::par, row_beg, row_end, row_beg, [&num](auto& el1) { return el1 * num; });
}

 void Matrix::row_divide(const size_t& r, const double& num)
{
	auto row_beg = this->begin() + (r - 1) * cols,
		row_end = this->begin() + r * cols;
	transform(std::execution::par, row_beg, row_end, row_beg, [&num](auto& el1) { return el1 / num; });
}

 void Matrix::row_sub(const size_t& r, const double& num)
{
	auto row_beg = this->begin() + (r - 1) * cols,
		row_end = this->begin() + r * cols;
	transform(std::execution::par, row_beg, row_end, row_beg, [&num](auto& el1) { return el1 - num; });
}

 void Matrix::col_add(const size_t& c, const double& num)
{
	 auto col_beg = colbegin() + (c - 1) * rows,
		 col_end = colbegin() + c * rows;
	 std::transform(col_beg, col_end, col_beg, [&num](auto& el1) { return el1 + num; });
}

 void Matrix::col_mult(const size_t& c, const double& num)
{
	 auto col_beg = colbegin() + (c - 1) * rows,
		 col_end = colbegin() + c * rows;
	 std::transform(col_beg, col_end, col_beg, [&num](auto& el1) { return el1 * num; });
}

 void Matrix::col_divide(const size_t& c, const double& num)
{
	 auto col_beg = colbegin() + (c - 1) * rows,
		 col_end = colbegin() + c * rows;
	 std::transform(col_beg, col_end, col_beg, [&num](auto& el1) { return el1 / num; });
}

 void Matrix::col_sub(const size_t& c, const double& num)
{
	 auto col_beg = colbegin() + (c - 1) * rows,
		 col_end = colbegin() + c * rows;
	 std::transform(col_beg, col_end, col_beg, [&num](auto& el1) { return el1 - num; });
}


 Matrix& Matrix::operator=(const Matrix& rhs)
 {
	 rows = rhs.rows;
	 cols = rhs.cols;
	 matrix = rhs.matrix;
	 return *this;
 }

 Matrix::column_iterator Matrix::colbegin() noexcept
 {
	 return column_iterator{ *this, 1, 1 };
 }

 Matrix::const_column_iterator Matrix::colbegin() const noexcept
 {
	 return const_column_iterator{ *this, 1, 1 };
 }

 Matrix::column_iterator Matrix::colend() noexcept
 {
	 return column_iterator{ *this, 1, cols + 1 };
 }

 Matrix::const_column_iterator Matrix::colend() const noexcept
 { 
	 return const_column_iterator{ *this, 1, cols + 1 }; 
 }

 Matrix::const_column_iterator Matrix::ccolbegin() const noexcept
 {
	 return const_column_iterator{ *this, 1, 1 };
 }

 Matrix::const_column_iterator Matrix::ccolend() const noexcept
 {
	 return const_column_iterator{ *this, 1, cols + 1 };
 }

 
 Matrix::reverse_column_iterator Matrix::rcolbegin() noexcept
 {
	 return reverse_column_iterator{ *this, rows, cols };
 }

 Matrix::const_reverse_column_iterator Matrix::rcolbegin() const noexcept
 { 
	 return const_reverse_column_iterator{ *this, rows, cols };
 }

 Matrix::const_reverse_column_iterator Matrix::crcolbegin() const noexcept
 {
	 return const_reverse_column_iterator{ *this, rows, cols };
 }

 Matrix::reverse_column_iterator Matrix::rcolend() noexcept
 { 
	 return reverse_column_iterator{ *this, rows, 0 };
 }

 Matrix::const_reverse_column_iterator Matrix::rcolend() const noexcept
 { 
	 return const_reverse_column_iterator{ *this, rows, 0 }; 
 }

 Matrix::const_reverse_column_iterator Matrix::crcolend() const noexcept
 { 
	 return const_reverse_column_iterator{ *this, rows, 0 }; 
 }

 //column_iterator
 Matrix::column_iterator Matrix::column_iterator::operator++()
 {
	 if (row == mat.row())
	 {
		 row = 1;
		 ++col;
		 return *this;
	 }
	 else
	 {
		 ++row;
		 return *this;
	 }
 }

 Matrix::column_iterator Matrix::column_iterator::operator++(int)
 {
	 auto tmp = *this;
	 operator++();
	 return tmp;
 }

 Matrix::column_iterator Matrix::column_iterator::operator--()
 {
	 if (row == 1)
	 {
		 row = mat.row();
		 --col;
		 return *this;
	 }
	 else
	 {
		 --row;
		 return *this;
	 }
 }

 Matrix::column_iterator Matrix::column_iterator::operator--(int)
 {
	 auto tmp = *this;
	 operator--();
	 return tmp;
 }

 Matrix::column_iterator Matrix::column_iterator::operator+=(const difference_type& n)
 {
	 col += n / mat.row();
	 row += n % mat.row();
	 return *this;
 }

 Matrix::column_iterator Matrix::column_iterator::operator-=(const difference_type& n)
 {
	 col -= n / mat.row();
	 row -= n % mat.row();
	 return *this;
 }

 Matrix::column_iterator Matrix::column_iterator::operator+(const difference_type& n)
 {
	 auto tmp = (*this);
	 tmp += n;
	 return tmp;
 }

 Matrix::column_iterator Matrix::column_iterator::operator-(const difference_type& n)
 {
	 auto tmp = (*this);
	 tmp -= n;
	 return tmp;
 }

 Matrix::column_iterator::difference_type Matrix::column_iterator::operator-(const column_iterator& other) const
 {
	 return (col - other.col) * mat.col() + (row - other.row);
 }


 bool Matrix::column_iterator::operator==(const column_iterator& other)
 {
	 return (col == other.col) and (row == other.row);
 }

 bool Matrix::column_iterator::operator!=(const column_iterator& other)
 {
	 return !((*this) == other);
 }

 bool Matrix::column_iterator::operator>(const column_iterator& other)
 {
	 if (col == other.col) return row > other.row;
	 return col > other.col;
 }

 bool Matrix::column_iterator::operator<(const column_iterator& other)
 {
	 if (col == other.col) return row < other.row;
	 return col < other.col;
 }

 bool Matrix::column_iterator::operator<=(const column_iterator& other)
 {
	 return !((*this) > other);
 }

 bool Matrix::column_iterator::operator>=(const column_iterator& other)
 {
	 return !((*this) < other);
 }

 Matrix::column_iterator::reference Matrix::column_iterator::operator[] (const difference_type& n)
 {
	 return *((*this) + n);
 }

 Matrix::column_iterator::reference Matrix::column_iterator::operator*()
 {
	 return mat(row, col);
 }
 
 Matrix::column_iterator& Matrix::column_iterator::operator=(const column_iterator& other)
 {
	 if (*this == other) return *this;
	 col = other.col;
	 row = other.row;
	 mat = other.mat;
	 other.~column_iterator();
	 return *this;
 }
 //column_iterator

 //const_column_iterator
 Matrix::const_reverse_column_iterator::const_reverse_column_iterator(const reverse_column_iterator& colIt) : mat(colIt.mat), row(colIt.row), col(colIt.col) {}

 Matrix::const_column_iterator Matrix::const_column_iterator::operator++()
 {
	 if (row == mat.row())
	 {
		 row = 1;
		 ++col;
		 return *this;
	 }
	 else
	 {
		 ++row;
		 return *this;
	 }
 }

 Matrix::const_column_iterator Matrix::const_column_iterator::operator++(int)
 {
	 auto tmp = *this;
	 operator++();
	 return tmp;
 }

 Matrix::const_column_iterator Matrix::const_column_iterator::operator--()
 {
	 if (row == 1)
	 {
		 row = mat.row();
		 --col;
		 return *this;
	 }
	 else
	 {
		 --row;
		 return *this;
	 }
 }

 Matrix::const_column_iterator Matrix::const_column_iterator::operator--(int)
 {
	 auto tmp = *this;
	 operator--();
	 return tmp;
 }

 Matrix::const_column_iterator Matrix::const_column_iterator::operator+=(const difference_type& n)
 {
	 col += n / mat.row();
	 row += n % mat.row();
	 return *this;
 }

 Matrix::const_column_iterator Matrix::const_column_iterator::operator-=(const difference_type& n)
 {
	 col -= n / mat.row();
	 row -= n % mat.row();
	 return *this;
 }

 Matrix::const_column_iterator Matrix::const_column_iterator::operator+(const difference_type& n)
 {
	 auto tmp = (*this);
	 tmp += n;
	 return tmp;
 }

 Matrix::const_column_iterator Matrix::const_column_iterator::operator-(const difference_type& n)
 {
	 auto tmp = (*this);
	 tmp -= n;
	 return tmp;
 }

 Matrix::const_column_iterator::difference_type Matrix::const_column_iterator::operator-(const const_column_iterator& other) const
 {
	 return (col - other.col) * mat.col() + (row - other.row);
 }

 bool Matrix::const_column_iterator::operator==(const const_column_iterator& other)
 {
	 return (col == other.col) and (row == other.row);
 }

 bool Matrix::const_column_iterator::operator!=(const const_column_iterator& other)
 {
	 return !((*this) == other);
 }

 bool Matrix::const_column_iterator::operator>(const const_column_iterator& other)
 {
	 if (col == other.col) return row > other.row;
	 return col > other.col;
 }

 bool Matrix::const_column_iterator::operator<(const const_column_iterator& other)
 {
	 if (col == other.col) return row < other.row;
	 return col < other.col;
 }

 bool Matrix::const_column_iterator::operator<=(const const_column_iterator& other)
 {
	 return !((*this) > other);
 }

 bool Matrix::const_column_iterator::operator>=(const const_column_iterator& other)
 {
	 return !((*this) < other);
 }

 Matrix::const_column_iterator::value_type Matrix::const_column_iterator::operator[] (const difference_type& n)
 {
	 return *((*this) + n);
 }

 Matrix::const_column_iterator::value_type Matrix::const_column_iterator::operator*()
 {
	 return mat(row, col);
 }

 Matrix::const_column_iterator& Matrix::const_column_iterator::operator=(const const_column_iterator& other)
 {
	 row = other.row;
	 col = other.col;
	 mat = other.mat;
	 return *this;
 }
 //const_column_iterator

 //reverse_column_iterator
 Matrix::reverse_column_iterator Matrix::reverse_column_iterator::operator++()
 {
	 if (row == 1)
	 {
		 row = mat.row();
		 --col;
		 return *this;
	 }
	 else
	 {
		 --row;
		 return *this;
	 }
	 
 }

 Matrix::reverse_column_iterator Matrix::reverse_column_iterator::operator++(int)
 {
	 auto tmp = *this;
	 operator++();
	 return tmp;
 }

 Matrix::reverse_column_iterator Matrix::reverse_column_iterator::operator--()
 {
	 if (row == mat.row())
	 {
		 row = 1;
		 ++col;
		 return *this;
	 }
	 else
	 {
		 ++row;
		 return *this;
	 }
 }

 Matrix::reverse_column_iterator Matrix::reverse_column_iterator::operator--(int)
 {
	 auto tmp = *this;
	 operator--();
	 return tmp;
 }

 Matrix::reverse_column_iterator Matrix::reverse_column_iterator::operator+=(const difference_type& n)
 {
	 col -= n / mat.row();
	 row -= n % mat.row();
	 return *this;
 }

 Matrix::reverse_column_iterator Matrix::reverse_column_iterator::operator-=(const difference_type& n)
 {
	 col += n / mat.row();
	 row += n % mat.row();
	 return *this;
 }

 Matrix::reverse_column_iterator Matrix::reverse_column_iterator::operator+(const difference_type& n)
 {
	 auto tmp = (*this);
	 tmp += n;
	 return tmp;
 }

 Matrix::reverse_column_iterator Matrix::reverse_column_iterator::operator-(const difference_type& n)
 {
	 auto tmp = (*this);
	 tmp -= n;
	 return tmp;
 }

 Matrix::reverse_column_iterator::difference_type Matrix::reverse_column_iterator::operator-(const reverse_column_iterator& other) const
 {
	 return (other.col - col) * mat.col() + (other.row - row);
 }


 bool Matrix::reverse_column_iterator::operator==(const reverse_column_iterator& other)
 {
	 return (col == other.col) and (row == other.row);
 }

 bool Matrix::reverse_column_iterator::operator!=(const reverse_column_iterator& other)
 {
	 return !((*this) == other);
 }

 bool Matrix::reverse_column_iterator::operator>(const reverse_column_iterator& other)
 {
	 if (col == other.col) return row < other.row;
	 return col < other.col;
 }

 bool Matrix::reverse_column_iterator::operator<(const reverse_column_iterator& other)
 {
	 if (col == other.col) return row > other.row;
	 return col > other.col;
 }

 bool Matrix::reverse_column_iterator::operator<=(const reverse_column_iterator& other)
 {
	 return !((*this) > other);
 }

 bool Matrix::reverse_column_iterator::operator>=(const reverse_column_iterator& other)
 {
	 return !((*this) < other);
 }

 Matrix::reverse_column_iterator::reference Matrix::reverse_column_iterator::operator[] (const difference_type& n)
 {
	 return *((*this) + n);
 }

 Matrix::reverse_column_iterator::reference Matrix::reverse_column_iterator::operator*()
 {
	 return mat(row, col);
 }

 Matrix::reverse_column_iterator& Matrix::reverse_column_iterator::operator=(const reverse_column_iterator& other)
 {
	 if (*this == other) return *this;
	 col = other.col;
	 row = other.row;
	 mat = other.mat;
	 other.~reverse_column_iterator();
	 return *this;
 }

 Matrix::column_iterator Matrix::reverse_column_iterator::base()
 {
	 auto colit = column_iterator(mat, row, col);
	 return ++colit;
 }
 //reverse_column_iterator

 //const_reverse_column_iterator
 Matrix::const_column_iterator::const_column_iterator(const column_iterator& colIt) : mat(colIt.mat), row(colIt.row), col(colIt.col) {}

 Matrix::const_reverse_column_iterator Matrix::const_reverse_column_iterator::operator++()
 {
	 if (row == 1)
	 {
		 row = mat.row();
		 --col;
		 return *this;
	 }
	 else
	 {
		 --row;
		 return *this;
	 }

 }

 Matrix::const_reverse_column_iterator Matrix::const_reverse_column_iterator::operator++(int)
 {
	 auto tmp = *this;
	 operator++();
	 return tmp;
 }

 Matrix::const_reverse_column_iterator Matrix::const_reverse_column_iterator::operator--()
 {
	 if (row == mat.row())
	 {
		 row = 1;
		 ++col;
		 return *this;
	 }
	 else
	 {
		 ++row;
		 return *this;
	 }
 }

 Matrix::const_reverse_column_iterator Matrix::const_reverse_column_iterator::operator--(int)
 {
	 auto tmp = *this;
	 operator--();
	 return tmp;
 }

 Matrix::const_reverse_column_iterator Matrix::const_reverse_column_iterator::operator+=(const difference_type& n)
 {
	 col -= n / mat.row();
	 row -= n % mat.row();
	 return *this;
 }

 Matrix::const_reverse_column_iterator Matrix::const_reverse_column_iterator::operator-=(const difference_type& n)
 {
	 col += n / mat.row();
	 row += n % mat.row();
	 return *this;
 }

 Matrix::const_reverse_column_iterator Matrix::const_reverse_column_iterator::operator+(const difference_type& n)
 {
	 auto tmp = (*this);
	 tmp += n;
	 return tmp;
 }

 Matrix::const_reverse_column_iterator Matrix::const_reverse_column_iterator::operator-(const difference_type& n)
 {
	 auto tmp = (*this);
	 tmp -= n;
	 return tmp;
 }

 Matrix::const_reverse_column_iterator::difference_type Matrix::const_reverse_column_iterator::operator-(const const_reverse_column_iterator& other) const
 {
	 return (other.col - col) * mat.col() + (other.row - row);
 }


 bool Matrix::const_reverse_column_iterator::operator==(const const_reverse_column_iterator& other)
 {
	 return (col == other.col) and (row == other.row);
 }

 bool Matrix::const_reverse_column_iterator::operator!=(const const_reverse_column_iterator& other)
 {
	 return !((*this) == other);
 }

 bool Matrix::const_reverse_column_iterator::operator>(const const_reverse_column_iterator& other)
 {
	 if (col == other.col) return row < other.row;
	 return col < other.col;
 }

 bool Matrix::const_reverse_column_iterator::operator<(const const_reverse_column_iterator& other)
 {
	 if (col == other.col) return row > other.row;
	 return col > other.col;
 }

 bool Matrix::const_reverse_column_iterator::operator<=(const const_reverse_column_iterator& other)
 {
	 return !((*this) > other);
 }

 bool Matrix::const_reverse_column_iterator::operator>=(const const_reverse_column_iterator& other)
 {
	 return !((*this) < other);
 }

 Matrix::const_reverse_column_iterator::value_type Matrix::const_reverse_column_iterator::operator[] (const difference_type& n)
 {
	 return *((*this) + n);
 }

 Matrix::const_reverse_column_iterator::value_type Matrix::const_reverse_column_iterator::operator*()
 {
	 return mat(row, col);
 }

 Matrix::const_reverse_column_iterator& Matrix::const_reverse_column_iterator::operator=(const const_reverse_column_iterator& other)
 {
	 if (*this == other) return *this;
	 col = other.col;
	 row = other.row;
	 mat = other.mat;
	 other.~const_reverse_column_iterator();
	 return *this;
 }

 Matrix::const_column_iterator Matrix::const_reverse_column_iterator::base()
 {
	 auto colit = const_column_iterator(mat, row, col);
	 return ++colit;
 }
 //const_reverse_column_iterator
