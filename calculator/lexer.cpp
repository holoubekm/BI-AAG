#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <cctype>
#include <cstring>
#include <sstream>
#include <stack>
#include <cmath>

using namespace std;

class Token {

public:
	enum class Type {
		DECIMAL,
		OP_PLUS,
		OP_SUB,
		OP_MUL,
		OP_DIV,
		IDENT
	};

	union Value {
		double mDbl;
		char* mStr;
	};

	Token(Type type, const char* input = nullptr, int inputLen = 0) : mType(type) {
		switch(mType) {
			case Type::DECIMAL: {
				stringstream inp(string(input, input + inputLen));
				inp >> mVal.mDbl;
			}
			break;
			case Type::IDENT: {
				mVal.mStr = new char[inputLen + 1];
				memcpy(mVal.mStr, input, sizeof(char) * inputLen);
				mVal.mStr[inputLen] = '\0';
			}
			break;
			default:
			break;
		}
	}

	Token(double dbl) : mType(Type::DECIMAL) {
		mVal.mDbl = dbl;
	}

	~Token() {
		if(mType == Type::IDENT) {
			delete[] mVal.mStr;
		}
	}

	Type GetType() const {
		return mType;
	}

	Value GetValue() const {
		return mVal;
	}

	friend ostream& operator<<(ostream& os, const Token& tok) {
		switch(tok.mType) {
			case Token::Type::DECIMAL:
				os << "[" << tok.mVal.mDbl << "]";
				break;
			case Token::Type::IDENT:
				os << "[" << tok.mVal.mStr << "]";
				break;
			case Token::Type::OP_PLUS:
				os << "[+]";
				break;
			case Token::Type::OP_SUB:
				os << "[-]";
				break;
			case Token::Type::OP_MUL:
				os << "[*]"; 
				break;
			case Token::Type::OP_DIV:
				os << "[/]"; 
				break;
		}
		return os;
	}

	friend Token* operator+(const Token& lhs, const Token& rhs) {
		const auto lhsType = lhs.GetType();
		const auto rhsType = rhs.GetType();
		if(lhsType == Token::Type::DECIMAL) {
			if(rhsType == Token::Type::DECIMAL) 
				return new Token(lhs.GetValue().mDbl + rhs.GetValue().mDbl);
			else
				throw "Wrong types of argumets";
		} else {
			throw "Wrong types of argumets";
		}
	}

	friend Token* operator-(const Token& lhs, const Token& rhs) {
		const auto lhsType = lhs.GetType();
		const auto rhsType = rhs.GetType();
		if(lhsType == Token::Type::DECIMAL) {
			if(rhsType == Token::Type::DECIMAL) 
				return new Token(lhs.GetValue().mDbl - rhs.GetValue().mDbl);
			else
				throw "Wrong types of argumets";
		} else {
			throw "Wrong types of argumets";
		}
	}

	friend Token* operator*(const Token& lhs, const Token& rhs) {
		const auto lhsType = lhs.GetType();
		const auto rhsType = rhs.GetType();
		if(lhsType == Token::Type::DECIMAL) {
			if(rhsType == Token::Type::DECIMAL) 
				return new Token(lhs.GetValue().mDbl * rhs.GetValue().mDbl);
			else
				throw "Wrong types of argumets";
		} else {
			throw "Wrong types of argumets";
		}
	}

	friend Token* operator/(const Token& lhs, const Token& rhs) {
		const auto lhsType = lhs.GetType();
		const auto rhsType = rhs.GetType();
		if(lhsType == Token::Type::DECIMAL) {
			if(rhsType == Token::Type::DECIMAL) 
				return new Token(lhs.GetValue().mDbl / rhs.GetValue().mDbl);
			else
				throw "Wrong types of argumets";
		} else {
			throw "Wrong types of argumets";
		}
	}


private:
	Value mVal;
	Type mType;
};

const char* States[] = { "START", "IDENT", "NUM", "DOT", "DECIMAL", "FINISH", "ERROR" };

class Lexer {
public:
	enum class State {
		START,
		
		IDENT,

		NUM,
		DOT,
		DECIMAL,

		FINISH,
		ERROR
	};

	Lexer() : 
		mState(State::START),
		mLastToken(nullptr),
		mBufPos(0),
		mBufLen(8),
		mBuf(new char[mBufLen]) {
	}

	~Lexer() {
		delete[] mBuf;
		if(mLastToken != nullptr)
			delete mLastToken;
	}

