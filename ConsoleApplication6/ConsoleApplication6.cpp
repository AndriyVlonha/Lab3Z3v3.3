#include <iostream>
#include <algorithm>
using namespace std;

class Matrix {
private:
    double* data;
    int rows, cols;
    mutable int errorState;
    static int objectCount;

public:
    Matrix() : rows(4), cols(3), errorState(0) {
        try {
            data = new double[rows * cols]();
            objectCount++;
        }
        catch (std::bad_alloc&) {
            errorState = 1;
            data = nullptr;
        }
    }

    Matrix(int n) : rows(n), cols(n), errorState(0) {
        try {
            data = new double[rows * cols]();
            objectCount++;
        }
        catch (std::bad_alloc&) {
            errorState = 1;
            data = nullptr;
        }
    }

    Matrix(int n, int m, double value = 0.0) : rows(n), cols(m), errorState(0) {
        try {
            data = new double[rows * cols];
            for (int i = 0; i < rows * cols; i++) data[i] = value;
            objectCount++;
        }
        catch (std::bad_alloc&) {
            errorState = 1;
            data = nullptr;
        }
    }

    Matrix(const Matrix& other) : rows(other.rows), cols(other.cols), errorState(0) {
        try {
            data = new double[rows * cols];
            for (int i = 0; i < rows * cols; i++) data[i] = other.data[i];
            objectCount++;
        }
        catch (std::bad_alloc&) {
            errorState = 1;
            data = nullptr;
        }
    }

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            delete[] data;
            rows = other.rows;
            cols = other.cols;
            errorState = 0;
            try {
                data = new double[rows * cols];
                for (int i = 0; i < rows * cols; i++) data[i] = other.data[i];
            }
            catch (std::bad_alloc&) {
                errorState = 1;
                data = nullptr;
            }
        }
        return *this;
    }

    ~Matrix() {
        delete[] data;
        objectCount--;
    }

    void setElement(int i, int j, double value = 0.0) {
        if (i < 0 || i >= rows || j < 0 || j >= cols) {
            errorState = 2;
            return;
        }
        data[i * cols + j] = value;
    }

    double getElement(int i, int j) const {
        if (i < 0 || i >= rows || j < 0 || j >= cols) {
            errorState = 2;
            return 0.0;
        }
        return data[i * cols + j];
    }

    void print() const {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) cout << data[i * cols + j] << " ";
            cout << endl;
        }
    }

    Matrix add(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            Matrix result(1);
            result.errorState = 3;
            return result;
        }
        Matrix result(rows, cols);
        for (int i = 0; i < rows * cols; i++) result.data[i] = data[i] + other.data[i];
        return result;
    }

    Matrix subtract(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            Matrix result(1);
            result.errorState = 3;
            return result;
        }
        Matrix result(rows, cols);
        for (int i = 0; i < rows * cols; i++) result.data[i] = data[i] - other.data[i];
        return result;
    }

    Matrix multiply(double scalar) const {
        Matrix result(rows, cols);
        for (int i = 0; i < rows * cols; i++) result.data[i] = data[i] * scalar;
        return result;
    }

    bool operator>(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) return false;
        for (int i = 0; i < rows * cols; i++) {
            if (data[i] <= other.data[i]) return false;
        }
        return true;
    }

    bool operator<(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) return false;
        for (int i = 0; i < rows * cols; i++) {
            if (data[i] >= other.data[i]) return false;
        }
        return true;
    }

    bool operator==(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) return false;
        for (int i = 0; i < rows * cols; i++) {
            if (data[i] != other.data[i]) return false;
        }
        return true;
    }

    int getErrorState() const { return errorState; }
    static int getObjectCount() { return objectCount; }
};

int Matrix::objectCount = 0;

int main() {
    Matrix m1;
    cout << "Default matrix (4x3):" << endl;
    m1.print();

    Matrix m2(2);
    cout << "\nSquare matrix (2x2):" << endl;
    m2.print();

    Matrix m3(2, 3, 1.5);
    cout << "\nCustom matrix (2x3) initialized with 1.5:" << endl;
    m3.print();

    Matrix m4 = m3;
    cout << "\nCopied matrix:" << endl;
    m4.print();

    m4.setElement(0, 0, 5.0);
    cout << "\nElement at (0,0): " << m4.getElement(0, 0) << endl;

    Matrix m5(2, 3, 1.0);
    Matrix sum = m3.add(m5);
    cout << "\nSum of matrices:" << endl;
    sum.print();

    Matrix diff = m3.subtract(m5);
    cout << "\nDifference of matrices:" << endl;
    diff.print();

    Matrix scaled = m3.multiply(2.0);
    cout << "\nMatrix multiplied by 2:" << endl;
    scaled.print();

    cout << "\nComparisons:" << endl;
    cout << "m3 > m5: " << (m3 > m5) << endl;
    cout << "m3 < m5: " << (m3 < m5) << endl;
    cout << "m3 == m4: " << (m3 == m4) << endl;

    cout << "\nTotal matrix objects: " << Matrix::getObjectCount() << endl;

    return 0;
}
