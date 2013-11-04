
#ifndef SENTIMENT_MATH_HPP
#define SENTIMENT_MATH_HPP


inline Vector2 & Vector2::operator=(const Vector2 & rhs)
{
	x = rhs.x;
	y = rhs.y;

	return *this;
}

inline Vector2 Vector2::operator+(Vector2 rhs)
{
	rhs.x += x;
	rhs.y += y;

	return rhs;
}

inline Vector2 Vector2::operator-(Vector2 rhs)
{
	rhs.x -= x;
	rhs.y -= y;

	return rhs;
}

inline Vector2 Vector2::operator*(const float & scalar)
{
	Vector2 ret( x * scalar, y * scalar);

	return ret;
}

inline Vector2 operator*(const float & scalar, Vector2 rhs)
{
	rhs.x *= scalar;
	rhs.y *= scalar;

	return rhs;
}

inline Vector2 & Vector2::operator+=(const Vector2 & rhs)
{
	x += rhs.x;
	y += rhs.y;

	return *this;
}

inline Vector2 & Vector2::operator-=(const Vector2 & rhs)
{
	x -= rhs.x;
	y -= rhs.y;

	return *this;
}

inline Vector2 & Vector2::operator*=(const float & scalar)
{
	x *= scalar;
	y *= scalar;

	return *this;
}

inline bool Vector2::operator==(const Vector2 & rhs)
{
	if(rhs.x == x && rhs.y == y)
		return true;
	else
		return false;
}

inline bool Vector2::operator!=(const Vector2 & rhs)
{
	if(rhs.x != x || rhs.y != y)
		return true;
	else
		return false;
}

inline Vector2 & Vector2::operator++()
{
	x++;
	y++;

	return *this;
}

inline Vector2 & Vector2::operator--()
{
	x--;
	y--;

	return *this;
}

//----------------------------------------------
// Vector3 inline definitions
//----------------------------------------------


inline Vector3 & Vector3::operator=(const Vector3 & rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;

	return *this;
}

inline Vector3 Vector3::operator+(Vector3 rhs)
{
	rhs.x += x;
	rhs.y += y;
	rhs.z += z;

	return rhs;
}

inline Vector3 Vector3::operator-(Vector3 rhs)
{
	rhs.x -= x;
	rhs.y -= y;
	rhs.z -= z;

	return rhs;
}

inline Vector3 Vector3::operator*(const float & rhs)
{
	Vector3 ret(x * rhs, y * rhs, z * rhs);
	return ret;
}

inline Vector3 operator*(const float & scalar, Vector3 rhs)
{
	rhs.x *= scalar;
	rhs.y *= scalar;
	rhs.z *= scalar;

	return rhs;
}

inline Vector3& Vector3::operator+=(const Vector3 & rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;

	return *this;
}

inline Vector3& Vector3::operator-=(const Vector3 & rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;

	return *this;
}

inline Vector3 & Vector3::operator*=(const float & rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;

	return *this;
}

inline bool Vector3::operator==(const Vector3 & rhs)
{
	if(x == rhs.x && y == rhs.y && z == rhs.z)
		return true;
	else
		return false;
}

inline bool Vector3::operator!=(const Vector3 & rhs)
{
	if(x != rhs.x || y != rhs.y || z != rhs.z)
		return true;
	else
		return false;
}

inline Vector3 & Vector3::operator++()
{
	x++;
	y++;
	z++;

	return *this;
}

inline Vector3 & Vector3::operator--()
{
	x--;
	y--;
	z--;

	return *this;
}

//-----------------------------------------
// Vector4 defintions
//-----------------------------------------

inline Vector4 & Vector4::operator=(const Vector4 & rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;

	return *this;
}

inline Vector4 Vector4::operator+(Vector4 rhs)
{
	rhs.x += x;
	rhs.y += y;
	rhs.z += z;
	rhs.w += w;

	return rhs;
}