	State NextState(char input) {

		switch(mState) {
			case State::START:
				if(isalpha(input)) {
					AppendBuf(input);
					mState = State::IDENT;
				} else if(isdigit(input)) {
					AppendBuf(input);
					mState = State::NUM;
				} else if(isblank(input) || iscntrl(input)) {
					mState = State::START;
				} else if(input == '+') {
					mState = State::FINISH;
					mLastToken = new Token(Token::Type::OP_PLUS);
				} else if(input == '-') {
					mState = State::FINISH;
					mLastToken = new Token(Token::Type::OP_SUB);
				} else if(input == '*') {
					mState = State::FINISH;
					mLastToken = new Token(Token::Type::OP_MUL);
				} else if(input == '/') {
					mState = State::FINISH;
					mLastToken = new Token(Token::Type::OP_DIV);
				} else {
					mState = State::ERROR;
					mLastToken = nullptr;
				}
				break;

			case State::IDENT:
				if(isalpha(input)) {
					AppendBuf(input);
					mState = State::IDENT;
				} else if(isblank(input) || iscntrl(input)) {
					mState = State::FINISH;
					mLastToken = new Token(Token::Type::IDENT, mBuf, mBufPos);
				} else {
					mState = State::ERROR;
					mLastToken = nullptr;
				}
			break;

			case State::NUM:
				if(isdigit(input)) {
					AppendBuf(input);
					mState = State::NUM;
				} else if(input == '.') {
					AppendBuf(input);
					mState = State::DOT;
				} else if(isblank(input) || iscntrl(input)) {
					mState = State::FINISH;
					mLastToken = new Token(Token::Type::DECIMAL, mBuf, mBufPos);
				} else {
					mState = State::ERROR;
					mLastToken = nullptr;
				}
			break;

			case State::DOT:
				if(isdigit(input)) {
					AppendBuf(input);
					mState = State::DECIMAL;
				} else {
					mState = State::ERROR;
					mLastToken = nullptr;
				}
			break;

			case State::DECIMAL:
				if(isdigit(input)) {
					AppendBuf(input);
					mState = State::DECIMAL;
				} else if(isblank(input) || iscntrl(input)) {
					mState = State::FINISH;
					mLastToken = new Token(Token::Type::DECIMAL, mBuf, mBufPos);
				} else {
					mState = State::ERROR;
					mLastToken = nullptr;
				}
			break;

			default:
				mState = State::ERROR;
				mLastToken = nullptr;
			break;
		}
		return mState;
	}

	Token* GetTokenAndReset() {
		auto tmp = mLastToken;
		Reset();
		return tmp;
	}

private:

	void Reset() {
		mState = State::START;
		mBufPos = 0;
		mLastToken = nullptr;
	}
	
	void AppendBuf(char input) {
		if(mBufPos == mBufLen - 1) {
			auto newBufLen = mBufLen * 2;
			auto newBuf = new char[newBufLen];
			memcpy(newBuf, mBuf, sizeof(char) * mBufLen);
			delete[] mBuf;
			mBuf = newBuf;
			mBufLen = newBufLen;
		}
		mBuf[mBufPos++] = input;
	}

private:
	State mState;
	Token* mLastToken;

	int mBufPos;
	int mBufLen;
	char* mBuf;
};

class Interpreter {
public:
	void Interpret(Token* token) {
		auto cnt = mStack.size();
		auto type = token->GetType();

		switch(type) {
			case Token::Type::DECIMAL:
				mStack.push(token);
				break;
			case Token::Type::OP_PLUS:
			case Token::Type::OP_SUB:
			case Token::Type::OP_MUL:
			case Token::Type::OP_DIV: {
				if(cnt < 2) {
					throw "Operator expects two operands!";
				}
				auto parA = mStack.top(); mStack.pop();
				auto parB = mStack.top(); mStack.pop();
				if(type == Token::Type::OP_PLUS)
					mStack.push(*parB + *parA);
				else if(type == Token::Type::OP_SUB)
					mStack.push(*parB - *parA);
				else if(type == Token::Type::OP_MUL)
					mStack.push(*parB * *parA);
				else if(type == Token::Type::OP_DIV)
					mStack.push(*parB / *parA);
				
				delete parA;
				delete parB;
			} break;
			case Token::Type::IDENT: {
				auto func = token->GetValue().mStr;	
				if(strcmp(func, "sin") == 0) {
					if(mStack.size() < 1 || mStack.top()->GetType() != Token::Type::DECIMAL)
						throw "Function sin expects one decimal argument";
					auto par = mStack.top(); mStack.pop();
					auto res = sin(par->GetValue().mDbl);
					mStack.push(new Token(res));
					delete par;
				} else if(strcmp(func, "cos") == 0) {
					if(mStack.size() < 1 || mStack.top()->GetType() != Token::Type::DECIMAL)
						throw "Function cos expects one decimal argument";
					auto par = mStack.top(); mStack.pop();
					auto res = cos(par->GetValue().mDbl);
					mStack.push(new Token(res));
					delete par;
				} else if(strcmp(func, "p") == 0) {
					auto size = mStack.size();
					Token** buf = new Token*[size];
					cout << "TOP: " << endl;
					for(decltype(size) x = 0; x < size; x++) {
						buf[x] = mStack.top(); mStack.pop();
						cout << ">> " << *buf[x] << endl;
					}
					cout << "BOT" << endl;
					for(int x = size - 1; x >= 0; x--) {
						mStack.push(buf[x]);
					}
					delete[] buf;
				} else {
					throw "Unknown function";
				}
			}
			break;
		}
	}

	~Interpreter() {
		Reset();
	}

	Token* GetResult() {
		if(mStack.size() != 1) 
			throw "Wrong count of elements at stack!";
		auto ret = mStack.top();
		mStack.pop();
		return ret;
	}

	void Reset() {
		while(!mStack.empty()) {
			delete mStack.top();
			mStack.pop();
		}
	}

private:
	stack<Token*> mStack;
};

int main(int, char**) {

	Lexer l;
	Interpreter i;
	char c;
	try {
		while((c = cin.get()) && c != EOF) {
			auto state = l.NextState(c);
			switch(state) {
				case Lexer::State::FINISH: {
					cout << "FINISH" << endl;
					auto token = l.GetTokenAndReset();
					cout << *token << endl;
					i.Interpret(token);
				}
				break;
				case Lexer::State::ERROR:
					cout << "ERROR: [" << c << "]" << endl;
				break;
				default:
					cout << States[static_cast<int>(state)] << endl;
				break;
			}
		} 
		auto res = i.GetResult();
		cout << "Result: " << *res << endl;
		delete res;
	}
	catch(const char* msg) {
		cout << "Exception: [" << msg << "]" << endl;
#ifdef _WIN32
		system("pause");
#endif
		return 1;
	}

#ifdef _WIN32
	system("pause");
#endif
	return 0;
}