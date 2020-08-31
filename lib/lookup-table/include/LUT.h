#pragma once
#include <iterator>
#include <optional>
#include <unordered_map>

// This a LUT type. It is not meant to be mutated after initilization.
template < class Key, class Value, class Hash = std::hash<Key>, class KeyEqual = std::equal_to<Key>,class Allocator = std::allocator<std::pair<const Key, Value>> > 
class LUT {
 private:
 const std::unordered_map<Key, Value, Hash, KeyEqual, Allocator> _unordered_map;

  public:
  using key_type = Key;
  using mapped_type = Value;
  using value_type = std::pair<const Key, Value>;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;
  using hasher = Hash;
  using key_equal = KeyEqual;
  using allocator_type = Allocator;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = std::allocator_traits<Allocator>::pointer;
  using const_pointer = std::allocator_traits<Allocator>::const_pointer;
  
  LUT() = delete;

  // range based constructor
  template <typename InputIterator>
  LUT(InputIterator first, InputIterator last) : _unordered_map(first, last) {}
  // copy constructor and assignment
  LUT(const LUT &copy) = default;
  LUT& operator= (const LUT &) = default;	
  // move constructor and assignment
  LUT(LUT &&move) = default;
  LUT& operator= (LUT &&) = default;	
  // initializer list constructor
  LUT(std::initializer_list<value_type> init, size_type bucket_count = 0, const Hash& hash = Hash(), const key_equal& equal = key_equal(), const Allocator& alloc = Allocator()) : _unordered_map(init, bucket_count, hash, equal, alloc) {}

  typename std::unordered_map<Key, Value>::iterator begin() {
    return _unordered_map.begin();
  }
  typename std::unordered_map<Key, Value>::iterator end() {
    return _unordered_map.end();
  }

  std::size_t size() const {
    return _unordered_map.size();
  }
  std::size_t max_size() const {
    return _unordered_map.max_size();
  }

  bool empty() const {
    return _unordered_map.empty();
  }

  std::optional<Value> at(const Key &key) const {
    if (_unordered_map.find(key) == _unordered_map.end()) return std::nullopt;
    return _unordered_map.at(key);
  }
  std::optional<Value> operator[](const Key &key) const {
    return at(key);
  }
};