inline Vector4 Vector4::operator-(Vector4 rhs)
{
	rhs.x += x;
	rhs.y += y;
	rhs.z += z;
	rhs.w += w;

	return rhs;
}

inline Vector4 Vector4::operator*(const float & rhs)
{
	Vector4 ret( x * rhs, y * rhs, z * rhs, w * rhs);
	return ret;
}

inline Vector4 operator*(const float & scalar, Vector4 rhs)
{
	rhs.x *= scalar;
	rhs.y *= scalar;
	rhs.z *= scalar;
	rhs.w *= scalar;

	return rhs;
}

inline Vector4 & Vector4::operator+=(const Vector4 & rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;

	return *this;
}

inline Vector4& Vector4::operator-=(const Vector4 & rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;

	return *this;
}

inline Vector4& Vector4::operator*=(const float & rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	w *= rhs;

	return *this;
}

inline bool Vector4::operator==(const Vector4 & rhs)
{
	if(x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w)
		return true;
	else
		return false;
}

inline bool Vector4::operator!=(const Vector4 & rhs)
{
	if(x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w)
		return true;
	else
		return false;
}

inline Vector4& Vector4::operator++()
{
	x++;
	y++;
	z++;
	w++;

	return *this;
}

inline Vector4& Vector4::operator--()
{
	x--;
	y--;
	z--;
	w--;

	return *this;
}

//---------------------------------
// Quaternion definitions
//---------------------------------

inline Quaternion & Quaternion::operator=(const Quaternion & rhs)
{
	v = rhs.v;
	w = rhs.w;

	return *this;
}

inline bool Quaternion::operator==(const Quaternion & rhs)
{
	if(v == rhs.v && w == rhs.w)
		return true;
	else
		return false;
}

inline bool Quaternion::operator!=(const Quaternion & rhs)
{
	if(v != rhs.v || w != rhs.w)
		return true;
	else
		return false;
}

//---------------------------------
// Matrix definitions
//---------------------------------


template<int row, int col>
const float Matrix<row, col>::zero_const = 0.000002f;

// default constructor
template<int row, int col>
Matrix<row,col>::Matrix()
{
    static_assert((row > 0), "Matrix must have at least 1 row");
    static_assert((col > 0), "Matrix must have at least 1 column");

    for(int i = 0; i < size; i++)
    {
        ar[i] = 0.0f;
    }
}

// copy constructor
template<int row, int col>
Matrix<row, col>::Matrix(const Matrix<row, col> & mat)
{
    static_assert((row > 0), "Matrix must have at least 1 row");
    static_assert((col > 0), "Matrix must have at least 1 column");

    for(int i = 0; i < size; i++)
    {
        ar[i] = mat.ar[i];
    }
}

// explicit constructor
template<int row, int col>
Matrix<row, col>::Matrix(std::array<float, size> args)
{
    static_assert((row > 0), "Matrix must have at least 1 row");
    static_assert((col > 0), "Matrix must have at least 1 column");
    for(int i = 0; i < size; i++)
    {
        ar[i] = args[i];
    }
}

template<int row, int col>
float & Matrix<row,col>::operator()(int rows, int cols)
{
    if(rows >= row || rows < 0)
        throw std::out_of_range("outside the range of rows");
    if(cols >= col || cols < 0)
        throw std::out_of_range("outside the range of cols");

    return mat[rows][cols];
}

template<int row, int col>
const float& Matrix<row, col>::operator()(int rows, int cols) const
{
    if(rows >= row || rows < 0)
        throw std::out_of_range("outside the range of rows");
    if(cols >= col || cols < 0)
        throw std::out_of_range("outside the range of cols");

    return mat[rows][cols];
}

template<int row, int col>
Matrix<row,col>& Matrix<row,col>::operator *= (const Matrix<row,col> & rhs)
{
    Matrix<row, col> temp(*this);

    // i = row
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < rhs.col; j++)
        {
            for(int k = 0; k < col; k++)
            {
                mat[i][j] += temp[i][k] * rhs.mat[k][j];
            }
        }
    }

    return *this;
}

