#ifndef TEXTQUERY
#define TEXTQUERY
#include <algorithm>
#include <fstream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <memory>
#include <string>
#include <vector>
class ConstStrBlobPtr;
class StrBlob {
  friend class ConstStrBlobPtr;

 public:
  using size_type = std::vector<std::string>::size_type;
  StrBlob();
  StrBlob(std::initializer_list<std::string> il);
  size_type size() const { return data->size(); }
  bool empty() const { return data->empty(); }
  void push_back(const std::string &s) { data->push_back(s); }
  void pop_back();
  std::string &front();
  std::string &back();
  const std::string &front() const;
  const std::string &back() const;

  ConstStrBlobPtr begin() const;
  ConstStrBlobPtr end() const;

 private:
  std::shared_ptr<std::vector<std::string>> data;
  void check(size_type i, const std::string &msg) const;
};

StrBlob::StrBlob() : data(std::make_shared<std::vector<std::string>>()) {}
StrBlob::StrBlob(std::initializer_list<std::string> il)
    : data(std::make_shared<std::vector<std::string>>(il)) {}

void StrBlob::check(size_type i, const std::string &msg) const {
  if (i >= data->size()) {
    throw std::out_of_range(msg);
  }
}

void StrBlob::pop_back() {
  check(0, "pop_back on empty StrBlob");
  data->pop_back();
}

std::string &StrBlob::front() {
  check(0, "front on empty StrBlob");
  return data->front();
}

std::string &StrBlob::back() {
  check(0, "back on empty StrBlob");
  return data->back();
}

const std::string &StrBlob::front() const {
  check(0, "front on empty StrBlob");
  return data->front();
}

const std::string &StrBlob::back() const {
  check(0, "back on empty StrBlob");
  return data->back();
}

class ConstStrBlobPtr {
 private:
  std::shared_ptr<std::vector<std::string>> check(std::size_t,
                                                  const std::string &) const;
  std::weak_ptr<std::vector<std::string>> wptr;
  std::size_t curr;

 public:
  ConstStrBlobPtr() : curr(0) {}
  ConstStrBlobPtr(const StrBlob &a, size_t sz = 0) : wptr(a.data), curr(sz) {}
  std::string &deref() const;
  ConstStrBlobPtr &incr();
  bool operator!=(const ConstStrBlobPtr &p) { return p.curr != curr; }
};

std::shared_ptr<std::vector<std::string>> ConstStrBlobPtr::check(
    std::size_t i, const std::string &msg) const {
  auto ret = wptr.lock();
  if (!ret) {
    throw std::runtime_error("unbound ConstStrBlobPtr");
  }
  if (i >= ret->size()) {
    throw std::out_of_range(msg);
  }
  return ret;
}

std::string &ConstStrBlobPtr::deref() const {
  auto p = check(curr, "dereference past end");
  return (*p)[curr];
}

ConstStrBlobPtr &ConstStrBlobPtr::incr() {
  check(curr, "increment past end of ConstStrBlobPtr");
  ++curr;
  return *this;
}

ConstStrBlobPtr StrBlob::begin() const { return ConstStrBlobPtr(*this); }

ConstStrBlobPtr StrBlob::end() const {
  auto ret = ConstStrBlobPtr(*this, data->size());
  return ret;
}

class QueryResult;

class TextQuery {
  friend class QueryResult;
  using line_no = StrBlob::size_type;

 private:
  std::shared_ptr<StrBlob> file;
  std::map<std::string, std::shared_ptr<std::set<line_no>>> wm;

 public:
  TextQuery(std::ifstream &is);
  QueryResult query(const std::string &s) const;
};

class QueryResult {
  friend std::ostream &print(std::ostream &os, const QueryResult &qr);
  using ResultIter = std::set<TextQuery::line_no>::iterator;

 private:
  std::string sought;
  std::shared_ptr<StrBlob> file;
  std::shared_ptr<std::set<TextQuery::line_no>> lines;

 public:
  QueryResult(std::string s, std::shared_ptr<StrBlob> f,
              std::shared_ptr<std::set<TextQuery::line_no>> l)
      : sought(s), file(f), lines(l) {}
  ResultIter begin() const { return lines->begin(); }
  ResultIter end() const { return lines->end(); }
  std::shared_ptr<StrBlob> get_file() const { return file; }
};

// class TextQuery

TextQuery::TextQuery(std::ifstream &is) : file(std::make_shared<StrBlob>()) {
  std::string text;
  while (std::getline(is, text)) {
    file->push_back(text);
    int n = file->size() - 1;
    std::istringstream line(text);
    std::string word;
    while (line >> word) {
      auto end = std::find_if(word.begin(), word.end(), ispunct);
      word.erase(end, word.end());
      auto &lines = wm[word];
      if (!lines) {
        lines.reset(new std::set<line_no>);
      }
      lines->insert(n);
    }
  }
}

QueryResult TextQuery::query(const std::string &s) const {
  static std::shared_ptr<std::set<TextQuery::line_no>> nodata(
      std::make_shared<std::set<line_no>>());
  auto loc = wm.find(s);
  if (loc != wm.end()) {
    return QueryResult(s, file, loc->second);
  } else {
    return QueryResult(s, file, nodata);
  }
}

// print

std::ostream &print(std::ostream &os, const QueryResult &qr) {
  os << qr.sought << " occurs " << qr.lines->size() << " time(s). "
     << std::endl;
  for (auto &x : *qr.lines) {
    ConstStrBlobPtr p{*qr.file, x};
    os << "(line " << x + 1 << ") " << p.deref() << std::endl;
  }
  return os;
}
#endif  // !TEXTQUERY
