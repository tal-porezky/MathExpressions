#include <iostream>
#include <cmath>

class Expression {
protected:
    virtual void print(std::ostream& out) const = 0;
public:
    virtual double eval() const = 0;
    virtual ~Expression() = default;
    friend std::ostream& operator<<(std::ostream& out, const Expression& expression) {
        expression.print(out);
        return out;
    }
};

class Number : public Expression {
    const double m_val{};
protected:
    void print(std::ostream& out) const override {
        out << "(" << m_val << ")";
    }
public:
    explicit Number(double val) : m_val{ val } {}
    double eval() const override {
        return m_val;
    }
};

class BinaryExpression : public Expression {
    char m_sign{};
    const Expression* m_e1{};
    const Expression* m_e2{};
protected:
    void print(std::ostream& out) const override {
        out << *m_e1 << " " << m_sign << " " << *m_e2;
    }
    virtual double eval(double d1, double d2) const = 0;
public:
    BinaryExpression(char sign, const Expression* e1, const Expression* e2) : m_sign{ sign }, m_e1{ e1 }, m_e2{ e2 } {}
    double eval() const override {
        return eval(m_e1->eval(), m_e2->eval());
    }
};

class Sum : public BinaryExpression {
protected:
    double eval(double d1, double d2) const override {
        return d1 + d2;
    }
public:
    Sum(const Expression* e1, const Expression* e2) : BinaryExpression{ '+', e1, e2 } {}
};

class Mul : public BinaryExpression {
protected:
    double eval(double d1, double d2) const override {
        return d1 * d2;
    }
public:
    Mul(const Expression* e1, const Expression* e2) : BinaryExpression{ '*', e1, e2 } {}
};

class Exp : public BinaryExpression {
protected:
    double eval(double d1, double d2) const override {
        return std::pow(d1, d2);
    }
public:
    Exp(const Expression* e1, const Expression* e2) : BinaryExpression{ '^', e1, e2 } {}
};


int main()
{
    Expression* e = new Exp{ new Sum{ new Number{ -2 }, new Number{ 4 }}, new Number{ 5 } };
    std::cout << *e << " = " << e->eval() << "\n";
    Expression* e2 = new Sum( new Exp( new Number(2.0), new Number(3.0) ), new Number(-2) );
    std::cout << *e2 << " = " << e2->eval() << "\n";

    return 0;
}