template<int row, int col>
Matrix<row,col> & Matrix<row,col>::operator += (const Matrix<row,col> & rhs)
{
    for(int i = 0; i < size; i++)
    {
        ar[i] += rhs.ar[i];
    }

    return *this;
}

template<int row, int col>
Matrix<row,col> & Matrix<row,col>::operator -= (const Matrix<row,col> & rhs)
{
    for(int i = 0; i < size; i++)
    {
        ar[i] -= rhs.ar[i];
    }

    return *this;
}

template<int row, int col>
Matrix<row,col> & Matrix<row,col>::operator *= (float rhs)
{
    for(int i = 0; i < size; i++)
    {
        ar[i] *= rhs;
    }


    return *this;
}

template<int row, int col>
Matrix<row,col> & Matrix<row,col>::operator /= (float rhs)
{
    if(std::fabs(rhs) < 0.000001f) rhs += zero_const;

    for(int i = 0; i < size; i++)
    {
        ar[i] /= rhs;
    }

    return *this;
}

template<int row, int col>
template<int rhscol>
Matrix<row, rhscol> Matrix<row, col>::operator* (const Matrix<col, rhscol> & rhs) const
{
    Matrix<row, rhscol> returnVal;

    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < rhscol; j++)
        {
            for(int k = 0; k < col; k++)
            {
                returnVal(i,j) += mat[i][k] * rhs(k,j);
            }
        }
    }

    return returnVal;
}

template<int row, int col>
Matrix<row, col> Matrix<row,col>::operator + (Matrix<row,col> rhs) const
{
    for(int i = 0; i < size; i++)
    {
        rhs.ar[i] += ar[i];
    }

    return rhs;
}

template<int row, int col>
Matrix<row, col> Matrix<row, col>::operator - (Matrix<row, col> rhs) const
{
    for(int i = 0; i < size; i++)
    {
        rhs.ar[i] -= ar[i];
    }

    return rhs;
}

template<int row, int col>
Matrix<row, col> Matrix<row, col>::operator * (float rhs) const
{
    Matrix<row, col> returnVal;

    for(int i = 0; i < size; i++)
    {
        returnVal.ar[i] = ar[i] * rhs;
    }

    return returnVal;
}

template<int row, int col>
Matrix<row, col> Matrix<row, col>::operator / (float rhs) const
{
    Matrix<row, col> returnVal;
    if(std::fabs(rhs) < 0.000001f) rhs += zero_const;

    for(int i = 0; i < size; i++)
    {
        returnVal.ar[i] = ar[i] / rhs;
    }

    return returnVal;
}

template<int row, int col>
Matrix<row, col> operator* (const float lhs, const Matrix<row, col> & rhs)
{
    Matrix<row, col> returnVal;

    for(int i = 0; i < rhs.size; i++)
    {
        returnVal.ar[i] = rhs.ar[i] * lhs;
    }

    return returnVal;
}

template<int row, int col>
bool Matrix<row, col>::operator == (const Matrix<row, col> & rhs) const
{
    bool is_equal = true;

    for(int i = 0; i < size; i++)
    {
        if(ar[i] != rhs.ar[i])
        {
            is_equal = false;
            break;
        }
    }

    return is_equal;
}

template<int row, int col>
bool Matrix<row, col>::operator != (const Matrix<row, col> & rhs) const
{
    return !(this->operator==(rhs));
}

template<int row, int col>
Matrix<row, col> & Matrix<row,col>::clear()
{
    for(int i = 0; i < size; i++)
    {
        ar[i] = 0.0f;
    }

    return *this;
}

template<int row, int col>
Matrix<row, row> & Matrix<row, col>::identity(Matrix<row, row> & mat)
{
    mat.clear();

    for(int i = 0; i < row; i++)
    {
        mat(i,i) = 1.0f;
    }

    return mat;
}


#endif
