#include"14_30_StrBlob.h"

inline bool operator==(const StrBlob & lhs, const StrBlob & rhs) {
    return *lhs.data == *rhs.data;
}
inline bool operator!=(const StrBlob & lhs, const StrBlob & rhs) {
    return !(lhs == rhs);
}
inline bool operator>(const StrBlob & lhs, const StrBlob & rhs) {
    return *lhs.data > *rhs.data;;
}
inline bool operator<(const StrBlob & lhs, const StrBlob & rhs) {
    return *lhs.data < *rhs.data;;
}
inline bool operator>=(const StrBlob & lhs, const StrBlob & rhs) {
    return *lhs.data >= *rhs.data;;
}
inline bool operator<=(const StrBlob & lhs, const StrBlob & rhs) {
    return *lhs.data <= *rhs.data;;
}

inline StrBlob & StrBlob::operator=(const StrBlob & sb) {
    auto p = std::make_shared<std::vector<std::string>>(*sb.data);
}
inline StrBlobPtr StrBlob::end( ) {
    StrBlobPtr ret = StrBlobPtr(*this, data->size( ));
    return ret;
}
inline StrBlobPtr StrBlob::begin( ) {
    return StrBlobPtr(*this);
}
void StrBlob::check(size_type i, const std::string & msg)const {
    if(i >= data->size( )) {
        throw std::out_of_range(msg);
    }
}



inline bool operator==(const StrBlobPtr & lhs, const StrBlobPtr & rhs) {
    auto l = lhs.wptr.lock( );
    auto r = rhs.wptr.lock( );
    if(l == r) {
        if(lhs.curr == rhs.curr) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}
inline bool operator!=(const StrBlobPtr & lhs, const StrBlobPtr & rhs) {
    return !(lhs == rhs);
}

inline std::string & StrBlobPtr::deref( )const {
    auto p = check(curr, "dereference past end");
    return (*p)[curr];
}
inline std::shared_ptr<std::vector<std::string>> StrBlobPtr::check(std::size_t i, const std::string & msg)const {
    auto ret = wptr.lock( );
    if(!ret) {
        throw std::runtime_error("unbound StrBlobPtr");
    }
    if(i >= ret->size( )) {
        throw std::out_of_range(msg);
    }
    return ret;
}

inline StrBlobPtr & StrBlobPtr::incr( ) {
    check(curr, "increment past end of StrBlobPtr");
    ++curr;
    return *this;
}
inline StrBlobPtr & StrBlobPtr::decr( ) {
    --curr;
    check(-1, "decrement past begin of StrBlobPtr");
    return *this;
}

std::string & StrBlobPtr::operator[](std::size_t n) {
    auto p = check(n, "unbound or out of range");
    return (*p)[n];
}
const std::string & StrBlobPtr::operator[](std::size_t n)const {
    auto p = check(n, "unbound or out of range");
    return (*p)[n];
}

StrBlobPtr & StrBlobPtr::operator++() {
    check(curr, "increment past end of StrBlobPtr");
    curr++;
    return *this;
}
StrBlobPtr & StrBlobPtr::operator--() {
    --curr;
    check(curr, "decrement past begin of StrBlobPtr");
    return *this;
}
StrBlobPtr StrBlobPtr::operator++(int) {
    auto ret = *this;
    ++*this;
    return ret;
}
StrBlobPtr StrBlobPtr::operator--(int) {
    auto ret = *this;
    --*this;
    return ret;
}

StrBlobPtr operator+(const StrBlobPtr & lhs, std::size_t n) {
    StrBlobPtr ret = lhs;
    ret.curr = lhs.curr + n;
    lhs.check(ret.curr, "past end of StrBlobPtr");
    return ret;
}
StrBlobPtr operator-(const StrBlobPtr & lhs, std::size_t n) {
    StrBlobPtr ret = lhs;
    ret.curr = lhs.curr - n;
    lhs.check(ret.curr, "past end of StrBlobPtr");
    return ret;
}

std::string & StrBlobPtr::operator*()const {
    auto p = check(curr, "dereference past end");
    return(*p)[curr];
}

std::string * StrBlobPtr::operator->()const {
    return &this->operator*();
}