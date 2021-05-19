// RUN: %check_clang_tidy %s dampsoft-change-value-same-parameters %t
namespace std {
class string {
public:
  string(const char *) {
  }
};
} // namespace std

int changeValue(int a, int b);

int a;
int b;

auto e = changeValue(a, a);
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: changeValue mustn't be called with the same parameter twice! [dampsoft-change-value-same-parameters]

auto c = changeValue(a, b);

class D {
  int a;
  int b;
  std::string c = "";
  std::string d = "";

  void changeValue(int &a, int &b) {
    int c = a;
    a = b;
    b = c;
  }

  void changeValue(std::string a, std::string b) {
    std::string c = a;
    a = b;
    b = c;
  }

  void test() {
    int a = 5;
    std::string c = "a";

    this->changeValue(this->a, this->a);
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: changeValue mustn't be called with the same parameter twice! [dampsoft-change-value-same-parameters]
    this->changeValue(this->a, a);
    this->changeValue(a, b);
    this->changeValue(a, a);
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: changeValue mustn't be called with the same parameter twice! [dampsoft-change-value-same-parameters]
    this->changeValue(this->c, this->c);
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: changeValue mustn't be called with the same parameter twice! [dampsoft-change-value-same-parameters]
    this->changeValue(this->c, this->d);
    this->changeValue(this->c, c);
    this->changeValue(c, this->d);
  }
};
