
#ifndef SENTIMENT_VECTORS_H
#define SENTIMENT_VECTORS_H

#include <array>
#include <cmath>
#include <stdexcept>

struct Vector2
{
    Vector2();
    Vector2(const Vector2 & vec);
    Vector2(const float & x, const float & y);
    ~Vector2();

    Vector2 & operator=(const Vector2 & rhs);

    inline Vector2 operator+(Vector2 rhs);
    inline Vector2 operator-(Vector2 rhs);
    inline Vector2 operator*(const float & scalar);
    friend inline Vector2 operator*(const float & scalar, Vector2 rhs);

    inline Vector2 & operator+=(const Vector2 & rhs);
    inline Vector2 & operator-=(const Vector2 & rhs);
    inline Vector2 & operator*=(const float & scalar);

    bool operator==(const Vector2 & rhs);
    bool operator!=(const Vector2 & rhs);

    inline Vector2 & operator++();
    inline Vector2 & operator--();

    inline void Normal(Vector2 & rhs);

    Vector2 & Normalize();
    void Normalize(Vector2 & rhs);

    Vector2 & Dot(const Vector2 & rhs1, const Vector2 & rhs2);
    static float Cross(const Vector2 & lhs, Vector2 & rhs);

    float x;
    float y;
};

struct Vector3
{

    Vector3();
    Vector3(const Vector3 & vec);
    Vector3(const float & x, const float & y, const float & z);
    ~Vector3();

    inline Vector3 & operator=(const Vector3 & rhs);

    inline Vector3 operator+(Vector3 rhs);
    inline Vector3 operator-(Vector3 rhs);
    inline Vector3 operator*(const float & scalar);
    friend inline Vector3 operator*(const float & scalar, Vector3 rhs);

    inline Vector3 & operator+=(const Vector3 & rhs);
    inline Vector3 & operator-=(const Vector3 & rhs);
    inline Vector3 & operator*=(const float & rhs);

    inline bool operator==(const Vector3 & rhs);
    inline bool operator!=(const Vector3 & rhs);

    inline Vector3 & operator++();
    inline Vector3 & operator--();

    void Normalize();
    void Normalize(Vector3 rhs);

    void Dot(const Vector3 & vec1, const Vector3 & vec2);
    void Cross(const Vector3 & vec1, const Vector3 & vec2);

    float x;
    float y;
    float z;
};

struct Vector4
{
    Vector4();
    Vector4(const Vector4 & rhs);
    Vector4(const float & x, const float & y, const float & z, const float & w);
    ~Vector4();

    Vector4 & operator=(const Vector4 & rhs);

    inline Vector4 operator+(Vector4 rhs);
    inline Vector4 operator-(Vector4 rhs);
    inline Vector4 operator*(const float & scalar);
    friend inline Vector4 operator*(const float & scalar, Vector4 rhs);

    inline Vector4 & operator+=(const Vector4 & rhs);
    inline Vector4 & operator-=(const Vector4 & rhs);
    inline Vector4 & operator*=(const float & rhs);

    inline bool operator==(const Vector4 & rhs);
    inline bool operator!=(const Vector4 & rhs);

    inline Vector4 & operator++();
    inline Vector4 & operator--();

    float x;
    float y;
    float z;
    float w;
};

struct Quaternion
{
    Quaternion();
    Quaternion(const Quaternion & rhs);
    Quaternion(const Vector3 & v, const float & w);
    ~Quaternion();

    inline Quaternion & operator=(const Quaternion & rhs);

    inline bool operator==(const Quaternion & rhs);
    inline bool operator!=(const Quaternion & rhs);

    inline Quaternion operator*(Quaternion rhs);
    inline Quaternion & mul(const Quaternion & rhs);

    inline void Conjugate();
    inline void Conjugate(Quaternion &);

    inline void Normalize();
    inline void Normalize(Quaternion &);

    Vector3 v;
    float w;
};

template<int row, int col>
struct Matrix
{
private:
    static const int size = row * col;
    static const float zero_const;

public:
    Matrix();
    Matrix(const Matrix<row, col> & other);
    Matrix(std::array<float, size>);

    Matrix<row,col> & operator = (const Matrix<row, col> & rhs);

    float & operator()(int rows, int cols);
    const float& operator()(int rows, int cols) const;

    Matrix<row,col> & operator *= (const Matrix<row,col> & rhs);
    Matrix<row,col> & operator += (const Matrix<row,col> & rhs);
    Matrix<row,col> & operator -= (const Matrix<row,col> & rhs);
    Matrix<row,col> & operator *= (float rhs);
    Matrix<row,col> & operator /= (float rhs);

    template<int rhscol>
    Matrix<row, rhscol> operator * (const Matrix<col, rhscol> & rhs) const;

    Matrix<row,col> operator + (Matrix<row,col> rhs) const;
    Matrix<row,col> operator - (Matrix<row,col> rhs) const;
    Matrix<row,col> operator * (const float rhs) const;
    Matrix<row,col> operator / (const float rhs) const;

    template<int Row, int Col>
    friend Matrix<Row,Col> operator * (float lhs, const Matrix<Row,Col> & rhs);

    bool operator == (const Matrix<row,col> &) const;
    bool operator != (const Matrix<row,col> &) const;

    Matrix<row,col> & clear();
    static Matrix<row, row> & identity(Matrix<row, row> & mat);

    int row_length() const {return row;}
    int col_length() const {return col;}

private:
    union
    {
        float ar[size];
        float mat[row][col];
    };
};

#include "SENTIMENT_Math.hpp"

#endif
