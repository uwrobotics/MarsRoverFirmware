#pragma once
#include <iterator>
#include <optional>
#include <unordered_map>

// This a LUT type. It is not meant to be mutated after initilization.
template <class Key, class Value, class Hash = std::hash<Key>, class KeyEqual = std::equal_to<Key>,
          class Allocator = std::allocator<std::pair<const Key, Value>>>
class LookupTable {
 private:
  const std::unordered_map<Key, Value, Hash, KeyEqual, Allocator> _unordered_map;
  const Value _defaultValue; 

 public:
  using key_type        = Key;
  using mapped_type     = Value;
  using value_type      = std::pair<const Key, Value>;
  using size_type       = size_t;
  using difference_type = std::ptrdiff_t;
  using hasher          = Hash;
  using key_equal       = KeyEqual;
  using allocator_type  = Allocator;
  using reference       = value_type &;
  using const_reference = const value_type &;
  using pointer         = std::allocator_traits<Allocator>::pointer;
  using const_pointer   = std::allocator_traits<Allocator>::const_pointer;

  LookupTable() = delete;

  // range based constructor
  template <typename InputIterator>
  LookupTable(InputIterator first, InputIterator last, Value defaultValue) : _unordered_map(first, last), _defaultValue(defaultValue) {}
  // copy constructor and assignment
  LookupTable(const LookupTable &copy) = default;
  LookupTable &operator=(const LookupTable &) = default;
  // move constructor and assignment
  LookupTable(LookupTable &&move) = default;
  LookupTable &operator=(LookupTable &&) = default;
  // initializer list constructor
  LookupTable(std::initializer_list<value_type> init, size_type bucket_count = 0, const Hash &hash = Hash(),
              const key_equal &equal = key_equal(), const Allocator &alloc = Allocator())
      : _unordered_map(init, bucket_count, hash, equal, alloc) {}

  typename std::unordered_map<Key, Value>::iterator begin() const {
    return _unordered_map.begin();
  }
  typename std::unordered_map<Key, Value>::iterator end() const {
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
    bool value_exists = _unordered_map.find(key) == _unordered_map.end();
    MBED_ASSERT(value_exists);
    return value_exists ? std::nullopt : std::optional<Value>{_unordered_map.at(key)};
  }
  std::optional<Value> operator[](const Key &key) const {
    return at(key);
  }
};
